// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such logs, many info!
//___________________________________________________________________________________________________________________________________________

#include <stdio.h>
#include <syslog.h>
#include "slurm/slurm.h"
#include "demeter.h"

static parsed_log_t *init_parsed_log(void)
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

linked_list_t *gather_logs(demeter_conf_t *demeter_conf, job_id_info_t *job_info, cgroup_data_t *cgroup_data)
{
    static char *log_buffer = NULL;
    parsed_log_t *curr_log = NULL;
    linked_list_t *log_list = NULL;
    static int len = 0;

    log_buffer = read_sys_logs();
    if (log_buffer == NULL)
        return (NULL);  
    log_list = add_to_list(log_list, init_parsed_log());
    for (int i = 0; log_buffer[i] != '\0'; i++) {
        curr_log = (parsed_log_t *)log_list->data;
        curr_log->cgroup_data = cgroup_data;
        curr_log->job_id_info = job_info;
        for (len = 0; log_buffer[i + len] != '\0' && log_buffer[i + len] != '\n'; len++);
        if (len == 0)
            continue;
        curr_log->unparsed_log = strndup(log_buffer + i, len);
        i += len - 1;
        if (curr_log->unparsed_log == NULL || is_log_empty(curr_log->unparsed_log)) {
            write_log_to_file(demeter_conf, "Caught empty log line", DEBUG, 2);
            continue;
        }
        curr_log->log_source_path = strdup("syslog");
        if (get_log_time(curr_log, job_info->start_time) != 0) {
            write_log_to_file(demeter_conf, "Cannot get log time/log is not written at runtime", DEBUG, 99);
            if (curr_log->unparsed_log != NULL)
                free(curr_log->unparsed_log);
            if (curr_log->log_source_path != NULL)
                free(curr_log->log_source_path);
            continue;
        }
        log_list = add_to_list(log_list, init_parsed_log());
    }
    if (log_buffer != NULL)
        free(log_buffer);
    return (log_list);
}

void free_logs(linked_list_t *log_list)
{
    parsed_log_t *curr_log = NULL;
    linked_list_t *next_list = NULL;

    while (log_list != NULL) {
        next_list = log_list->next;
        curr_log = (parsed_log_t *)log_list->data;
        free_parsed_log(curr_log);
        log_list = next_list;
    }
}
