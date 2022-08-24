// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such logs, many info!
//___________________________________________________________________________________________________________________________________________

#include <stdio.h>
#include <syslog.h>
#include "slurm/slurm.h"
#include "demeter.h"

parsed_log_t *init_parsed_log(void)
{
    parsed_log_t *parsed_log = malloc(sizeof(parsed_log_t));

    parsed_log->unparsed_log = NULL;
    parsed_log->log_proc_name = NULL;
    parsed_log->job_id_info = NULL;
    parsed_log->log_source_path = NULL;
    parsed_log->log_time_str = NULL;
    parsed_log->cgroup_data = NULL;
    parsed_log->error_code = 0;
    return (parsed_log);
}

void free_logs(linked_list_t *log_list)
{
    parsed_log_t *curr_log = NULL;
    linked_list_t *next_list = NULL;

    if (log_list == NULL || log_list->data == NULL)
        return;
    while (log_list != NULL) {
        next_list = log_list->next;
        if ((parsed_log_t *)log_list->data != NULL) {
            curr_log = (parsed_log_t *)log_list->data;
            free_parsed_log(curr_log);
        }
        free(log_list);
        log_list = next_list;
    }
}

linked_list_t *gather_logs(demeter_conf_t *demeter_conf, job_id_info_t *job_info, cgroup_data_t *cgroup_data)
{
    linked_list_t *log_list = NULL;

    log_list = gather_kernel_logs(demeter_conf, job_info, cgroup_data, log_list);
    return (log_list);
}