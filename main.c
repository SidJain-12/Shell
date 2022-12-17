#include "headers.h"


char *iwd, *cwd, *prewd;
char *history[20];
int pointer = 0;
int og_pid;
int pid_array[1000];

double tt;

void die(const char *s) {
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

/**
 * Enable row mode for the terminal
 * The ECHO feature causes each key you type to be printed to the terminal, so you can see what you’re typing.
 * Terminal attributes can be read into a termios struct by tcgetattr().
 * After modifying them, you can then apply them to the terminal using tcsetattr().
 * The TCSAFLUSH argument specifies when to apply the change: in this case, it waits for all pending output to be written to the terminal, and also discards any input that hasn’t been read.
 * The c_lflag field is for “local flags”
 */
void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

int main()
{

    iwd = (char *)malloc(sizeof(char) * 1024);
    cwd = (char *)malloc(sizeof(char) * 1024);
    prewd = (char *)malloc(sizeof(char) * 1024);

    load_history();
    og_pid = getpid();
    iwd = getcwd(NULL, 0);
    cwd = getcwd(NULL, 0);
    prewd = getcwd(NULL, 0);

    signal(SIGCHLD, SigchldHandler);
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);

    char **commands;
    int fore_num, back_num;
    char **fore_commands;
    char **back_commands;
    int num[2];
    int num_commands = 0;

    char *inp = malloc(sizeof(char) * 100);
    char c;

    tt = 0;
    while (1)
    {
        setbuf(stdout, NULL);
        enableRawMode();
        prompt();
        memset(inp, '\0', 100);
        int pt = 0;
        tt = 0;
        int tab_flag=0;

        // char *command = NULL;
        char *command=malloc(sizeof(char) * 100);
        char *token_original;
        size_t comm_inp = 0;
autocomplete:;
        char* new_command = NULL;
        // int return_check = getline(&command, &comm_inp, stdin); // Take input commands

        while (read(STDIN_FILENO, &c, 1) == 1) {
            if (iscntrl(c)) {
                if (c == 10) break;
                else if (c == 27) {
                    char buf[3];
                    buf[2] = 0;
                    if (read(STDIN_FILENO, buf, 2) == 2) { // length of escape code
                        // printf("\rarrow key: %s", buf);
                    }
                } else if (c == 127) { // backspace
                    if (pt > 0) {
                        if (inp[pt-1] == 9) {
                            for (int i = 0; i < 7; i++) {
                                printf("\b");
                            }
                        }
                        inp[--pt] = '\0';
                        printf("\b \b");
                    }
                } else if (c == 9) { // TAB character
                    inp[pt++] = c;
                    // for (int i = 0; i < 8; i++) { // TABS should be 8 spaces
                    //     printf(" ");
                    // }
                    autocomplete(inp, &new_command);
                    if (new_command != NULL) {
                        // printf("NEW COMMAND: %s\n", new_command);
                        // continue;
                        pt = 0;
                        for (int i = 0; i < strlen(new_command); i++) {
                            inp[pt++] = new_command[i];
                        }
                        goto autocomplete;
                    }
                } else if (c == 4) {
                    exit(0);
                } else {
                    printf("%d\n", c);
                }
            } else {
                inp[pt++] = c;
                printf("%c", c);
            }
        }
        inp[pt] = '\0';

        disableRawMode();
        printf("\n");

        strcpy(command,inp);
        add_history(command);

        // if (return_check == -1)
        // {
        //     print_red();
        //     printf("\nLogging out\n");
        //     print_reset();
        //     exit(1);
        // }
        get_tokens(command, &fore_commands, &back_commands, num); // Get tokens from input commands


        for (int i = 0; i < num[1]; i++)
        {
            token_original = back_commands[i];
            char *t2 = strdup(token_original);
            char *token = strtok(token_original, " \t");

            char *arguments[1024];
            char *word;
            int j = 0;
            while ((word = strtok_r(t2, " \t", &t2)))
            {
                arguments[j] = word;
                j++;
            }

            // pid_t pid = fork();
            // int status;
            // if (pid == 0)
            // {
            
            background_process(j, arguments);
            //     exit(0);
            // }
        }

        // printf("%d\n", num[0]);
        // for (int i = 0; i < num[0]; i++)
        //     printf("%s\n", fore_commands[i]);

        for (int i = 0; i < num[0]; i++)
        {
            // semi colon separated foreground commands
            char tempCommand[500];
            strcpy(tempCommand, fore_commands[i]);
            token_original = fore_commands[i];
            char *t2 = strdup(token_original);
            char *token = strtok(token_original, " \t");
            char *arguments[1024];
            char *word;
            int j = 0;
            while ((word = strtok_r(t2, " \t", &t2)))
            {
                arguments[j] = word;
                j++;
            }
            int stdin_copy = dup(0);
            int stdout_copy = dup(1);

            if (isPipe(j, arguments))
            {
                // printf("fore command = %s\n", tempCommand);
                piping(tempCommand);
            }
            else
            {
                run_commands(j, arguments);
            }

            dup2(stdin_copy, 0);
            dup2(stdout_copy, 1);
        }
        // }
    }
}
