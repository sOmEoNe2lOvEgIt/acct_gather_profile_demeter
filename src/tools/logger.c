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

// LOGGING TOOLS
//___________________________________________________________________________________________________________________________________________
FILE *init_log_file(const char *log_file_path, bool silent)
{
    FILE *log_file;

    log_file = fopen(log_file_path, "a");
    if (log_file == NULL) {
        if (!silent)
            my_slurm_debug("error: can't open log file. Will try to open log file at \"/tmp/demeter.log\".", 2);
        log_file = fopen("/tmp/demeter.log", "a");
    }
    //substitut log file if chosen one is not writable (although this one may not be writable either)
    if (log_file == NULL) {
        my_slurm_error("FATAL: can't open log file at /tmp/demeter.log either. Exiting.");
        return (NULL);
    }
    return (log_file);
}

//LOGGING FUNCTIONS
//___________________________________________________________________________________________________________________________________________ 
int write_log_to_file(const char *log_file_path, char *message,
enum log_format_types format, uint verbose)
{
    FILE *log_file;

    log_file=init_log_file(log_file_path, true);
    if (log_file == NULL) {
        my_slurm_debug("error : can't write to log file, log file is NULL.", 2);
        return (1);
    }
    //different styles of logs
    switch (format)
    {
        case FANCY:
            fprintf(log_file, "[%s]:[acct_gather_profile/demeter]> %s\n", get_time_str(), message);
            break;
        case SIMPLE:
            fprintf(log_file, "%s	| acct_gather_profile/demeter: %s\n", get_time_str(), message);
            break;
        case SYSTEM:
            fprintf(log_file, "acct_gather_profile/demeter: %s\n", message);
            break;
        default:
            my_slurm_debug("error : invalid log format.", 2);
            fclose(log_file);
            return (1);
    }
    fclose(log_file);
    return (0);
}
