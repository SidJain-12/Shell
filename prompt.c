#include "headers.h"
#define _PROGRAM_NAME "Sid prompt"

void prompt()
{
    register struct passwd *pw;
    register uid_t uid;

    uid = geteuid();
    pw = getpwuid(uid);

    // get username
    char *username = pw->pw_name;
    // get hostname
    char hostname[1024];
    gethostname(hostname, sizeof(hostname));

    cwd = getcwd(NULL, 0);
    
    // get current working directory
    if(strncmp(cwd, iwd, strlen(iwd)) == 0)
    {
        if(tt>=1)
            printf("%s@%s:~%s took %.2f seconds$ ", username, hostname, cwd+strlen(iwd) ? cwd+strlen(iwd) : "",tt);
        else
             printf("%s@%s:~%s$ ", username, hostname, cwd+strlen(iwd) ? cwd+strlen(iwd) :"");
    }
    else
    {
        if(tt>=1)
            printf("%s@%s:%s took %.2f seconds $ ", username, hostname, cwd,tt);

        else
            printf("%s@%s:%s$ ", username, hostname, cwd);
    }
}
