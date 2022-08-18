// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such include, many things!
//___________________________________________________________________________________________________________________________________________

#include "slurm/slurm.h"
#include "src/slurmd/slurmd/slurmd.h"
#include "src/slurmd/slurmstepd/slurmstepd_job.h"
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

typedef struct job_id_info_s{
    uint job_id;
    uint uid;
} job_id_info_t;

typedef struct job_env_sort_s{
    int id;
} job_env_sort_t;

// TOOLS
//___________________________________________________________________________________________________________________________________________

void my_slurm_info(char *message);
void my_slurm_error(char *message);
void my_slurm_debug(char *message, int level);
char *get_time_str(void);

// LOGGER FUNCTIONS
//___________________________________________________________________________________________________________________________________________

FILE *init_log_file(const char *log_file_path, bool silent);
int write_log_to_file(const char *log_file_path, char *message,
enum log_format_types format, uint verbose);
void prolog_message(const char *log_file_path, uint32_t nb_jobid, enum log_format_types format);
void epilog_message(const char *log_file_path, uint32_t nb_jobid, enum log_format_types format);

// CGROUP FUNCTIONS
//___________________________________________________________________________________________________________________________________________

cgroup_data_t *gather_cgroup(job_id_info_t *job_info);
job_id_info_t *get_job_info(stepd_step_rec_t* job);

void get_mem_max_usage(cgroup_data_t *cgroup_data, job_id_info_t *job_info);
void get_oom_status(cgroup_data_t *cgroup_data, job_id_info_t *job_info);

#endif /* !DEMETER_H_ */
