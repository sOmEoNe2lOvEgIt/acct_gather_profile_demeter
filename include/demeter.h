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
typedef enum log_style_e
{
    FANCY=0,
    SIMPLE=1,
    SYSTEM=2,
} log_style_t;

typedef enum dem_log_level_e
{
    DEBUG=0,
    INFO=1,
    WARNING=2,
    ERROR=3,
    FATAL=4,
} dem_log_level_t;

typedef struct demeter_conf_s{
    uint verbose_lv;
    dem_log_level_t log_level;
    log_style_t log_style;
    char *log_file_path;
} demeter_conf_t;

typedef struct cgroup_data_s{
    uint mem_max_usage_bytes;
    uint oom_kill_disable;
    uint under_oom;
    uint oom_kill;
    char *cpuset_cpus;
    char *cpuset_effective_cpus;
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
demeter_conf_t *read_conf(void);

// LOGGER FUNCTIONS
//___________________________________________________________________________________________________________________________________________

FILE *init_log_file(demeter_conf_t *conf, bool silent);
int write_log_to_file(demeter_conf_t *conf, char *message, dem_log_level_t level, uint verbose);

// CGROUP FUNCTIONS
//___________________________________________________________________________________________________________________________________________

cgroup_data_t *gather_cgroup(job_id_info_t *job_info, demeter_conf_t *conf);
void log_cgroup(cgroup_data_t *cgroup_data, job_id_info_t *job_info, demeter_conf_t *conf);
job_id_info_t *get_job_info(stepd_step_rec_t* job);

void get_oom_status(cgroup_data_t *cgroup_data, job_id_info_t *job_info, demeter_conf_t *conf);
void get_mem_max_usage(cgroup_data_t *cgroup_data, job_id_info_t *job_info, demeter_conf_t *conf);
void get_cpuset(cgroup_data_t *cgroup_data, job_id_info_t *job_info, demeter_conf_t *conf);

#endif /* !DEMETER_H_ */
