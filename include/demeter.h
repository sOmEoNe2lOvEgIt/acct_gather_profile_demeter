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

// TOOLS
//___________________________________________________________________________________________________________________________________________
void my_slurm_info(char *message);
char *get_time_str(void);

// LOGGER FUNCTIONS
//___________________________________________________________________________________________________________________________________________

FILE *init_log_file(const char *log_file_path);
int write_log_to_file(const char *log_file_path, char *message);

#endif /* !DEMETER_H_ */
