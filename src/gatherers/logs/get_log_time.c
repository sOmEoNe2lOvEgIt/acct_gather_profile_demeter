// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such log, much time!
//___________________________________________________________________________________________________________________________________________

#include <time.h>
#include <stdio.h>
#include "demeter.h"

int get_log_time(parsed_log_t *log_to_parse, time_t start_time)
{
    FILE *uptime_file = NULL;
    char *uptime_str = NULL;
    int len_before_timestamp;
    time_t uptime = 0;
    time_t log_time = 0;
    size_t uptime_str_len = 30;

    if (log_to_parse->log_source_path == NULL || strcmp(log_to_parse->log_source_path, "syslog") != 0)
        return(1);
    for (len_before_timestamp = 0; log_to_parse->unparsed_log[len_before_timestamp] != '\0' && log_to_parse->unparsed_log[len_before_timestamp] != '['; len_before_timestamp++);
    if (log_to_parse->unparsed_log[len_before_timestamp] == '\0')
        return(1);
    len_before_timestamp ++;
    uptime_file = fopen("/proc/uptime", "r");
    if (uptime_file == NULL)
        return(2);
    getline(&uptime_str, &uptime_str_len, uptime_file);
    fclose(uptime_file);
    uptime = (time_t)atol(uptime_str);
    log_time = (time_t)atol((log_to_parse->unparsed_log + len_before_timestamp));
    log_time += (time(NULL) - uptime); //boot time + time since boot to log
    if (log_time < start_time) {
        free (uptime_str);
        return(3);
    }
    log_to_parse->log_time_str = strdup(asctime(localtime(&log_time)));    
    free(uptime_str);
    return (0);
}