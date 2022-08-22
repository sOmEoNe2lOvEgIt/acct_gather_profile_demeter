// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, much job, many info!
//___________________________________________________________________________________________________________________________________________

#include <stdio.h>
#include "slurm/slurm.h"
#include "demeter.h"

job_id_info_t *get_job_info(stepd_step_rec_t* job)
{
    job_id_info_t *job_info = malloc(sizeof(job_id_info_t));

    if (job_info == NULL) {
        my_slurm_error("malloc failed");
        return (NULL);
    }
    job_info->job_id = job->array_job_id;
    job_info->uid = job->uid;
    return (job_info);
}
