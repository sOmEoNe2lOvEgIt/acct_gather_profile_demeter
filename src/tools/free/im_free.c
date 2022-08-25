// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, much var, such free!
//___________________________________________________________________________________________________________________________________________

#include <stdio.h>
#include "demeter.h"

void free_list(linked_list_t *list)
{
    linked_list_t *tmp = list;
    linked_list_t *prev = NULL;

    while (tmp != NULL) {
        prev = tmp;
        tmp = tmp->next;
        if (prev->data != NULL)
            free(prev->data);
        free(prev);
    }
}

void free_conf(demeter_conf_t *conf)
{
    if (conf == NULL)
        return;
    if (conf->slurm_log_path != NULL)
        free(conf->slurm_log_path);
    if (conf->log_file_path != NULL)
        free(conf->log_file_path);
    free(conf);
    conf = NULL;
}

void free_cgroup(cgroup_data_t *data)
{
    if (data == NULL)
        return;
    if (data->cpuset_cpus != NULL)
        free(data->cpuset_cpus);
    if (data->cpuset_effective_cpus != NULL)
        free(data->cpuset_effective_cpus);
    free(data);
    data = NULL;
}

void free_job_id_info(job_id_info_t *job_info)
{
    if (job_info == NULL)
        return;
    free(job_info);
    job_info = NULL;
}

void free_parsed_log(parsed_log_t *log)
{
    if (log == NULL)
        return;
    if (!is_log_empty(log->unparsed_log))
        free(log->unparsed_log);
    if (log->log_proc_name != NULL)
        free(log->log_proc_name);
    if (log->log_source_path != NULL)
        free(log->log_source_path);
    if (log->log_time_str != NULL)
        free(log->log_time_str);
    free(log);
}
