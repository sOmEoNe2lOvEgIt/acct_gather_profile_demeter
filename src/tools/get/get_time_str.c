// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such plugin, much data!
//___________________________________________________________________________________________________________________________________________

#include <time.h>
#include <string.h>

char *get_time_str(void)
{
    char *time_str;
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    time_str = asctime(timeinfo);
    time_str[strlen(time_str) - 1] = '\0';
    return (time_str);
}