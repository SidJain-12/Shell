#include "headers.h"


void load_history()
{
    FILE *fp = fopen("history.txt", "r");
    if (fp == NULL)
    {
        return;
    }
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int i = 0;

    // load contents from history to history.txt
    while ((read = getline(&line, &len, fp)) != -1)
    {
        if ( line[read-1 ] == '\n') {
            line[read-1] = '\0';
        }
        // printf("%s", line);
        history[i] = strdup(line);
        pointer++;
        i++;
    }
    fclose(fp);
}

void add_history(char *command)
{
    
    // maximum 20 commands
    if(pointer>0 && strcmp(command,history[pointer-1])==0)
    {
        return;
    }
    if (pointer == 20)
    {
        for (int i = 0; i < 19; i++)
        {
            free(history[i]);
            history[i] = strdup(history[i + 1]);
        }
        free(history[19]);
        history[19] = strdup(command);
    }
    else
    {
        free(history[pointer]);
        history[pointer] = strdup(command);
        pointer++;
    }
    
}

void save_history()
{
    char* filename;
    filename = (char*)malloc(sizeof(char)*1024);
    strcpy(filename, iwd);
    strcat(filename, "/history.txt");

    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("Error in opening history file\n");
        exit(1);
    }
   
    for (int i = 0; i < pointer; i++)
    {
        fprintf(fp, "%s\n", history[i]);
    }
    fclose(fp);
}

void history_command(int n)
{
    // read contents from history.txt

    for (int i = 20-n; i < 20; i++)
    {
        if (history[i] != NULL)
        {
            printf("%s\n", history[i]);
        }
    }
     
}

void call_history(int j,char** arguments)
{
    if(j==1) // history
    {
        history_command(20);
    }
    else if(j==2) // history n
    {
        int n = atoi(arguments[1]);
        if(n>20)
        {
            printf("Error: n should be less than 20\n");
            return;
        }
        history_command(n);
    }
    else
    {
        printf("Error: Invalid number of arguments\n");
    }
}