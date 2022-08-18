// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such cgroup, much gathering!
//___________________________________________________________________________________________________________________________________________

#include <string.h>
#include "src/slurmd/slurmd/slurmd.h"
#include "src/common/xmalloc.h"
#include "demeter.h"

//Settings for the plugin (later conf file to implement):
static const enum log_format_types format = FANCY;
static const char log_file_path[] = "/var/log/demeter.log";

void get_mem_max_usage(cgroup_data_t *cgroup_data, job_id_info_t *job_info)
{
    char res[50];
    char cgroup_path[130];
    FILE *file = NULL;

    sprintf(cgroup_path, "/sys/fs/cgroup/memory/slurm/uid_%u/job_%u/memory.max_usage_in_bytes", job_info->uid, job_info->job_id);
    file = fopen(cgroup_path, "r");
    if (file == NULL) {
        write_log_to_file(log_file_path, "Could not open cgroup file", format, 1);
        return;
    }
    write_log_to_file(log_file_path, "Getting max memory usage", format, 99);
    fgets(res, 50, file);
    cgroup_data->mem_max_usage_bytes = atoi(res);
    fclose(file);
}

void get_oom_status(cgroup_data_t *cgroup_data, job_id_info_t *job_info)
{
    char *res = NULL;
    char cgroup_path[130];
    size_t read_size = 30;
    FILE *file = NULL;

    sprintf(cgroup_path, "/sys/fs/cgroup/memory/slurm/uid_%u/job_%u/memory.oom_control", job_info->uid, job_info->job_id);
    file = fopen(cgroup_path, "r");
    if (file == NULL) {
        write_log_to_file(log_file_path, "Could not open cgroup file", format, 1);
        return;
    }
    write_log_to_file(log_file_path, "Getting oom status", format, 99);
    getline(&res, &read_size, file);
    cgroup_data->oom_kill_disable = atoi(&res[17]);
    getline(&res, &read_size, file);
    cgroup_data->under_oom = atoi(&res[10]);
    getline(&res, &read_size, file);
    cgroup_data->oom_kill = atoi(&res[9]);
    free(res);
    fclose(file);
}