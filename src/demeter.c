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
#include "demeter.h"

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
//Settings for the plugin (later conf file to implement):
const enum log_format_types format = SIMPLE;
const char log_file_path[] = "/var/log/demeter.log";

// CALLED BEFORE JOB
//___________________________________________________________________________________________________________________________________________
extern int prep_p_prolog(job_env_t *job_env, slurm_cred_t *cred)
{
    slurm_info(PLUGIN_NAME "prolog for job %u\n", job_env->jobid);
	prolog_message(log_file_path, job_env->jobid, format);
    return (SLURM_SUCCESS);
}

// CALLED AFTER JOB
//___________________________________________________________________________________________________________________________________________
extern int prep_p_epilog(job_env_t *job_env, slurm_cred_t *cred)
{
    slurm_info(PLUGIN_NAME "epilog for job %u\n", job_env->jobid);
	epilog_message(log_file_path, job_env->jobid, format);
    return (SLURM_SUCCESS);
}

// REQUIRED FUNCTIONS
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
		if (running_in_slurmd())
			*required = true;
		break;
	case PREP_EPILOG:
		if (running_in_slurmd())
			*required = true;
		break;
	default:
		return;
	}
	return;
}

// PLUGIN INITIALIZATION AND EXIT FUNCTIONS
//___________________________________________________________________________________________________________________________________________
extern int init (void)
{
	FILE *log_file;

    slurm_info(PLUGIN_NAME "starting");
	log_file = init_log_file(log_file_path, false);
	//check if log file is writable:
	if (log_file == NULL)
		return (SLURM_ERROR);
	fclose(log_file);
	write_log_to_file(log_file_path, "[Demeter started]", format, 1);
	slurm_info(PLUGIN_NAME "started, thank god!");
    return (SLURM_SUCCESS);
}

extern void fini (void)
{
    slurm_info(PLUGIN_NAME "stopping");
	write_log_to_file(log_file_path, "[Demeter stopped]", format, 1);
	slurm_info(PLUGIN_NAME "stopped");
}

// TOOLS
//___________________________________________________________________________________________________________________________________________
void my_slurm_info(char *message)
{
	slurm_info(PLUGIN_NAME "%s", message);
}

void my_slurm_error(char *message)
{
	slurm_perror(message);
}

// UNUSED FUNCTIONS
//___________________________________________________________________________________________________________________________________________
extern int prep_p_prolog_slurmctld(int rc, uint32_t job_id)
{
    slurm_info(PLUGIN_NAME "prolog Slurmctld");
    return (SLURM_SUCCESS);
}

extern int prep_p_epilog_slurmctld(int rc, uint32_t job_id)
{
    slurm_info(PLUGIN_NAME "epilog Slurmctld");
    return (SLURM_SUCCESS);
}
