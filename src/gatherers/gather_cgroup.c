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

static void get_mem_max_usage(cgroup_data_t *cgroup_data, char *cgroup_path)
{
    char res[50];
    FILE *file = NULL;

    file = fopen(cgroup_path, "r");
    if (file == NULL) {
        write_log_to_file(log_file_path, "Could not open cgroup file", format, 1);
        return;
    }
    fgets(res, 50, file);
    cgroup_data->mem_max_usage_bytes = atoi(res);
    fclose(file);
}

cgroup_data_t *gather_cgroup(char *cgroup_path)
{
    cgroup_data_t *cgroup_data;

    cgroup_data = alloc_cgroup_struct();
    if (cgroup_data == NULL)
        return (NULL);
    get_mem_max_usage(cgroup_data, cgroup_path);
    return (cgroup_data);
}

char *get_cgroup_path(stepd_step_rec_t* job)
{
    char cgroup_path[160];

    sprintf(cgroup_path, "/sys/fs/cgroup/memory/slurm/uid_%u/job_%u/memory.max_usage_in_bytes", job->uid, job->array_job_id);
    return (strdup(cgroup_path));
}