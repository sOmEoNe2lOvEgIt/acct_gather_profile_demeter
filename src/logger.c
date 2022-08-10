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

FILE *init_log_file(const char *log_file_path, bool silent)
{
    FILE *log_file;

    log_file = fopen(log_file_path, "a");
    if (log_file == NULL) {
        if (!silent)
            my_slurm_info ("error: can't open log file. Will try to open log file at \"/tmp/demeter.log\".");
        log_file = fopen("/tmp/demeter.log", "a");
    }
    if (log_file == NULL) {
        my_slurm_error ("FATAL: can't open log file at /tmp/demeter.log either. Exiting.");
        return (NULL);
    }
    return (log_file);
}

int write_log_to_file(const char *log_file_path, char *message,
enum log_format_types format, uint verbose)
{
    FILE *log_file;

    log_file=init_log_file(log_file_path, true);
    if (log_file == NULL) {
        my_slurm_info ("error : can't write to log file, log file is NULL.");
        return (1);
    }
    switch (format)
    {
        case FANCY:
            fprintf(log_file, "[%s]:[prep_demeter]> %s\n", get_time_str(), message);
            break;
        case SIMPLE:
            fprintf(log_file, "%s	| prep_demeter: %s\n", get_time_str(), message);
            break;
        case SYSTEM:
            fprintf(log_file, "prep_demeter: %s\n", message);
            break;
        default:
            my_slurm_info ("error : invalid log format.");
            fclose(log_file);
            return (1);
    }
    fclose(log_file);
    return (0);
}

void prolog_message(const char *log_file_path, uint32_t nb_jobid, enum log_format_types format)
{
    char message[50];

	sprintf(message, "epilog for job with id %u", nb_jobid);
    write_log_to_file(log_file_path, message, format, 0);
}

void epilog_message(const char *log_file_path, uint32_t nb_jobid, enum log_format_types format)
{
    char message[50];

	sprintf(message, "epilog for job with id %u", nb_jobid);
    write_log_to_file(log_file_path, message, format, 0);
}