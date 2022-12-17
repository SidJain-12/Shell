#include "headers.h"
#define ENOENT 2

int cmpfunc(const void *a, const void *b)
{
    // compare fucntion for qsort to sort the files

    return strcasecmp(*(const char **)a, *(const char **)b);
}
void _ls(const char *dir, int op_a, int op_l)
{
    // Here we will list the directory
    struct dirent *d;
    DIR *dh = opendir(dir);
    char *files[1000];
    char *dirs[1000];
    char *all[1000];
    int num_files = 0;
    int num_dirs = 0;
    int num_all = 0;
    if (!dh)
    {
        // If the directory is not found
        perror("Directory doesn't exist");
        exit(EXIT_FAILURE);
    }
    while ((d = readdir(dh)) != NULL)
    {
        // store files in an array

        if (d->d_type == DT_REG)
        {
            files[num_files] = d->d_name;
            all[num_all] = d->d_name;
            num_files++;
            num_all++;
        }

        // store directories in an array
        if (d->d_type == DT_DIR)
        {
            dirs[num_dirs] = d->d_name;
            all[num_all] = d->d_name;
            num_all++;
            num_dirs++;
        }
    }
    // sort the files and directories

    qsort(files, num_files, sizeof(char *), cmpfunc);
    qsort(dirs, num_dirs, sizeof(char *), cmpfunc);
    qsort(all, num_all, sizeof(char *), cmpfunc);

    for (int a = 0; a < num_all; a++)
    {
        if (!op_a && all[a] && all[a][0] == '.')
            continue;

        // if all[a] in dirs
        int flag = 0;

        for (int b = 0; b < num_dirs; b++)
        {
            if (strcmp(all[a], dirs[b]) == 0)
            {
                flag = 1;
                break;
            }
        }
        if (flag == 1)
        {
            print_blue();
            printf("%s\n", all[a]);
            if (op_l)
                printf("\n");
            print_reset();
        }
        else
        {
            // if file is executable print green
            char *path = (char *)malloc(sizeof(char) * 1000);
            strcpy(path, dir);
            strcat(path, "/");
            strcat(path, all[a]);
            struct stat st;
            stat(path, &st);
            if (st.st_mode & S_IXUSR)
            {
                print_green();
                printf("%s\n", all[a]);
                if (op_l)
                    printf("\n");
                print_reset();
            }
            else
            {
                print_white();
                printf("%s\n", all[a]);
                if (op_l)
                    printf("\n");
                print_reset();
            }
        }
    }
    closedir(dh);
}

void ls_a()
{
    _ls(getcwd(NULL, 0), 1, 0);
}

void ls_l(const char *dir, int op_a)
{
    DIR *thedirectory;
    struct dirent *thefile;
    struct stat thestat;
    struct passwd *tf;
    struct group *gf;
    char buf[512];

    char *files[1000];
    char *dirs[1000];
    char *all[1000];
    int num_files = 0;
    int num_dirs = 0;
    int num_all = 0;

    thedirectory = opendir(dir);

    while ((thefile = readdir(thedirectory)) != NULL)
    {
        // store files in an array

        if (thefile->d_type == DT_REG)
        {
            files[num_files] = thefile->d_name;
            num_files++;
            all[num_all] = thefile->d_name;
            num_all++;
        }
        // store directories in an array
        if (thefile->d_type == DT_DIR)
        {
            dirs[num_dirs] = thefile->d_name;
            num_dirs++;
            all[num_all] = thefile->d_name;
            num_all++;
        }
    }

    // sort the files and directories
    qsort(files, num_files, sizeof(char *), cmpfunc);
    qsort(dirs, num_dirs, sizeof(char *), cmpfunc);
    qsort(all, num_all, sizeof(char *), cmpfunc);

    // print total number of blocks
    int total = 0;
    for (int i = 0; i < num_all; i++)
    {
        if (!op_a && all[i] && all[i][0] == '.')
            continue;
        char *path = (char *)malloc(sizeof(char) * 1000);
        strcpy(path, dir);
        strcat(path, "/");
        strcat(path, all[i]);
        struct stat st;
        stat(path, &st);
        total += st.st_blocks;
    }
    printf("total %d\n", total );

    for (int a = 0; a < num_all; a++)
    {
        if (!op_a && all[a] && all[a][0] == '.')
            continue;

        // if all[a] in dirs
        

        sprintf(buf, "%s/%s", dir, all[a]);

        stat(buf, &thestat);

        switch (thestat.st_mode & S_IFMT)
        {
        case S_IFBLK:
            printf("b");
            break;
        case S_IFCHR:
            printf("c");
            break;
        case S_IFDIR:
            printf("d");
            break; // It's a (sub)directory
        case S_IFIFO:
            printf("p");
            break; // fifo
        case S_IFLNK:
            printf("l");
            break; // Sym link
        case S_IFSOCK:
            printf("s");
            break;
        // Filetype isn't identified
        default:
            printf("-");
            break;
        }
        //[permissions]
        printf((thestat.st_mode & S_IRUSR) ? "r" : "-");
        printf((thestat.st_mode & S_IWUSR) ? "w" : "-");
        printf((thestat.st_mode & S_IXUSR) ? "x" : "-");
        printf((thestat.st_mode & S_IRGRP) ? "r" : "-");
        printf((thestat.st_mode & S_IWGRP) ? "w" : "-");
        printf((thestat.st_mode & S_IXGRP) ? "x" : "-");
        printf((thestat.st_mode & S_IROTH) ? "r" : "-");
        printf((thestat.st_mode & S_IWOTH) ? "w" : "-");
        printf((thestat.st_mode & S_IXOTH) ? "x" : "-");

        // [number of hard links]

        printf("\t%d ", thestat.st_nlink);

        //[owner]
        tf = getpwuid(thestat.st_uid);
        printf("\t%s ", tf->pw_name);

        //[group]
        gf = getgrgid(thestat.st_gid);
        printf("\t%s ", gf->gr_name);

        //[size in bytes] [time of last modification] [filename]
        printf("\t%lld", thestat.st_size);
        char date[20];
        strftime(date, 20, "%b %d %H:%M", localtime(&thestat.st_mtime));
        printf("\t%s", date);
        int flag = 0;

        for (int b = 0; b < num_dirs; b++)
        {
            if (strcmp(all[a], dirs[b]) == 0)
            {
                flag = 1;
                break;
            }
        }
        if (flag == 1)
        {
            print_blue();
        }
        else
        {
            char *path = (char *)malloc(sizeof(char) * 1000);
            strcpy(path, dir);
            strcat(path, "/");
            strcat(path, all[a]);
            struct stat st;
            stat(path, &st);
            if (st.st_mode & S_IXUSR)
            {
                print_green();
            }
            else
            {
                print_white();
            }
        }
        printf("\t%s \n", all[a]);

        print_reset();
    }

    closedir(thedirectory);
}

void file_permissions(int op_a, char *file, char *cwd)
{
    char buf[512];
    struct stat thestat;
    struct passwd *tf;
    struct group *gf;
    print_white();
    // if (!op_a)
    //     return;

    sprintf(buf, "%s/%s", cwd, file);

    stat(buf, &thestat);

    switch (thestat.st_mode & S_IFMT)
    {
    case S_IFBLK:
        printf("b");
        break;
    case S_IFCHR:
        printf("c");
        break;
    case S_IFDIR:
        printf("d");
        break; // It's a (sub)directory
    case S_IFIFO:
        printf("p");
        break; // fifo
    case S_IFLNK:
        printf("l");
        break; // Sym link
    case S_IFSOCK:
        printf("s");
        break;
    // Filetype isn't identified
    default:
        printf("-");
        break;
    }
    //[permissions]
    printf((thestat.st_mode & S_IRUSR) ? "r" : "-");
    printf((thestat.st_mode & S_IWUSR) ? "w" : "-");
    printf((thestat.st_mode & S_IXUSR) ? "x" : "-");
    printf((thestat.st_mode & S_IRGRP) ? "r" : "-");
    printf((thestat.st_mode & S_IWGRP) ? "w" : "-");
    printf((thestat.st_mode & S_IXGRP) ? "x" : "-");
    printf((thestat.st_mode & S_IROTH) ? "r" : "-");
    printf((thestat.st_mode & S_IWOTH) ? "w" : "-");
    printf((thestat.st_mode & S_IXOTH) ? "x" : "-");

    // [number of hard links]

    printf("\t%d ", thestat.st_nlink);

    //[owner]
    tf = getpwuid(thestat.st_uid);
    printf("\t%s ", tf->pw_name);

    //[group]
    gf = getgrgid(thestat.st_gid);
    printf("\t%s ", gf->gr_name);

    //[size in bytes] [time of last modification] [filename]
    printf("\t%lld", thestat.st_size);
    char date[20];
    strftime(date, 20, "%b %d %H:%M", localtime(&thestat.st_mtime));
    printf("\t%s", date);
    printf("\t%s \n", file);

    print_reset();
}

void ls_command(int j, char **arguments)
// void ls_command(char *token)
{

    char *dirs[1024];
    char *files[1024];
    int num_dirs = 0;
    int num_files = 0;
    int to_return = 0;

    int op_a = 0;
    int op_l = 0;

    for (int i = 1; i < j; i++)
    {
        if (strcmp(arguments[i], "-a") == 0)
        {
            op_a = 1;
        }
        else if (strcmp(arguments[i], "-l") == 0)
        {
            op_l = 1;
        }
        else if (strcmp(arguments[i], "-al") == 0 || strcmp(arguments[i], "-la") == 0)
        {
            op_a = 1;
            op_l = 1;
        }
        else if (opendir(arguments[i]) != NULL)
        {
            dirs[num_dirs] = arguments[i];
            num_dirs++;
        }
        else if (access(arguments[i], F_OK) == 0)
        {
            files[num_files] = arguments[i];
            num_files++;
        }
        else if (strncmp(arguments[i], "~", 1) == 0)
        {
            // dirs[num_dirs] = owd;
            char *temp = (char *)malloc(sizeof(char) * 1024);
            strcpy(temp, iwd);
            strcat(temp, arguments[i] + 1);
            if (opendir(temp) != NULL)
            {
                dirs[num_dirs] = temp;
                num_dirs++;
                
            }
            else if (access(temp, F_OK) == 0)
            {
                files[num_files] = temp;
                num_files++;
            }
            else
            {
                printf("%s: No such file or directory\n", arguments[i]);
                to_return = 1;
            }
        }
        else
        {
            printf("%s: No such file or directory\n", arguments[i]);
            to_return = 1;
        }
    }

    if (to_return == 1)
        return;

    qsort(files, num_files, sizeof(char *), cmpfunc);

    for (int i = 0; i < num_files; i++)
    {
        if (op_l)
        {
            file_permissions(op_a, files[i], getcwd(NULL, 0));
        }
        else
        {
            print_white();
            printf("%s\n", files[i]);
            print_reset();
        }
    }

    if (num_dirs == 0 && num_files == 0 && op_l)
    {
        ls_l(getcwd(NULL, 0), op_a);
    }
    else if (num_dirs == 0 && num_files == 0 && !op_l)
    {
        _ls(getcwd(NULL, 0), op_a, 0);
    }
    else if (num_dirs == 1 && num_files == 0 && op_l)
    {
        ls_l(dirs[0], op_a);
    }
    else if (num_dirs == 1 && num_files == 0 && !op_l)
    {
        _ls(dirs[0], op_a, 0);
    }
    else
    {
        qsort(dirs, num_dirs, sizeof(char *), cmpfunc);

        if (!op_l)
        {
            for (int i = 0; i < num_dirs; i++)
            {
                print_blue();
                printf("%s:\n", dirs[i]);
                _ls(dirs[i], op_a, op_l);
                print_reset();
            }
        }
        else
        {
            for (int i = 0; i < num_dirs; i++)
            {
                print_blue();
                printf("%s:\n", dirs[i]);
                ls_l(dirs[i], op_a);
                print_reset();
            }
        }
    }
}
