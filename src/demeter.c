// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such plugin, much data!
//___________________________________________________________________________________________________________________________________________
// INCLUDES
//___________________________________________________________________________________________________________________________________________
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "slurm/slurm.h"
#include "slurm/slurm_errno.h"
#include "src/common/prep.h"
#include "src/common/cgroup.h"
#include "src/common/xmalloc.h"
#include "demeter.h"
#include "gather_ib.h"

// GLOBAL VARIABLES
//___________________________________________________________________________________________________________________________________________
//setting required variables for slurm:
#define DEMETER_PLUGIN_NAME "prep/demeter: "
const char plugin_name[] = "Demeter godess of data harvest.";
const char plugin_type[] = "prep/demeter";
const uint32_t plugin_version = SLURM_VERSION_NUMBER;
//Disable the plugin for controller:
// static bool have_prolog_slurmctld = false;
// static bool have_epilog_slurmctld = false;
void (*prolog_slurmctld_callback)(int rc, uint32_t job_id) = NULL;
void (*epilog_slurmctld_callback)(int rc, uint32_t job_id) = NULL;
//Data used by the plugin.
static job_id_info_t *job_info = NULL;
static linked_list_t *cgroup_data = NULL, *gathered_sys_logs = NULL,
*gathered_sel = NULL, *gathered_slurm_logs = NULL;
static log_counter_t *sys_log_counter = NULL, *slurm_log_counter = NULL;
static perf_data_t *gathered_perf_data = NULL, *gathered_perf_data_diff = NULL;
static demeter_conf_t *demeter_conf = NULL;

// PLUGIN INITIALIZATION AND EXIT FUNCTIONS
//___________________________________________________________________________________________________________________________________________
extern int init (void)
{
    FILE *log_file = NULL;

    debug(DEMETER_PLUGIN_NAME "starting");
    demeter_conf = read_conf();
    log_file = init_log_file(demeter_conf, false);
    //check if log file is writable:
    if (log_file == NULL || demeter_conf == NULL)
        return (SLURM_ERROR);
    debug2(DEMETER_PLUGIN_NAME "log file initialized");
    fclose(log_file);
    write_log_to_file(demeter_conf, "demeter started", INFO, 0);
    info(DEMETER_PLUGIN_NAME "started, thank god!");
    return (SLURM_SUCCESS);
}

extern void fini (void)
{
    if (demeter_conf != NULL) {
        if (!is_log_empty(demeter_conf->log_file_path))
            free(demeter_conf->log_file_path);
        free(demeter_conf);
    }
    info(DEMETER_PLUGIN_NAME "stopped");
}

// USED
//___________________________________________________________________________________________________________________________________________
extern int prep_p_prolog(job_env_t *job, slurm_cred_t *cred)
{
    write_log_to_file(demeter_conf, "getting cgroup file path", DEBUG, 3);
    if (job == NULL)
        return (SLURM_ERROR);
    job_info = get_job_info(job);
    gathered_perf_data = gather_ib();
    return (SLURM_SUCCESS);
}

extern int prep_p_epilog(job_env_t *job_env, slurm_cred_t *cred)
{
    if (demeter_conf && demeter_conf->using_task_plugin)
        cgroup_data = unlog_cgroup(demeter_conf, job_info->job_id);
    gathered_sys_logs = gather_all_sys_logs(demeter_conf, job_info, &sys_log_counter);
    gathered_slurm_logs = gather_all_slurm_logs(demeter_conf, job_info, &slurm_log_counter);
    gathered_sel = gather_sel(job_info);
    gathered_perf_data_diff = gather_ib_diff(gathered_perf_data);
    write_log_to_file(demeter_conf, "call to send_elastic", INFO, 0);
    send_logs_elastic(demeter_conf, job_info, gathered_slurm_logs,
    gathered_sys_logs, gathered_sel);
    send_elastic(demeter_conf, job_info, cgroup_data,
    gathered_slurm_logs, slurm_log_counter,
    gathered_sys_logs, sys_log_counter,
    gathered_sel, gathered_perf_data_diff);
    debug(DEMETER_PLUGIN_NAME "stopping");
    free_log_list(gathered_slurm_logs);
    free_log_list(gathered_sys_logs);
    free_log_counter(slurm_log_counter);
    free_log_counter(sys_log_counter);
    free_sel_list(gathered_sel);
    free_job_id_info(job_info);
    free_cgroup_list(cgroup_data);
    free_perf_count(gathered_perf_data);
    free_perf_count(gathered_perf_data_diff);
    return (SLURM_SUCCESS);
}

// REQUIRED
//___________________________________________________________________________________________________________________________________________

extern void prep_p_register_callbacks(prep_callbacks_t *callbacks)
{
    // if (!(prolog_slurmctld_callback = callbacks->prolog_slurmctld))
    //     have_prolog_slurmctld = false;
    // if (!(epilog_slurmctld_callback = callbacks->epilog_slurmctld))
    //     have_epilog_slurmctld = false;
}

extern void prep_p_required(prep_call_type_t type, bool *required)
{
    *required = false;

    switch (type) {
    case PREP_PROLOG_SLURMCTLD:
        if (running_in_slurmctld())
            *required = false;
        break;
    case PREP_EPILOG_SLURMCTLD:
        if (running_in_slurmctld())
            *required = false;
        break;
    case PREP_PROLOG:
    case PREP_EPILOG:
        if (running_in_slurmd())
            *required = true;
        break;
    default:
        return;
    }
    return;
}

extern int prep_p_prolog_slurmctld(int rc, uint32_t job_id)
{
    debug2(DEMETER_PLUGIN_NAME "prolog Slurmctld (this shoudn't happend)");
    return (SLURM_SUCCESS);
}

extern int prep_p_epilog_slurmctld(int rc, uint32_t job_id)
{
    debug2(DEMETER_PLUGIN_NAME "epilog Slurmctld (this shoudn't happend)");
    return (SLURM_SUCCESS);
}
