// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such include, many things!
//___________________________________________________________________________________________________________________________________________

#include "slurm/slurm.h"
#include <inttypes.h>
#include <string.h>

#ifndef DEMETER_H_
#define DEMETER_H_

// ENUMS&STRUCTS
//___________________________________________________________________________________________________________________________________________
enum log_format_types
{
    FANCY=0,
    SIMPLE=1,
    SYSTEM=2,
};

typedef struct cgroup_data_s{
    uint mem_max_usage_bytes;
    uint oom_kill_disable;
    uint under_oom;
    uint oom_kill;
} cgroup_data_t;

// TOOLS
//___________________________________________________________________________________________________________________________________________
void my_slurm_info(char *message);
void my_slurm_error(char *message);
char *get_time_str(void);

// LOGGER FUNCTIONS
//___________________________________________________________________________________________________________________________________________

FILE *init_log_file(const char *log_file_path);
int write_log_to_file(const char *log_file_path, char *message,
enum log_format_types format, uint verbose);
void prolog_message(const char *log_file_path, uint32_t nb_jobid, enum log_format_types format);
void epilog_message(const char *log_file_path, uint32_t nb_jobid, enum log_format_types format);

#endif /* !DEMETER_H_ */
