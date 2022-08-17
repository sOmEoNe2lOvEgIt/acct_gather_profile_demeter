// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such cgroup, much gathering!
//___________________________________________________________________________________________________________________________________________

#include "demeter.h"
#include "src/slurmd/slurmd/slurmd.h"

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

static void get_mem_max_usage(cgroup_data_t *cgroup_data, uid_t uid, uint32_t jobid)
{
    char cgroup_path[160];
    char res[50];
    FILE *file = NULL;

    sprintf(cgroup_path, "/sys/fs/cgroup/memory/slurm/uid_%u/job_%u/memory.max_usage_in_bytes", uid, jobid);
    write_log_to_file(log_file_path, "Opening cgroup file.", format, 1);
    write_log_to_file(log_file_path, cgroup_path, format, 1);
    file = fopen(cgroup_path, "r"); // <--- This never works because the file is gone at this point... IDK what to do.
    if (file == NULL)
    {
        write_log_to_file(log_file_path, "Could not open cgroup file", format, 0);
        return;
    }
    write_log_to_file(log_file_path, "Opened cgroup file!!!", format, 0);
    fgets(res, 50, file);
    cgroup_data->mem_max_usage_bytes = atoi(res);
    write_log_to_file(log_file_path, "mem_max_usage_bytes: " + cgroup_data->mem_max_usage_bytes, format, 0);
    fclose(file);
}

cgroup_data_t *gather_cgroup(stepd_step_rec_t* job)
{
    cgroup_data_t *cgroup_data = alloc_cgroup_struct();
    // char max_mem_str[50];

    if (cgroup_data == NULL) {
        write_log_to_file(log_file_path, "alloc_cgroup_struct failed", format, 0);
        return (NULL);
    }
    if (job == NULL) {
        write_log_to_file(log_file_path, "job is NULL, nothing to gather", format, 0);
        return (NULL);
    }
    get_mem_max_usage(cgroup_data, job->uid, job->array_job_id);
    return (cgroup_data);
}