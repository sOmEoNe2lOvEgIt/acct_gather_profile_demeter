// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such logs, much file!
//___________________________________________________________________________________________________________________________________________

#include <stdio.h>
#include <inttypes.h>
#include "slurm/slurm.h"
#include "slurm/slurm_errno.h"
#include "src/common/log.h"
#include "demeter.h"

FILE *init_log_file(const char *log_file_path)
{
    FILE *log_file;

    log_file = fopen(log_file_path, "a");
    if (log_file == NULL) {
        my_slurm_info ("[ERROR]:[can't open log file at default log file path.]");
        my_slurm_info ("[Will try to open log file at /tmp/demeter.log]");
        log_file = fopen("/tmp/demeter.log", "a");
    }
    if (log_file == NULL) {
        my_slurm_info ("[FATAL]:[can't open log file at /tmp/demeter.log either.]");
        return (NULL);
    }
    return (log_file);
}

int write_log_to_file(const char *log_file_path, char *message)
{
    FILE *log_file;

    log_file=init_log_file(log_file_path);
    if (log_file == NULL) {
        my_slurm_info ("[ERROR]:[can't write to log file, log file is NULL.]");
        return (1);
    }
    fprintf(log_file, "%s", message);
    fclose(log_file);
    return (0);
}

void name_date(const char *log_file_path, enum log_format_types format)
{
    switch (format)
    {
    case FANCY:
        write_log_to_file(log_file_path, "[prep_demeter]:[");
        write_log_to_file(log_file_path, get_time_str());
        write_log_to_file(log_file_path, "]>");
        break;
    default:
        write_log_to_file(log_file_path, "prep_demeter>");
        break;
    }
}

void prolog_message(const char *log_file_path, uint32_t nb_jobid)
{
    char str_jobid[20];

	sprintf(str_jobid, "%u", nb_jobid);
    name_date(log_file_path, FANCY);
    write_log_to_file(log_file_path, "[Prolog for job with id ");
	write_log_to_file(log_file_path, str_jobid);
	write_log_to_file(log_file_path, "]\n");
}

void epilog_message(const char *log_file_path, uint32_t nb_jobid)
{
    char str_jobid[20];

	sprintf(str_jobid, "%u", nb_jobid);
    name_date(log_file_path, FANCY);
	write_log_to_file(log_file_path, "[Epilog for job with id ");
	write_log_to_file(log_file_path, str_jobid);
	write_log_to_file(log_file_path, "]\n");
}