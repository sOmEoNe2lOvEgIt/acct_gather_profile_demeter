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
#define PLUGIN_NAME "prep/demeter: "
const char plugin_name[] = "Demeter godess of data harvest.";
const char plugin_type[] = "prep/demeter";
const uint32_t plugin_version = SLURM_VERSION_NUMBER;
//Disable the plugin for controller:
static bool have_prolog_slurmctld = false;
static bool have_epilog_slurmctld = false;
void (*prolog_slurmctld_callback)(int rc, uint32_t job_id) = NULL;
void (*epilog_slurmctld_callback)(int rc, uint32_t job_id) = NULL;
//Data used by the plugin.
static job_id_info_t *job_info = NULL;
static cgroup_data_t *cgroup_data = NULL;
static linked_list_t *gathered_logs = NULL;
static linked_list_t *gathered_sel = NULL;
static perf_data_t *gathered_perf_data = NULL;
static demeter_conf_t *demeter_conf = NULL;

// PLUGIN INITIALIZATION AND EXIT FUNCTIONS
//___________________________________________________________________________________________________________________________________________
extern int init (void)
{
	FILE *log_file = NULL;

    debug(PLUGIN_NAME "starting");
	demeter_conf = read_conf();
	log_file = init_log_file(demeter_conf, false);
	//check if log file is writable:
	if (log_file == NULL || demeter_conf == NULL)
		return (SLURM_ERROR);
	debug2(PLUGIN_NAME "log file initialized");
	fclose(log_file);
	write_log_to_file(demeter_conf, "demeter started", INFO, 0);
	info(PLUGIN_NAME "started, thank god!");
    return (SLURM_SUCCESS);
}

extern void fini (void)
{
    debug(PLUGIN_NAME "stopping");
	free_log_list(gathered_logs);
	free_sel_list(gathered_sel);
	free_job_id_info(job_info);
	free_cgroup(cgroup_data);
	free_perf_count(gathered_perf_data);
	write_log_to_file(demeter_conf, "demeter stopped", INFO, 0);
	if (demeter_conf != NULL) {
		if (demeter_conf->log_file_path != NULL)
			free(demeter_conf->log_file_path);
		free(demeter_conf);
	}
	info(PLUGIN_NAME "stopped");
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
	write_log_to_file(demeter_conf, "call to gather_cgroup", DEBUG, 3);
	cgroup_data = gather_cgroup(job_info, demeter_conf);
	write_log_to_file(demeter_conf, "call to gather_logs", DEBUG, 3);
	gathered_logs = gather_logs(demeter_conf, job_info, cgroup_data);
	write_log_to_file(demeter_conf, "call to gather_sel", DEBUG, 3);
	gathered_sel = gather_sel(demeter_conf, job_info, cgroup_data);
	// log_cgroup(cgroup_data, job_info, demeter_conf);
	log_parsed_logs(gathered_logs, demeter_conf);
	log_parsed_sel(gathered_sel, demeter_conf);
	return (SLURM_SUCCESS);
}

// REQUIRED
//___________________________________________________________________________________________________________________________________________

extern void prep_p_register_callbacks(prep_callbacks_t *callbacks)
{
    if (!(prolog_slurmctld_callback = callbacks->prolog_slurmctld))
		have_prolog_slurmctld = false;
	if (!(epilog_slurmctld_callback = callbacks->epilog_slurmctld))
		have_epilog_slurmctld = false;
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
    debug2(PLUGIN_NAME "prolog Slurmctld (this shoudn't happend)");
    return (SLURM_SUCCESS);
}

extern int prep_p_epilog_slurmctld(int rc, uint32_t job_id)
{
    debug2(PLUGIN_NAME "epilog Slurmctld (this shoudn't happend)");
    return (SLURM_SUCCESS);
}
