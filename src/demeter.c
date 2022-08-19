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
#include "src/common/slurm_acct_gather_profile.h"
#include "src/common/cgroup.h"
#include "src/common/xmalloc.h"
#include "demeter.h"

// GLOBAL VARIABLES
//___________________________________________________________________________________________________________________________________________
//setting required variables for slurm:
#define PLUGIN_NAME "acct_gather_profile/demeter: "
const char plugin_name[] = "Demeter godess of data harvest.";
const char plugin_type[] = "acct_gather_profile/demeter";
const uint32_t plugin_version = SLURM_VERSION_NUMBER;
//Data made/needed for the plugin:
static job_id_info_t *job_info = NULL;
static cgroup_data_t *cgroup_data = NULL;
static demeter_conf_t *demeter_conf = NULL;

// PLUGIN INITIALIZATION AND EXIT FUNCTIONS
//___________________________________________________________________________________________________________________________________________
extern int init (void)
{
	FILE *log_file = NULL;

    my_slurm_debug("starting",1);
	demeter_conf = read_conf();
	log_file = init_log_file(demeter_conf, false);
	my_slurm_debug("log file initialized", 2);
	//check if log file is writable:
	if (log_file == NULL)
		return (SLURM_ERROR);
	fclose(log_file);
	write_log_to_file(demeter_conf, "demeter started", INFO, 0);
	my_slurm_debug("started, thank god!", 1);
    return (SLURM_SUCCESS);
}

extern void fini (void)
{
    my_slurm_debug("stopping", 1);
	if (job_info != NULL) {
		free(job_info);
	}
	if (cgroup_data != NULL) {
		free(cgroup_data);
	}
	write_log_to_file(demeter_conf, "demeter stopped", INFO, 0);
	if (demeter_conf != NULL) {
		if (demeter_conf->log_file_path != NULL) {
			free(demeter_conf->log_file_path);
		}
		free(demeter_conf);
	}
	my_slurm_debug("stopped", 1);
}

// USED
//___________________________________________________________________________________________________________________________________________
extern int acct_gather_profile_p_node_step_start(stepd_step_rec_t* job)
{
	write_log_to_file(demeter_conf, "getting cgroup file path", DEBUG, 3);
	if (job == NULL)
		return (SLURM_ERROR);
	job_info = get_job_info(job);
	return (SLURM_SUCCESS);
}

extern int acct_gather_profile_p_node_step_end(stepd_step_rec_t* job)
{
	write_log_to_file(demeter_conf, "call to gather_cgroup", DEBUG, 3);
	if (job_info != NULL)
		cgroup_data = gather_cgroup(job_info, demeter_conf);
	return (SLURM_SUCCESS);
}

// REQUIRED
//___________________________________________________________________________________________________________________________________________
extern int acct_gather_profile_g_conf_options(s_p_options_t **full_options, int *full_options_cnt)
{
	my_slurm_debug("acct_gather_profile_g_conf_options", 3);
	return (SLURM_SUCCESS);
}

extern int acct_gather_profile_g_conf_set(s_p_hashtbl_t *tbl)
{
	my_slurm_debug("acct_gather_profile_g_conf_set", 3);
	return (SLURM_SUCCESS);
}

extern int acct_gather_profile_p_task_start(stepd_step_rec_t* job, uint32_t taskid)
{
	my_slurm_debug("acct_gather_profile_p_task_start", 3);
	return (SLURM_SUCCESS);
}

extern int acct_gather_profile_p_task_end(stepd_step_rec_t* job, pid_t taskpid)
{
	my_slurm_debug("acct_gather_profile_p_task_end", 3);
	return (SLURM_SUCCESS);
}

extern int acct_gather_profile_p_add_sample_data(uint32_t type, void* data)
{
	my_slurm_debug("acct_gather_profile_p_add_sample_data", 3);
	return (SLURM_SUCCESS);
}

extern int acct_gather_profile_p_child_forked(pid_t child_pid)
{
	my_slurm_debug("acct_gather_profile_p_child_forked", 3);
	return (SLURM_SUCCESS);
}

extern int acct_gather_profile_p_conf_options(s_p_options_t **full_options, int *full_options_cnt)
{
	my_slurm_debug("acct_gather_profile_p_conf_options", 3);
	return (SLURM_SUCCESS);
}

extern int acct_gather_profile_p_conf_set(s_p_hashtbl_t *tbl)
{
	my_slurm_debug("acct_gather_profile_p_conf_set", 3);
	return (SLURM_SUCCESS);
}

extern int acct_gather_profile_p_get(acct_gather_profile_type_t *profile)
{
	my_slurm_debug("acct_gather_profile_p_get", 3);
	return (SLURM_SUCCESS);
}

extern int acct_gather_profile_p_create_group(acct_gather_profile_type_t *profile, uint32_t group_id)
{
	my_slurm_debug("acct_gather_profile_p_create_group", 3);
	return (SLURM_SUCCESS);
}

extern int acct_gather_profile_p_create_dataset(acct_gather_profile_type_t *profile, uint32_t group_id, uint32_t dataset_id)
{
	my_slurm_debug("acct_gather_profile_p_create_dataset", 3);
	return (SLURM_SUCCESS);
}

extern int acct_gather_profile_p_conf_values(s_p_hashtbl_t *tbl)
{
	my_slurm_debug("acct_gather_profile_p_conf_values", 3);
	return (SLURM_SUCCESS);
}

extern int acct_gather_profile_p_is_active(void)
{
	my_slurm_debug("acct_gather_profile_p_is_active", 3);
	return (SLURM_SUCCESS);
}

// TOOLS
//___________________________________________________________________________________________________________________________________________
void my_slurm_info(char *message)
{
	slurm_info(PLUGIN_NAME "%s", message);
}

void my_slurm_debug(char *message, int level)
{
	switch (level)
	{
	case 1:
		debug(PLUGIN_NAME "%s", message);
		break;
	case 2:
		debug2(PLUGIN_NAME "%s", message);
		break;
	case 3:
		debug3(PLUGIN_NAME "%s", message);
		break;
	case 4:
		debug4(PLUGIN_NAME "%s", message);
		break;
	case 5:
		debug5(PLUGIN_NAME "%s", message);
		break;
	default:
		break;
	}
}

void my_slurm_error(char *message)
{
	slurm_perror(message);
}
