// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such conf, much modularity!
//___________________________________________________________________________________________________________________________________________

#include <stdio.h>
#include "demeter.h"

static demeter_conf_t *init_conf(void)
{
    demeter_conf_t *conf = malloc(sizeof(demeter_conf_t));

    if (conf == NULL)
        return (NULL);
    conf->verbose_lv = 0;
    conf->log_style = SIMPLE;
    conf->log_level = INFO;
    conf->log_file_path = strdup("/var/log/demeter.log");
    return (conf);
}

static char *get_log_file_path(char *dirty_path)
{
    char *clean_path = malloc(sizeof(char) * (strlen(dirty_path) - 1));
    int i;
    int j;

    for(i = 0; dirty_path[i] != '\0' &&
    dirty_path[i] != '\"'; i++);
    i++;
    for(j = 0; dirty_path[i] != '\0' &&
    dirty_path[i] != '\"'; i++, j++)
        clean_path[j] = dirty_path[i];
    clean_path[j] = '\0';
    free(dirty_path);
    return (clean_path);
}

demeter_conf_t *read_conf(void)
{
    demeter_conf_t *conf = init_conf();
    FILE *conf_file;
    char *line = NULL;
    size_t len = 130;
    char teststr[1000];

    if (conf == NULL)
        return (NULL);
    conf_file = fopen("/etc/slurm/demeter.conf", "r");
    if (conf_file == NULL)
        return (conf);
    while (getline(&line, &len, conf_file) != -1) {
        if (line[0] == '#')
            continue;
        if (strncmp(line, "Verbose", 7) == 0)
            conf->verbose_lv = atoi(line + 8);
        if (strncmp(line, "LogStyle", 8) == 0) {
            if (strncmp(line + 9, "FANCY", 5) == 0)
                conf->log_style = FANCY;
            if (strncmp(line + 9, "SIMPLE", 6) == 0)
                conf->log_style = SIMPLE;
            if (strncmp(line + 9, "SYSTEM", 6) == 0)
                conf->log_style = SYSTEM;
        }
        if (strncmp(line, "LogLevel", 8) == 0) {
            if (strncmp(line + 9, "DEBUG", 5) == 0)
                conf->log_level = DEBUG;
            if (strncmp(line + 9, "INFO", 4) == 0)
                conf->log_level = INFO;
            if (strncmp(line + 9, "WARNING", 7) == 0)
                conf->log_level = WARNING;
            if (strncmp(line + 9, "ERROR", 5) == 0)
                conf->log_level = ERROR;
            if (strncmp(line + 9, "FATAL", 5) == 0)
                conf->log_level = FATAL;
        }
        if (strncmp(line, "LogFilePath", 11) == 0)
            conf->log_file_path = get_log_file_path(strdup(line + 12));
    }
    sprintf(teststr, "%u,%u,%s", conf->verbose_lv, conf->log_style, conf->log_file_path);
    write_log_to_file(conf, teststr, DEBUG, 99);
    return (conf);
}
