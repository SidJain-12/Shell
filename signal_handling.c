#include "headers.h"

void SigintHandler(int sig_num)
{
    // CTRL-C
    signal(SIGINT, SigintHandler);
    printf("\n");
    fflush(stdout);
}

void SigstpHandler(int sig_num)
{
    // push any foreground process to background
    // and change its status to stopped

    // CTRL-Z

    int pid = getpid();
    if (pid != og_pid)
    {
        if (kill(pid, SIGTSTP) < 0)
        {
            print_red();
            perror("Error in sending signal");
            print_reset();
        }
    }

    printf("\n");
    fflush(stdout);
}

// exit shell if CTRL-D is pressed
void SigchldHandler(int sig_num)
{
    // CTRL-D
    int status;
    int pid = waitpid(-1, &status, WNOHANG);
    if (pid > 0)
    {
        if (WIFEXITED(status))
        {
            // printf("haha ded %d\n", pid);
            // printf("sleep with pid = %d exited normally\n", pid);
            for (int i = 0; i < 1000; i++)
            {
                if (bg_pid_array[i] == pid)
                {
                    bg_pid_array[i] = 0;
                    break;
                }
            }
            fflush(stdout);
        }
        else if (WIFSIGNALED(status))
        {
            // printf("sleep with pid = %d exited abnormally\n", pid);
            for (int i = 0; i < 1000; i++)
            {
                if (bg_pid_array[i] == pid)
                {
                    bg_pid_array[i] = 0;
                    break;
                }
            }
            fflush(stdout);
        }
        // else
        // {
        //     // printf("sleep with pid = %d exited abnormally\n", pid);
        // }
    }
}
