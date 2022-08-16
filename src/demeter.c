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
#include "demeter.h"

// GLOBAL VARIABLES
//___________________________________________________________________________________________________________________________________________
//setting required variables for slurm:
#define PLUGIN_NAME "acct_gather_profile/demeter: "
const char plugin_name[] = "Demeter godess of data harvest.";
const char plugin_type[] = "acct_gather_profile/demeter";
const uint32_t plugin_version = SLURM_VERSION_NUMBER;
//Settings for the plugin (later conf file to implement):
const enum log_format_types format = FANCY;
const char log_file_path[] = "/var/log/demeter.log";

// PLUGIN INITIALIZATION AND EXIT FUNCTIONS
//___________________________________________________________________________________________________________________________________________
extern int init (void)
{
	FILE *log_file;

    debug(PLUGIN_NAME "starting");
	log_file = init_log_file(log_file_path, false);
	//check if log file is writable:
	if (log_file == NULL)
		return (SLURM_ERROR);
	fclose(log_file);
	write_log_to_file(log_file_path, "[Demeter started]", format, 1);
	debug(PLUGIN_NAME "started, thank god!");
    return (SLURM_SUCCESS);
}

extern void fini (void)
{
    debug(PLUGIN_NAME "stopping");
	write_log_to_file(log_file_path, "[Demeter stopped]", format, 1);
	debug(PLUGIN_NAME "stopped");
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

extern int acct_gather_profile_p_node_step_start(stepd_step_rec_t* job)
{
	my_slurm_debug("acct_gather_profile_p_node_step_start", 3);
	return (SLURM_SUCCESS);
}

extern int acct_gather_profile_p_node_step_end(stepd_step_rec_t* job)
{
	my_slurm_debug("acct_gather_profile_p_node_step_end", 3);
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

//___________________________________________________________________________________________________________________________________________ 
//___________________________________________________________________________________________________________________________________________
//___________________________________________________________________________________________________________________________________________
// OLD PREP
//___________________________________________________________________________________________________________________________________________
//___________________________________________________________________________________________________________________________________________
//___________________________________________________________________________________________________________________________________________ 

// // UNUSED FUNCTIONS
// //___________________________________________________________________________________________________________________________________________
// extern int prep_p_prolog_slurmctld(int rc, uint32_t job_id)
// {
//     my_slurm_debug("prolog Slurmctld (this shoudn't happend)", 2);
//     return (SLURM_SUCCESS);
// }

// extern int prep_p_epilog_slurmctld(int rc, uint32_t job_id)
// {
//     my_slurm_debug("epilog Slurmctld (this shoudn't happend)", 2);
//     return (SLURM_SUCCESS);
// }

// // CALLED BEFORE JOB
// //___________________________________________________________________________________________________________________________________________
// extern int prep_p_prolog(job_env_t *job_env, slurm_cred_t *cred)
// {
//     debug2(PLUGIN_NAME "prolog for job %u", job_env->jobid);
// 	prolog_message(log_file_path, job_env->jobid, format);
//     return (SLURM_SUCCESS);
// }

// // CALLED AFTER JOB
// //___________________________________________________________________________________________________________________________________________
// extern int prep_p_epilog(job_env_t *job_env, slurm_cred_t *cred)
// {
// 	cgroup_data_t *cgroup_data;

//     debug2(PLUGIN_NAME "epilog for job %u", job_env->jobid);
// 	epilog_message(log_file_path, job_env->jobid, format);
// 	cgroup_data = gather_cgroup(job_env);
// 	free(cgroup_data); // <--- Avoid data leak but later in the code after (here for now)
//     return (SLURM_SUCCESS);
// }

// // REQUIRED FUNCTIONS
// //___________________________________________________________________________________________________________________________________________
// extern void prep_p_register_callbacks(prep_callbacks_t *callbacks)
// {
//     if (!(prolog_slurmctld_callback = callbacks->prolog_slurmctld))
// 		have_prolog_slurmctld = false;
// 	if (!(epilog_slurmctld_callback = callbacks->epilog_slurmctld))
// 		have_epilog_slurmctld = false;
// }

// extern void prep_p_required(prep_call_type_t type, bool *required)
// {
// 	*required = false;

// 	switch (type) {
// 	case PREP_PROLOG_SLURMCTLD:
// 		if (running_in_slurmctld())
// 			*required = false;
// 		break;
// 	case PREP_EPILOG_SLURMCTLD:
// 		if (running_in_slurmctld())
// 			*required = false;
// 		break;
// 	case PREP_PROLOG:
// 	case PREP_EPILOG:
// 		if (running_in_slurmd())
// 			*required = true;
// 		break;
// 	default:
// 		return;
// 	}
// 	return;
// }