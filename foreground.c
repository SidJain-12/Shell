#include "headers.h"

// int pid_count = 0;
void foreground_process(int j,char **args)
{
    args[j] = NULL;
   
    time_t start, end;
    time(&start);
    pid_t pid = fork();
    if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        if (execvp(args[0], args) < 0)
        {
            printf("Error: Command not found\n");
            exit(0);
        }
    }
    else
    {
        int status;
        waitpid(pid, &status, WUNTRACED | WCONTINUED);
        time(&end);
        // print if process terminated normally or not for a particular pid
        
        if(WIFSTOPPED(status))
        {
            // pid_count++;
            for(int i=0;i<1000;i++)
            {
                if(bg_pid_array[i]==0)
                {
                   bg_pid_array[i] = pid;
                   break;
                }
            }
        }
        
        double time_taken = end - start;
        tt += time_taken;
    }
    
}

