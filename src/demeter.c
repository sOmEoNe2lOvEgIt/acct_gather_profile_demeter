// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such plugin, much data!

#include "config.h"
#include "src/slurmctld/slurmctld.h"
#include "src/common/slurm_xlator.h"
#include <slurm/slurm.h>
#include <string.h>
#include <stdio.h>

const char plugin_name[] = "The godess of harvesting data.";
const char plugin_type[] = "prep/demeter";
const uint32_t plugin_version = SLURM_VERSION_NUMBER;

int init (void)
{
    return (SLURM_SUCCESS);
}

int prep_p_prolog(job_env_t *job_env, slurm_cred_t *cred)
{
    printf("%s\n", "ProLog");
    return (SLURM_SUCCESS);
}

int prep_p_epilog(job_env_t *job_env, slurm_cred_t *cred)
{
    printf("%s\n", "Epilog");
    return (SLURM_SUCCESS);
}

void fini (void)
{
}