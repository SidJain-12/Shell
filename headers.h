#ifndef __DEF_FUNCTIONS_H__
#define __DEF_FUNCTIONS_H__

#include <stdio.h>    
#include <string.h>
#include <pwd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <grp.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <fcntl.h>
#include <termios.h>
#include <ctype.h>

#define ENOENT 2
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1b[33m"
#define NORMAL  "\x1B[0m"
#define CYAN "\x1b[36m"
#define WHITE "\x1b[37m"

extern char *iwd, *cwd, *prewd;
extern char *history[20];
extern int pointer;

void prompt();

void run_commands( int j, char **arguments);

void get_tokens(char*, char***, char ***, int *);
void pipe_tokenise(char*, char***, int *);

void pwd();

void echo(int,char**);

int cmpfunc (const void * a, const void * b);
void _ls(const char*, int, int);
void ls_a();
void ls_l(const char*, int);
void ls_command(int,char**);

void cd(char*);

void print_yellow();
void print_green();
void print_blue();
void print_red();
void print_reset();
void print_white();

void load_history();
void add_history(char*);
void save_history();
void history_command(int);
void call_history(int,char**);

void read_file(pid_t pid);
int pinfo(int,char**);

void foreground_process(int,char**);
void background_process(int,char**);

void discover(char *,int,int ,char *);
void discover_command(int,char**);

void redirection(int *,char**,FILE **,FILE **);
void io_redirection(int *,char**);
int isRedirection(int ,char**);

void piping(char*);
int isPipe(int ,char**);

void SigintHandler(int);
void SigchldHandler(int);
void SigstpHandler(int);

void jread_file(pid_t pid);
void jpinfo(pid_t, int);
void sort_jobs();
void print_jobs(int ,char**);
void bg(int ,char**);
void fg(int ,char**);
void sig(int ,char**);

void autocomplete(char*, char**);

extern double tt;
extern int og_pid;
extern int bg_pid_array[1000];

// int stdout_copy;
// int stdin_copy;


#endif