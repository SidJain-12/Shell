#include "headers.h"

void discover_command(int j, char **arguments)
{
    char *target_dir = ".";
    char *file = NULL;
    int op_d = 0;
    int op_f = 0;

    for (int i = 1; i < j; i++)
    {
        if (strcmp(arguments[i], "-d") == 0)
            op_d = 1;
        else if (strcmp(arguments[i], "-f") == 0)
            op_f = 1;
        else if (strcmp(arguments[i], "-fd") == 0 || strcmp(arguments[i], "-df") == 0)
        {
            op_d = 1;
            op_f = 1;
        }
        else if (arguments[i][0] == '"')
        {
            file = arguments[i] + 1;
            file[strlen(file) - 1] = '\0';
        }
        else
        {
            target_dir = arguments[i];
            if(strcmp(target_dir,"~")==0)
            {
                target_dir = iwd;
            }
        }
    }
    if(strcmp(getcwd(NULL,0),iwd)==0)
    {
        target_dir=".";
    }
    if (!file && !op_f)
    {
        printf("%s\n", target_dir);
    }

    discover(target_dir, op_d, op_f, file);
}

void discover(char *dir, int op_d, int op_f, char *file)
{
    // Here we will list the directory
    // if(strcmp(dir,iwd)==0)
    // {
    //     dir=".";
    // }
    DIR *dh = opendir(dir);
    struct dirent *d;

    while ((d = readdir(dh)) != NULL)
    {
        if (file && strcmp(d->d_name, file) == 0)
        {
            if ((op_d - op_f) == 0 || (op_d && d->d_type == DT_DIR) || (op_f && d->d_type != DT_DIR))
            {
                printf("%s/%s\n", dir, d->d_name);
            }
        }
        else if (!file)
        {
            if (d->d_name[0] == '.')
                continue;
            if ((op_d - op_f) == 0 || (op_d && d->d_type == DT_DIR) || (op_f && d->d_type != DT_DIR))
            {
                printf("%s/%s\n", dir, d->d_name);
                if (d->d_type == DT_DIR)
                {
                    char *new_dir = (char *)malloc(sizeof(char) * 1024);
                    strcpy(new_dir, dir);
                    strcat(new_dir, "/");
                    strcat(new_dir, d->d_name);
                    discover(new_dir, op_d, op_f, file);
                }
            }
        }
    }
    closedir(dh);
}