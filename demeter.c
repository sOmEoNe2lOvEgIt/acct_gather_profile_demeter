// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such plugin, much data!
//___________________________________________________________________________________________________________________________________________

#include <stdarg.h>
#include <unistd.h>
#include <inttypes.h>

#include "slurm/slurm.h"
#include "slurm/slurm_errno.h"
#include "src/common/prep.h"
#include "src/common/macros.h"
#include "src/common/xmalloc.h"
#include "src/common/xstring.h"
#include "src/common/parse_time.h"
#include "src/common/uid.h"

#define PLUGIN_NAME "PrEp-demeter: "
#define STRING_LEN 1024

const char plugin_name[] = "Demeter godess of data harvest";
const char plugin_type[] = "prep/demeter";
const uint32_t plugin_version = SLURM_VERSION_NUMBER;

static bool have_prolog_slurmctld = false;
static bool have_epilog_slurmctld = false;

void (*prolog_slurmctld_callback)(int rc, uint32_t job_id) = NULL;
void (*epilog_slurmctld_callback)(int rc, uint32_t job_id) = NULL;

char *prefix = "JOB_RECORD__";
char *exclude_keys_prolog = "";
char *exclude_keys_epilog = "";
static pthread_mutex_t  plugin_log_lock = PTHREAD_MUTEX_INITIALIZER;

extern int init (void)
{
    slurm_info(PLUGIN_NAME "init\n");

    return (SLURM_SUCCESS);
}

extern void fini (void)
{
    slurm_info(PLUGIN_NAME "fini\n");
}

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

extern int prep_p_prolog(job_env_t *job_env, slurm_cred_t *cred)
{
    slurm_info(PLUGIN_NAME "ProLog for job %u\n", job_env->jobid);
    return (SLURM_SUCCESS);
}

extern int prep_p_epilog(job_env_t *job_env, slurm_cred_t *cred)
{
    slurm_info(PLUGIN_NAME "EpiLog for job %u\n", job_env->jobid);
    return (SLURM_SUCCESS);
}

extern int prep_p_prolog_slurmctld(int rc, uint32_t job_id)
{
    slurm_info(PLUGIN_NAME "ProLog Slurmctld\n");
    return (SLURM_SUCCESS);
}

extern int prep_p_epilog_slurmctld(int rc, uint32_t job_id)
{
    slurm_info(PLUGIN_NAME "EpiLog Slurmctld\n");
    return (SLURM_SUCCESS);
}