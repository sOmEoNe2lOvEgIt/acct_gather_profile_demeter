// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such logs, good slurm!
//___________________________________________________________________________________________________________________________________________

#include <stdio.h>
#include <syslog.h>
#include "slurm/slurm.h"
#include "demeter.h"

static bool handle_log_time(parsed_log_t *curr_log, demeter_conf_t *demeter_conf, time_t start_time)
{
    if (get_slurm_log_time(curr_log, start_time) != 0) {
        write_log_to_file(demeter_conf, "Cannot get log time/log is not written at runtime", DEBUG, 99);
        if (curr_log->unparsed_log != NULL) {
            free(curr_log->unparsed_log);
            curr_log->unparsed_log = NULL;
        }
        if (curr_log->log_source_path != NULL) {
            free(curr_log->log_source_path);
            curr_log->log_source_path = NULL;
        }
        return false;
    }
    return true;
}

static bool handle_log_level(parsed_log_t *curr_log, demeter_conf_t *demeter_conf)
{
    dem_log_level_t level_of_curr_log = DEBUG;

    if (curr_log->unparsed_log == NULL)
        return false;
    if (strstr(curr_log->unparsed_log, "FATAL") != NULL || strstr(curr_log->unparsed_log, "fatal") != NULL)
        level_of_curr_log = FATAL;
    if (strstr(curr_log->unparsed_log, "ERROR") != NULL || strstr(curr_log->unparsed_log, "error") != NULL)
        level_of_curr_log = ERROR;
    if (strstr(curr_log->unparsed_log, "WARNING") != NULL || strstr(curr_log->unparsed_log, "warning") != NULL)
        level_of_curr_log = WARNING;
    if (strstr(curr_log->unparsed_log, "INFO") != NULL || strstr(curr_log->unparsed_log, "info") != NULL)
        level_of_curr_log = INFO;
    if (strstr(curr_log->unparsed_log, "DEBUG") != NULL || strstr(curr_log->unparsed_log, "debug") != NULL)
        level_of_curr_log = DEBUG;
    if (level_of_curr_log < demeter_conf->slurm_log_level) {
        if (curr_log->unparsed_log != NULL) {
            free(curr_log->unparsed_log);
            curr_log->unparsed_log = NULL;
        }
        if (curr_log->log_source_path != NULL) {
            free(curr_log->log_source_path);
            curr_log->log_source_path = NULL;
        }
        return false;
    }
    return true;
}

linked_list_t *gather_slurm_logs
(demeter_conf_t *demeter_conf, job_id_info_t *job_info,cgroup_data_t *cgroup_data, linked_list_t *log_list)
{
    FILE *log_file = NULL;
    parsed_log_t *curr_log = NULL;
    char *buffer = NULL;
    size_t len = 1000;

    if ((log_file = fopen(demeter_conf->slurm_log_path, "r")) == NULL) {
        write_log_to_file(demeter_conf, "Cannot open slurm log file", DEBUG, 3);
        write_log_to_file(demeter_conf, demeter_conf->slurm_log_path, DEBUG, 99);
        return (NULL);
    }
    log_list = add_to_list(log_list, init_parsed_log());
    while (getline(&buffer, &len, log_file) != -1) {
        curr_log = (parsed_log_t *)log_list->data;
        curr_log->unparsed_log = strdup(buffer);
        curr_log->log_source_path = strdup("slurm_log_path");
        if (!handle_log_level(curr_log, demeter_conf))
            continue;
        if (!handle_log_time(curr_log, demeter_conf, job_info->start_time))
            continue;
        curr_log->log_proc_name = strdup("slurm");
        log_list = add_to_list(log_list, init_parsed_log());
    }
    fclose(log_file);
    return (log_list);
}