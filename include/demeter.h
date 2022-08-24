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

typedef enum log_style_e {
    FANCY=0,
    SIMPLE=1,
    SYSTEM=2,
} log_style_t;

typedef enum dem_log_level_e {
    DEBUG=0,
    INFO=1,
    WARNING=2,
    ERROR=3,
    FATAL=4,
} dem_log_level_t;

typedef struct demeter_conf_s {
    uint verbose_lv;
    dem_log_level_t log_level;
    log_style_t log_style;
    char *log_file_path;
    char *get_log_file_path;
} demeter_conf_t;

typedef struct cgroup_data_s {
    uint mem_max_usage_bytes;
    uint oom_kill_disable;
    uint under_oom;
    uint oom_kill;
    char *cpuset_cpus;
    char *cpuset_effective_cpus;
} cgroup_data_t;

typedef struct job_id_info_s {
    uint job_id;
    uint uid;
    time_t start_time;
} job_id_info_t;

// TO IMPLEMENT
typedef struct parsed_log_s {
    char *unparsed_log;
    char *log_proc_name;
    job_id_info_t *job_id_info;
    char *log_source_path; //path to the log file, "stdout" if stdout
    char *log_time_str;
    //struct host_info_s *host_info; <-- future struct whith info about the host to implement eventually
    int error_code; //0 if no error, 1 if error, only used for stdout as log source
    cgroup_data_t *cgroup_data; //matching cgroup data to the log
} parsed_log_t;

typedef struct linked_list_s {
    void *data;
    struct linked_list_s *next;
} linked_list_t;

// TOOLS
//___________________________________________________________________________________________________________________________________________

void my_slurm_info(char *message);
void my_slurm_error(char *message);
void my_slurm_debug(char *message, int level);
char *get_time_str(void);
bool is_log_empty(char *log);
demeter_conf_t *read_conf(void);
job_id_info_t *get_job_info(stepd_step_rec_t* job);
linked_list_t *add_to_list(linked_list_t *list, void *data);

// I'M FREE!!!
//___________________________________________________________________________________________________________________________________________

void free_list(linked_list_t *list);
void free_conf(demeter_conf_t *conf);
void free_cgroup(cgroup_data_t *data);
void free_job_id_info(job_id_info_t *job_info);
void free_parsed_log(parsed_log_t *log);

// LOGGER FUNCTIONS
//___________________________________________________________________________________________________________________________________________

FILE *init_log_file(demeter_conf_t *conf, bool silent);
int write_log_to_file(demeter_conf_t *conf, char *message, dem_log_level_t level, uint verbose);

// CGROUP FUNCTIONS
//___________________________________________________________________________________________________________________________________________

cgroup_data_t *gather_cgroup(job_id_info_t *job_info, demeter_conf_t *conf);
void log_cgroup(cgroup_data_t *cgroup_data, job_id_info_t *job_info, demeter_conf_t *conf);

// CGROUP TOOLS
//___________________________________________________________________________________________________________________________________________

void get_oom_status(cgroup_data_t *cgroup_data, job_id_info_t *job_info, demeter_conf_t *conf);
void get_mem_max_usage(cgroup_data_t *cgroup_data, job_id_info_t *job_info, demeter_conf_t *conf);
void get_cpuset(cgroup_data_t *cgroup_data, job_id_info_t *job_info, demeter_conf_t *conf);

// LOG PARSER
//___________________________________________________________________________________________________________________________________________

linked_list_t *gather_logs(demeter_conf_t *demeter_conf, job_id_info_t *job_info, cgroup_data_t *cgroup_data);
void free_logs(linked_list_t *log_list);
void log_parsed_logs(linked_list_t *gathered_logs, demeter_conf_t *demeter_conf);

// LOG PARSER TOOLS
//___________________________________________________________________________________________________________________________________________

char *read_sys_logs(void);
int get_log_time(parsed_log_t *log_to_parse, time_t start_time);


#endif /* !DEMETER_H_ */
