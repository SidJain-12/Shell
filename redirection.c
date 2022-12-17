#include "headers.h"

int isRedirection(int j, char **arguments)
{
    for (int i = 0; i < j; i++)
    {
        if (arguments[i][0] == '<' || arguments[i][0] == '>')
        {
            return 1;
        }
    }
    return 0;
}

void io_redirection(int *j,char ** arguments )
{
    int input = 0, inp=0;
    int output_overwrite = 0, out_o=0;
    int output_append = 0, out_a=0;

    for(int i=0;i<*j;i++)
    {
        if(strcmp(arguments[i],"<")==0)
        {
            input = 1;
            inp = i;
        }
        else if(strcmp(arguments[i],">")==0)
        {
            output_overwrite = 1;
            out_o = i;
        }
        else if(strcmp(arguments[i],">>")==0)
        {
            output_append = 1;
            out_a = i;
        }
    }

    if(input)
    {
        int fd = open(arguments[inp+1],O_RDONLY,0);

        if(fd<0)
        {
            perror("Error in opening input file");
            exit(1);
        }
        dup2(fd,0);
        close(fd);
    }

    if(output_overwrite == 1 || output_append == 1)
    {
        int fd=0;
        if(output_overwrite)
        {
            fd = open(arguments[out_o+1],O_WRONLY | O_CREAT | O_TRUNC, 0644);
        }
        else
        {
            fd = open(arguments[out_a+1],O_WRONLY | O_CREAT | O_APPEND, 0644);
        }
        if(fd<0)
        {
            perror("Error in opening output file");
            exit(1);
        }
        dup2(fd,1);
        close(fd);
    }

    int i = 0;

    while (i < *j - 1)
    {
        if (arguments[i][0] == '<' || arguments[i][0] == '>')
        {
            int a = 2;
            while (a--)
            {
                for (int k = i; k < *j - 1; k++)
                {
                    arguments[k] = arguments[k + 1];
                }
                arguments[*j - 1] = NULL;
                *j = *j - 1;
            }
        }
        else
        {
            i++;
        }
    }
    
}