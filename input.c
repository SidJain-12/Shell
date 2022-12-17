#include "headers.h"

void get_tokens(char *command, char ***fore, char ***back, int num[])
{
    char *command_copy = strdup(command);
    int num_commands = 0;
    for (int i = 0; i < strlen(command); i++)
    {
        if (command[i] == ';')
        {
            num_commands++;
        }
    }

    char **back_commands = (char **)malloc(sizeof(char *) * (num_commands + 1));

    int w = 0;
    int x = 0;
    int len = 0;

    char *sptr1, *sptr2;
    char **fore_commands = (char **)malloc((num_commands + 1) * sizeof(char *));
    char *token = strtok_r(command, "\n;", &sptr1);
    char *subtoken;
    while (token != NULL)
    {
        // fore_commands[w] = token;
        subtoken = strtok_r(token, "&", &sptr2);
        while (subtoken != NULL)
        {
            len += strlen(subtoken);
            if (len < strlen(command_copy) && command_copy[len] == '&')
            {
                back_commands[x] = subtoken;
                x++;
            }
            else
            {
                fore_commands[w] = subtoken;
                w++;
            }
            subtoken = strtok_r(NULL, "&", &sptr2);
            len++;
        }
        token = strtok_r(NULL, "\n;", &sptr1);
    }

    *fore = fore_commands;
    *back = back_commands;
    num[0] = w;
    num[1] = x;
}

void pipe_tokenise(char *command, char ***commands, int *num_commands)
{
    char *command_copy = strdup(command);
    int num = 0;
    for (int i = 0; i < strlen(command); i++)
    {
        if (command[i] == '|')
        {
            num++;
        }
    }
    char **command_tokens = (char **)malloc(sizeof(char *) * (num + 1));
    int w = 0;
    char *token = strtok(command, "|");
    while (token != NULL)
    {
        command_tokens[w] = token;
        token = strtok(NULL, "|");
        w++;
    }
    *commands = command_tokens;
    *num_commands = num;
}
