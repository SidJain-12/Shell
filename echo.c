#include "headers.h"

void echo(int j, char **args)
{
    for (int i = 1; i < j; i++)
    {
        printf("%s ", args[i]);
    }
    printf("\n");
}
