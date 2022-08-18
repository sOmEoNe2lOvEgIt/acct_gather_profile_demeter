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

void get_mem_max_usage(cgroup_data_t *cgroup_data, job_id_info_t *job_info)
{
    char res[50];
    char cgroup_path[130];
    FILE *file = NULL;

    sprintf(cgroup_path, "/sys/fs/cgroup/memory/slurm/uid_%u/job_%u/", job_info->uid, job_info->job_id);
    file = fopen(cgroup_path, "r");
    if (file == NULL) {
        write_log_to_file(log_file_path, "Could not open cgroup file", format, 1);
        return;
    }
    fgets(res, 50, file);
    cgroup_data->mem_max_usage_bytes = atoi(res);
    fclose(file);
}