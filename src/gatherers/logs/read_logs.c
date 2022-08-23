// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such logs, many buffer!
//___________________________________________________________________________________________________________________________________________

#include <sys/klog.h>
#include <string.h>

char *read_sys_logs(void)
{
    char log_buffer[4096];

    if (klogctl(3, log_buffer, 4096) < 0)
        return (NULL);
    return (strdup(log_buffer));
}