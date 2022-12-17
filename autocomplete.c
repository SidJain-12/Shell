#include "headers.h"

char* common_prefix(char arr[100][100], int num)
{
    int i,j;
    char *prefix = (char*)malloc(100*sizeof(char));
    for(i=0;i<strlen(arr[0]);i++)
    {
        for(j=1;j<num;j++)
        {
            if(arr[j][i] == '\0' || arr[0][i] == '\0' || arr[j][i]!=arr[0][i])
            {
                prefix[i] = '\0';
                return prefix;
            }
        }
        prefix[i] = arr[0][i];
    }
    return prefix;
}

void autocomplete(char * inp, char ** new_command)
{
    char *token =strdup(inp);

    char* sptr1, *sptr2;
    char* autocomplete_word;
    char* subtoken = strtok_r(token, " \t\n;&", &sptr1);
    if (subtoken != NULL) 
        autocomplete_word = subtoken;
    while (subtoken != NULL) {
        subtoken = strtok_r(NULL, " \t\n;&", &sptr1);
        if (subtoken != NULL)
            autocomplete_word = subtoken;
    }

    // Opening current directory
    DIR* dir = opendir(".");
    if (dir == NULL) {
        perror("opendir() error");
        exit(1);
    }

    // Reading directory
    int num=0;
    char arr[100][100];
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        char *name = entry->d_name;
        if (strncmp(entry->d_name, autocomplete_word, strlen(autocomplete_word)) == 0) 
        {
            if(entry->d_type == DT_DIR){
                name=strcat(name,"/");
                strcpy(arr[num], name);
                num++;
            }
            else{
                strcpy(arr[num], name);
                num++;
            }
        }

    }

    if(num==0)
    {
        return;
    }
    else if(num==1)
    {
        inp[strlen(inp)-1]='\0';
        printf("%s",&arr[0][strlen(autocomplete_word)]);
        *new_command = strcat(inp,&arr[0][strlen(autocomplete_word)]);
    }
    else
    {
        printf("\n");
        for(int i=0;i<num;i++)
        {
            printf("%s\t",arr[i]);
        }
        printf("\n");

        inp[strlen(inp) - 1] = '\0';
        char * common_word = common_prefix(arr, num);
        prompt();
        printf("%s%s",inp,&common_word[strlen(autocomplete_word)]);
        *new_command = strdup(inp);
        strcat(*new_command, &common_word[strlen(autocomplete_word)]);
    }
}