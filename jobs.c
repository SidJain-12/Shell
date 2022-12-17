#include "headers.h"

char jstatfile[1000];
char jstats[1024];

// char stopped_process[1000][1000];
// int stopped_process_count = 0;
// char running_process[1000][1000];
// int running_process_count = 0;
// char all_jobs[1000][1000];
// int all_jobs_count = 0;

struct Job
{
    int pid;
    char name[1000];
    char status;
};

struct Job jobs[1000];
struct Job jobs_copy[1000];
int jobs_count = 0;

void jread_file(pid_t pid)
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

    strcpy(jstatfile, info_file);
    strcpy(jstats, info_array);
}

void jpinfo(pid_t pid, int flag)
{
    char *execPath = (char *)malloc(sizeof(char) * 1024);
    char *info_array[100] = {NULL};
    char path[1024];

    jread_file(pid);

    info_array[0] = strtok(jstats, " ");
    for (int i = 1; info_array[i - 1] != NULL; i++)
        info_array[i] = strtok(NULL, " ");

    struct Job *job = (struct Job *)malloc(sizeof(struct Job));
    job->pid = pid;
    strcpy(job->name, info_array[1]);
    job->status = info_array[2][0];

    if (flag == 1)
    {
        if (job->status == 'T')
        {
            jobs[jobs_count] = *job;
            jobs_count++;
        }
    }
    else if (flag == 0)
    {
        if (job->status != 'T')
        {
            jobs[jobs_count] = *job;
            jobs_count++;
        }
    }
    else
    {
        jobs[jobs_count] = *job;
        jobs_count++;
    }
  
    free(execPath);
    // return 0;
}

int cmpfunc1(const void *a, const void *b)
{
    return strcmp(((struct Job *)a)->name, ((struct Job *)b)->name);
}

void sort_jobs()
{
    qsort(jobs_copy, jobs_count, sizeof(struct Job), cmpfunc1);
}

void print_jobs(int j, char **arguments)
{
    jobs_count = 0;

    int flag = -1; // all
    if (j == 2)
    {
        if (strcmp(arguments[1], "-r") == 0)
            flag = 0; // running
        else if (strcmp(arguments[1], "-s") == 0)
            flag = 1; // stopped
        else
        {
            printf("Invalid argument\n");
            return;
        }
    }
    // if(jobs_count != 0)
    // {
    for (int i = 0; i < 1000; i++)
    {
        if (bg_pid_array[i] != 0)
        {
            jpinfo(bg_pid_array[i], flag);
        }
    }
    // }
    

    for (int i=0; i<1000; i++) {
        jobs_copy[i] = jobs[i];
    }
    sort_jobs();

    for (int i = 0; i < jobs_count; i++)
    {

        if (jobs_copy[i].status != 'T')
            printf("[%d] %s %s [%d]\n", i + 1, "Running", jobs_copy[i].name, jobs_copy[i].pid);
        else
            printf("[%d] %s %s [%d]\n", i + 1, "Stopped", jobs_copy[i].name, jobs_copy[i].pid);
    }
}



void bg(int j,char **arguments)
{

    if(j!=2)
    {
        perror("Error: Invalid number of arguments\n");
        return;
    }

    int pid = atoi(arguments[1]);
    if(pid<0 || pid>100)
    {
        perror("Error: Invalid pid\n");
    }

    // for(int i=0;i<5;i++)
    // {
    //     fprintf(stderr,"%d\n",jobs[i].pid);
    // }
    if(bg_pid_array[pid-1]==0) {
        perror("Error: No such job\n");
        return;
    }
    kill(bg_pid_array[pid-1],SIGCONT);
}

void fg(int j,char **arguments)
{
    int Sig;

    if(j!=2)
    {
        perror("Error: Invalid number of arguments\n");
        return;
    }

    int pid = atoi(arguments[1]);
    if(pid<0 || pid>100)
    {
        perror("Error: Invalid pid\n");
    }

    if(bg_pid_array[pid-1]==0) {
        perror("Error: No such job\n");
        return;
    }
       
    kill(bg_pid_array[pid-1],SIGCONT);
    waitpid(bg_pid_array[pid-1],&Sig,WUNTRACED);
    bg_pid_array[pid-1]=0;
    
}

void sig(int j,char **arguments)
{
    int Sig;

    if(j!=3)
    {
        perror("Error: Invalid number of arguments\n");
        return;
    }

    int pid = atoi(arguments[1]);
    int signal= atoi(arguments[2]);
    if(pid<0 || pid>100)
    {
        perror("Error: Invalid pid\n");
    }

    if(bg_pid_array[pid-1]==0) {
        perror("Error: No such job\n");
        return;
    }
       
    if(kill(bg_pid_array[pid-1],signal)==-1)
    {
        perror("Error: Invalid signal\n");
    }

    
}