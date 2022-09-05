// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such include, many things!
//___________________________________________________________________________________________________________________________________________

#ifndef PREP_DEMETER_H_
#define PREP_DEMETER_H_

// TOOLS
//___________________________________________________________________________________________________________________________________________

void my_slurm_info(char *message); // Prints INFO message to slurm log.
void my_slurm_error(char *message); // Prints ERROR message to slurm log.
void my_slurm_debug(char *message, int level); // Prints DEBUG message to slurm log whith log level.

#endif /* !DEMETER_H_ */
