// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such logs, many info!
//___________________________________________________________________________________________________________________________________________

#include <stdio.h>
#include <syslog.h>
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

static void process_unparsed_log(parsed_log_t *log_to_parse)
{
    get_log_time(log_to_parse);
    //TODO
}

static void gather_sys_logs(linked_list_t *log_list, job_id_info_t *job_info, cgroup_data_t *cgroup_data)
{
    char *log_buffer = read_sys_logs();
    parsed_log_t *curr_log = (parsed_log_t *)log_list->data;
    int len = 0;

    if (log_buffer == NULL)
        return;
    for (int i = 0; log_buffer[i] != '\0'; i++) {
        for (len = 0; log_buffer[i + len] != '\0' && log_buffer[i + len] != '\n'; len++);
        if (len == 0)
            continue;
        curr_log->unparsed_log = strndup(log_buffer + i, len);
        curr_log->log_source_path = strdup("syslog");
        process_unparsed_log(curr_log);
        log_list = add_to_list(log_list, init_parsed_log());
        curr_log = (parsed_log_t *)log_list->data;
        curr_log->cgroup_data = cgroup_data;
        curr_log->job_id_info = job_info;
        i += len;
    }
    free(log_buffer);
}

void free_logs(linked_list_t *log_list)
{
    parsed_log_t *curr_log = (parsed_log_t *)log_list->data;
    linked_list_t *next_list = NULL;

    while (log_list != NULL) {
        next_list = log_list->next;
        curr_log = (parsed_log_t *)log_list->data;
        free_parsed_log(curr_log);
        log_list = next_list;
    }
}

linked_list_t *gather_logs(demeter_conf_t *demeter_conf, job_id_info_t *job_info, cgroup_data_t *cgroup_data)
{
    linked_list_t *log_list = NULL;
    parsed_log_t *parsed_log = NULL;

    parsed_log = init_parsed_log();
    if (parsed_log == NULL)
        return (NULL);
    parsed_log->job_id_info = job_info;
    parsed_log->cgroup_data = cgroup_data;
    log_list = add_to_list(log_list, parsed_log);
    gather_sys_logs(log_list, job_info, cgroup_data);
    return (log_list);
}