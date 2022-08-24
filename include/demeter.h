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

typedef enum log_style_e { // Log style for demeter log output.
    FANCY=0,
    SIMPLE=1,
    SYSTEM=2,
} log_style_t;

typedef enum dem_log_level_e { // Log level for demeter log output.
    DEBUG=0,
    INFO=1,
    WARNING=2,
    ERROR=3,
    FATAL=4,
} dem_log_level_t;

typedef struct demeter_conf_s { // Demeter configuration.
    uint verbose_lv; // Verbosity level if log level is DEBUG.
    dem_log_level_t log_level; // Log level for demeter log output.
    log_style_t log_style;
    char *log_file_path; // Path to demeter log file.
    char *get_log_file_path; // Path to file in which demeter will parse log from. (to be implemented)
} demeter_conf_t;

typedef struct cgroup_data_s { // Cgroup gathered data for each job step || job.
    uint mem_max_usage_bytes; // Maximum memory usage in bytes.
    uint oom_kill_disable;
    uint under_oom;
    uint oom_kill; 
    char *cpuset_cpus;
    char *cpuset_effective_cpus;
} cgroup_data_t;

typedef struct job_id_info_s {
    uint job_id; // Id from current job when setup propperly.
    uint uid; // User id from current job when setup propperly.
    time_t start_time; // "Start time for job", actually time of
    //execution of the acct_gather_profile_p_node_step_start function in the plugin.
    // Used to verify that logs gathered are logs that are timestamped in job runtime.
} job_id_info_t;

typedef struct parsed_log_s { // Logs gathered for each job step || job.
    char *unparsed_log; // Raw log line.
    char *log_proc_name; // Name of process outputing log.
    job_id_info_t *job_id_info;
    char *log_source_path; //path to the log file, "stdout" if stdout
    char *log_time_str; // Time of log in readable format.
    //struct host_info_s *host_info; <-- future struct whith info about the host to implement eventually
    int error_code; //0 if no error, 1 if error, only used for stdout as log source
    cgroup_data_t *cgroup_data; //matching cgroup data to the log
} parsed_log_t;

typedef struct linked_list_s { // Generic linked list.
    void *data; // Needs to be casted to the appropriate type.
    struct linked_list_s *next;
} linked_list_t;

// TOOLS
//___________________________________________________________________________________________________________________________________________

void my_slurm_info(char *message); // Prints INFO message to slurm log.
void my_slurm_error(char *message); // Prints ERROR message to slurm log.
void my_slurm_debug(char *message, int level); // Prints DEBUG message to slurm log whith log level.
char *get_time_str(void); // Returns time in readable format.
bool is_log_empty(char *log); // Returns true if log is empty.
demeter_conf_t *read_conf(void); // Reads demeter configuration.
job_id_info_t *get_job_info(stepd_step_rec_t* job); // Returns job info.
linked_list_t *add_to_list(linked_list_t *list, void *data); // Adds a new link to the begining
//of the list given as arg, returns new said link.

// I'M FREE!!!
//___________________________________________________________________________________________________________________________________________

// Functions for freeing different types of structs whithin demeter.
void free_list(linked_list_t *list);
void free_conf(demeter_conf_t *conf);
void free_cgroup(cgroup_data_t *data);
void free_job_id_info(job_id_info_t *job_info);
void free_parsed_log(parsed_log_t *log);

// LOGGER FUNCTIONS
//___________________________________________________________________________________________________________________________________________

FILE *init_log_file(demeter_conf_t *conf, bool silent); // Initializes demeter log file.
int write_log_to_file(demeter_conf_t *conf, char *message, dem_log_level_t level, uint verbose); // Writes log to demeter log file.
void log_parsed_logs(linked_list_t *gathered_logs, demeter_conf_t *demeter_conf); // Specific to gathered logs.
void log_cgroup(cgroup_data_t *cgroup_data, job_id_info_t *job_info, demeter_conf_t *conf); // Specific to cgroup data.

// CGROUP FUNCTION
//___________________________________________________________________________________________________________________________________________

cgroup_data_t *gather_cgroup(job_id_info_t *job_info, demeter_conf_t *conf); // Gathers cgroup data.

// CGROUP TOOLS
//___________________________________________________________________________________________________________________________________________

void get_oom_status(cgroup_data_t *cgroup_data, job_id_info_t *job_info, demeter_conf_t *conf);
void get_mem_max_usage(cgroup_data_t *cgroup_data, job_id_info_t *job_info, demeter_conf_t *conf);
void get_cpuset(cgroup_data_t *cgroup_data, job_id_info_t *job_info, demeter_conf_t *conf);

// LOG_PARSER FUNCTION
//___________________________________________________________________________________________________________________________________________

linked_list_t *gather_logs(demeter_conf_t *demeter_conf, job_id_info_t *job_info, cgroup_data_t *cgroup_data); // Gathers logs.

// LOG_PARSER TOOLS
//___________________________________________________________________________________________________________________________________________

char *read_sys_logs(void); //Returns logs from kernel logs.
parsed_log_t *init_parsed_log(void) ; // Returns a new propperly allocated empty parsed log struct.
void free_logs(linked_list_t *log_list); // Frees all logs in a list. (does not free the list itself)
int get_log_time(parsed_log_t *log_to_parse, time_t start_time); // Adds readable time to log. Returns 0 if no error,
// other if error (including the log time not being job runtime).
linked_list_t *gather_kernel_logs (demeter_conf_t *demeter_conf, job_id_info_t *job_info, cgroup_data_t *cgroup_data, linked_list_t *log_list);


#endif /* !DEMETER_H_ */
