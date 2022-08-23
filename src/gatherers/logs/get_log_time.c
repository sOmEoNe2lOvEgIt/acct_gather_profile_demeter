// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such log, much time!
//___________________________________________________________________________________________________________________________________________

#include <time.h>
#include <stdio.h>
#include "demeter.h"

void get_log_time(parsed_log_t *log_to_parse)
{
    FILE *uptime_file = NULL;
    char *uptime_str = NULL;
    time_t uptime_sec = 0;
    time_t log_time = 0;
    size_t uptime_str_len = 30;

    if (log_to_parse->log_source_path == NULL || strcmp(log_to_parse->log_source_path, "syslog") != 0)
        return;
    uptime_file = fopen("/proc/uptime", "r");
    if (uptime_file == NULL)
        return;
    getline(&uptime_str, &uptime_str_len, uptime_file);
    uptime_sec = (time_t)atol(uptime_str);
    fclose(uptime_file);
    log_time = (time_t)atol((log_to_parse->unparsed_log + 1));
    log_time += (time(NULL) - uptime_sec); //boot time + time since boot to log
    log_to_parse->log_time_str = strdup(asctime(localtime(&log_time)));
    free(uptime_str);
}