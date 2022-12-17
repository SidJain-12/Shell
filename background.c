#include "headers.h"

int pid_count = 0;
int bg_pid_array[1000] = {0};

void background_process(int j, char **args)
{
    args[j] = NULL;
    pid_t pid;
    pid = fork();
    int status;

    // create child process
    if (pid < 0)
    {
        // Error forking
        perror("Error forking");
        exit(1);
    }

    else if (pid != 0)
    {
        printf("\n%d\n", pid);

        for (int i = 0; i < 1000; i++)
        {
            if (bg_pid_array[i] == 0)
            {
                bg_pid_array[i] = pid;
                break;
            }
        }

        if (WIFEXITED(status))
        {
            // printf("haha ded %d\n", pid);
            printf("sleep with pid = %d exited normally\n", pid);
        }
        else
        {
            printf("sleep with pid = %d exited abnormally\n", pid);
        }
    }
    else
    {
        // Child process
        if (execvp(args[0], args) < 0)
        {
            printf("Error: Command not found\n");
            exit(0);
        }
    }
}

// cat < 1.txt > file1.txt