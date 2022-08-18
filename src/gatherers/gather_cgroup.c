// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such cgroup, much gathering!
//___________________________________________________________________________________________________________________________________________

#include <string.h>
#include "src/slurmd/slurmd/slurmd.h"
#include "demeter.h"

//Settings for the plugin (later conf file to implement):
static const enum log_format_types format = FANCY;
static const char log_file_path[] = "/var/log/demeter.log";

static cgroup_data_t *alloc_cgroup_struct(void)
{
    cgroup_data_t *cgroup_data = malloc(sizeof(cgroup_data_t));

    if (cgroup_data == NULL)
    {
        my_slurm_error("malloc failed");
        return (NULL);
    }
    cgroup_data->mem_max_usage_bytes = 0;
    cgroup_data->oom_kill_disable = 0;
    cgroup_data->under_oom = 0;
    cgroup_data->oom_kill = 0;
    return (cgroup_data);
}

job_id_info_t *get_job_info(stepd_step_rec_t* job)
{
    job_id_info_t *job_info = malloc(sizeof(job_id_info_t));

    if (job_info == NULL)
    {
        my_slurm_error("malloc failed");
        return (NULL);
    }
    job_info->job_id = job->array_job_id;
    job_info->uid = job->uid;
    return (job_info);
}

cgroup_data_t *gather_cgroup(job_id_info_t *job_info)
{
    cgroup_data_t *cgroup_data = NULL;

    cgroup_data = alloc_cgroup_struct();
    if (cgroup_data == NULL)
        return (NULL);
    get_mem_max_usage(cgroup_data, job_info);
    get_oom_status(cgroup_data, job_info);
    write_log_to_file(log_file_path, "Gathered cgroup data", format, 3);
    return (cgroup_data);
}