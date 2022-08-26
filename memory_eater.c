#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main (int ac, char **av)
{
    if (ac != 2) {
        printf("usage: %s SizeOfMemInBytes\n", av[0]);
        return (1);
    }
    long size = atol(av[1]);
    char *mem = malloc(sizeof(char) * size);
    if (mem == NULL) {
        printf("error: can't allocate %ld bytes of memory\n", size);
        return (1);
    }
    free(mem);
    return (0);
}