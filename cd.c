#include "headers.h"

void cd(char *token)
{
    token = strtok(NULL, " \t");
    if (strtok(NULL, " \t") != NULL)
    {
        printf("cd: too many arguments\n");
        return;
    }
    if (token == NULL || strcmp(token, "~") == 0)
    {
        chdir(iwd);
        strcpy(prewd, cwd);
        strcpy(cwd, iwd);
    }
    else if (strcmp(token, "-") == 0)
    {
        chdir(prewd);
        strcpy(prewd, cwd);
        strcpy(cwd, prewd);
    }
    else
    {
        if (chdir(token) == -1)
        {
            printf("cd: %s: No such file or directory\n", token);
        }
        else
        {
            strcpy(prewd, cwd);
            strcpy(cwd, token);
        }
    }
}