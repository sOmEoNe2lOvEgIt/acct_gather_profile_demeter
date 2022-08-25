// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such conf, much modularity!
//___________________________________________________________________________________________________________________________________________

#include <stdio.h>
#include "demeter.h"

static char *get_hostname(void)
{
    char *hostname = NULL;
    size_t hostname_len = 50;
    FILE *hostname_file = NULL;

    hostname_file = fopen("/etc/hostname", "r");
    if (hostname_file == NULL)
        return (NULL);
    getline(&hostname, &hostname_len, hostname_file);
    fclose(hostname_file);
    for (hostname_len = 0; hostname[hostname_len] != '\0'; hostname_len++) {
        if (hostname[hostname_len] == '\n') {
            hostname[hostname_len] = '\0';
            break;
        }
    }
    return (hostname);
}

static demeter_conf_t *init_conf(void)
{
    demeter_conf_t *conf = malloc(sizeof(demeter_conf_t));
    char *hostname = NULL;
    char slurm_log_path[80];

    if (conf == NULL)
        return (NULL);
    hostname = get_hostname();
    if (hostname == NULL) {
        free(conf);
        return (NULL);
    }
    sprintf(slurm_log_path, "/var/log/slurm/slurm.%s.log", hostname);
    conf->verbose_lv = 0;
    conf->log_style = SIMPLE;
    conf->log_level = INFO;
    conf->log_file_path = strdup("/var/log/demeter.log");
    conf->slurm_log_path = strdup(slurm_log_path);
    free (hostname);
    return (conf);
}

static char *get_log_file_path(char *dirty_path)
{
    char *clean_path = malloc(sizeof(char) * (strlen(dirty_path) - 1));
    int i;
    int j;

    if (clean_path == NULL)
        return (NULL);
    for (i = 0; dirty_path[i] != '\"'; i++) {
        if (dirty_path[i] == '\0') {
            free(clean_path);
            return (NULL);
        }
    }
    i++;
    for (j = 0; dirty_path[i] != '\0' && dirty_path[i] != '\"'; i++, j++)
        clean_path[j] = dirty_path[i];
    clean_path[j] = '\0';
    free(dirty_path);
    return (clean_path);
}

static dem_log_level_t get_slurm_log_level(char *str_level)
{
    if (strncmp(str_level, "DEBUG", 5) == 0)
        return (DEBUG);
    if (strncmp(str_level, "INFO", 4) == 0)
        return (INFO);
    if (strncmp(str_level, "WARNING", 7) == 0)
        return (WARNING);
    if (strncmp(str_level, "ERROR", 5) == 0)
        return (ERROR);
    if (strncmp(str_level, "FATAL", 5) == 0)
        return (FATAL);
    return (INFO);
}

demeter_conf_t *read_conf(void)
{
    demeter_conf_t *conf = init_conf();
    FILE *conf_file;
    char *line = NULL;
    size_t len = 130;
    char teststr[1000];
    char *log_file_path;
    char *slurm_log_path;

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
        if (strncmp(line, "LogLevel", 8) == 0)
            conf->log_level = get_slurm_log_level(line + 9);
        if (strncmp(line, "SlurmLogLevel", 13) == 0)
            conf->slurm_log_level = get_slurm_log_level(line + 14);
        if (strncmp(line, "SysLogLevel", 11) == 0) {
            conf->sys_log_level = get_slurm_log_level(line + 12);
        if (strncmp(line, "LogFilePath", 11) == 0) {
            log_file_path = get_log_file_path(strdup(line + 12));
            if (log_file_path != NULL)
                conf->log_file_path = log_file_path;
        }
        if (strncmp(line, "SlurmLogPath", 12) == 0) {
            slurm_log_path = get_log_file_path(strdup(line + 13));
            if (slurm_log_path != NULL)
                conf->slurm_log_path = slurm_log_path;
        }
    }
    sprintf(teststr, "%u,%u,%s", conf->verbose_lv, conf->log_style, conf->log_file_path);
    write_log_to_file(conf, teststr, DEBUG, 99);
    return (conf);
}
