#include "headers.h"

char statfile[1000];
char stats[1024];

void read_file(pid_t pid)
{
    // printf("pid: %d\n", pid);
    char info_file[1000];
    sprintf(info_file, "/proc/%d/stat", (int)pid);
    info_file[strlen(info_file)] = '\0';

    // printf("info_file: %s\n", info_file);

    char info_array[1024] = {'\0'};

    FILE *f_statfile;
    f_statfile = fopen(info_file, "r");
    if (f_statfile == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fread(info_array, 1024, 1, f_statfile);
    fclose(f_statfile);

    strcpy(statfile, info_file);
    strcpy(stats, info_array);
}
int pinfo(int j, char **arguments)
{
    char *execPath = (char *)malloc(sizeof(char) * 1024);
    char *info_array[100] = {NULL};
    char path[1024];

    if (j > 2)
    {
        printf("pinfo: too many arguments\n");
        return 0;
    }
    pid_t pid = getpid();
    if (j == 2)
    {
        for (int i = 0; i < strlen(arguments[1]); i++)
        {
            if (arguments[1][i] < '0' || arguments[1][i] > '9')
            {
                printf("pinfo: invalid pid\n");
                return 1;
            }
        }
        pid = atoi(arguments[1]);
    }

    read_file(pid);

    info_array[0] = strtok(stats, " ");
    for (int i = 1; info_array[i - 1] != NULL; i++)
        info_array[i] = strtok(NULL, " ");

    printf("pid : %d\nprocess Status : %s%s\nmemory : %s {Virtual Memory}\n", 
    (int)pid,info_array[2], strcmp(info_array[0], info_array[7]) ? "+" : "",info_array[22]);

    sprintf(statfile, "/proc/%d/exe", (int)pid);
    
    int readInfo = readlink(statfile, execPath, 1023);
    
    strcpy(path, execPath);
    if (readInfo != -1)
    {
        if (strncmp(execPath, iwd, strlen(iwd)) == 0)
        {
            strcpy(path, "~");
            strcat(path, execPath + strlen(iwd));
        }
    }
    if(readInfo == -1)
    {
        printf("Executable Path : No access\n");
    }
    else
    {
        printf("Executable Path : %s\n", path);
    }
    free(execPath);
    return 0;
}