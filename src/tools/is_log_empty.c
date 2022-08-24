// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such logs, much test!
//___________________________________________________________________________________________________________________________________________

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool is_log_empty(char *log)
{
    if (log == NULL)
        return (true);
    if (strlen(log) == 0)
        return (true);
    if (strcmp(log, "\n") == 0)
        return (true);
    return (false);
}
