#include "headers.h"

void run_commands(int j, char **arguments)
{

    if (isRedirection(j, arguments))
    {
        io_redirection(&j, arguments);
    }

    if (strcmp(arguments[0], "exit") == 0)
    {
        save_history();
        exit(0);
    }
    
    else if (strcmp(arguments[0], "cd") == 0)
        cd(arguments[0]);
    else if (strcmp(arguments[0], "pwd") == 0)
        pwd();
    else if (strcmp(arguments[0], "echo") == 0)
        echo(j, arguments);
    else if (strcmp(arguments[0], "ls") == 0)
        ls_command(j, arguments);
    else if (strcmp(arguments[0], "history") == 0)
    {
        call_history(j, arguments);
    }
    else if (strcmp(arguments[0], "pinfo") == 0)
        pinfo(j, arguments);
    else if (strcmp(arguments[0], "discover") == 0)
        discover_command(j, arguments);
    else if(strcmp(arguments[0],"jobs")==0)
        print_jobs(j,arguments);
    else if(strcmp(arguments[0],"bg")==0)
        bg(j,arguments);
    else if(strcmp(arguments[0],"fg")==0)
        fg(j,arguments);
    else if(strcmp(arguments[0],"sig")==0)
        sig(j,arguments);
    else
        foreground_process(j, arguments);
}