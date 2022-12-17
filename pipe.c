#include "headers.h"

int isPipe(int j, char **arguments)
{
    for (int i = 0; i < j; i++)
    {
        if (arguments[i][0] == '|')
        {
            return 1;
        }
    }
    return 0;
}

void piping(char *command)
{

    int saved_stdout;
    int saved_stdin;
    saved_stdout = dup(STDOUT_FILENO); // saved_stdout refers to the STDOUT
    saved_stdin = dup(STDIN_FILENO);
    int max_size = 500;

    char tempCommand[max_size];
    strcpy(tempCommand, command);
    char arrayCommands[max_size][max_size];

    int itr = 0;
    char s[2];
    s[0] = '|';
    s[1] = '\0';
    char *token;
    token = strtok(tempCommand, s);

    while (token != NULL)
    {
        strcpy(arrayCommands[itr], token);
        itr++;
        token = strtok(NULL, s);
    }
    int numCommands = itr;

    int pipesArray[numCommands][2];

    for (int i = 0; i < numCommands; i++)
    {
        int x = pipe(pipesArray[i]);
        if (x == -1)
        {
            perror(RED "Piping unsuccessful \n");
            return;
        }
    }

    for (int i = 0; i < numCommands - 1; i++)
    {
        if (i == 0)
        {
            dup2(pipesArray[i][1], 1); // fd = 1 refers to the write end of pipe[i]
            close(pipesArray[i][1]);

            char *arguments[1024];
            char *word;
            int j = 0;
            char *t2 = strdup(arrayCommands[i]);
            while ((word = strtok_r(t2, " \t", &t2)))
            {
                arguments[j] = word;
                j++;
            }

            run_commands(j, arguments);
            dup2(saved_stdout, 1);
        }
        else
        {
            dup2(pipesArray[i][1], 1);     // fd = 1 refers to the write end of the pipe
            dup2(pipesArray[i - 1][0], 0); // fd = 0 refers to the read end of the pipe
            close(pipesArray[i][1]);
            close(pipesArray[i - 1][0]);
            char *arguments[1024];
            char *word;
            int j = 0;
            char *t2 = strdup(arrayCommands[i]);
            while ((word = strtok_r(t2, " \t", &t2)))
            {
                arguments[j] = word;
                j++;
            }

            run_commands(j, arguments);
            dup2(saved_stdin, 0);
            dup2(saved_stdout, 1);
        }
    }
    // For last command
    dup2(saved_stdout, 1);
    dup2(pipesArray[numCommands - 2][0], 0);
    close(pipesArray[numCommands - 2][0]);

    char *arguments[1024];
    char *word;
    int j = 0;
    char *t2 = strdup(arrayCommands[numCommands - 1]);
    while ((word = strtok_r(t2, " \t", &t2)))
    {
        arguments[j] = word;
        j++;
    }

    run_commands(j, arguments);
    dup2(saved_stdin, 0);
}