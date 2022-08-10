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

// TOOLS
//___________________________________________________________________________________________________________________________________________
void my_slurm_info(char *message);
char *get_time_str(void);

// LOGGER FUNCTIONS
//___________________________________________________________________________________________________________________________________________

FILE *init_log_file(const char *log_file_path);
int write_log_to_file(const char *log_file_path, char *message);
void name_date(const char *log_file_path, enum log_format_types format);
void prolog_message(const char *log_file_path, uint32_t nb_jobid);
void epilog_message(const char *log_file_path, uint32_t nb_jobid);

#endif /* !DEMETER_H_ */
