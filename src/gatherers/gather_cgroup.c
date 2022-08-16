// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such cgroup, much gathering!
//___________________________________________________________________________________________________________________________________________

#include "demeter.h"
#include "src/slurmd/slurmd/slurmd.h"

static cgroup_data_t *alloc_cgroup_struct(void)
{
    cgroup_data_t *cgroup_data = malloc(sizeof(cgroup_data_t));

    if (cgroup_data == NULL)
    {
        my_slurm_error("malloc failed");
        return (NULL);
    }
    cgroup_data->mem_max_usage_bytes = 0;
    cgroup_data->oom_kill_disable = 0;
    cgroup_data->under_oom = 0;
    cgroup_data->oom_kill = 0;
    return (cgroup_data);
}

static void get_mem_max_usage(cgroup_data_t *cgroup_data, uint uid, uint jobid)
{
    char cgroup_path[160];
    char res[50];
    FILE *file;

    sprintf(cgroup_path, "/sys/fs/cgroup/memory/slurm/uid_%u/job_%u/memory.max_usage_in_bytes", uid, jobid);
    my_slurm_debug(cgroup_path, 3);
    file = fopen(cgroup_path, "r"); // <--- This never works because the file is gone at this point... IDK what to do.
    if (fopen(cgroup_path, "r") == NULL)
    {
        my_slurm_error("fopen failed");
        return;
    }
    fgets(res, 50, file);
    my_slurm_debug(res, 3);
    cgroup_data->mem_max_usage_bytes = atoi(res);
    fclose(file);
}

cgroup_data_t *gather_cgroup(uint uid, uint jobid)
{
    cgroup_data_t *cgroup_data = alloc_cgroup_struct();

    if (cgroup_data == NULL)
        return (NULL);
    get_mem_max_usage(cgroup_data, uid, jobid);
    return (cgroup_data);
}