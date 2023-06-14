#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <grp.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <pwd.h>
#include <signal.h>

char *initial, *prev_dir, *home_path, *userid, *userid1;
char *at = "@", *colon = ":", *str, *tilda = "~", *right_arrow = ">", *slash = "/";
char delimiter[] = "\t\r\n\v\f ";
int g = 0, flags = 0, cnn = 0;
int pid_st[2000];
int yy = 0, tim = 0;
int dd = 0;
int foregr = 0;
int are_we_redirecting = 0;
int original;
pid_t shellid, childpid, pidt;

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define RESET "\x1B[0m"

void echo(char *inp)
{
    char delimit[] = "\t\r\n\v\f ";
    char *sub = strtok(inp, delimit);
    sub = strtok(NULL, delimit);
    while (sub != NULL)
    {
        printf("%s ", sub);
        sub = strtok(NULL, delimit);
    }
    printf("\n");
    return;
}

void pwd()
{
    char *path = getcwd(NULL, 1024);
    if (path == NULL)
    {
        if (errno == ERANGE)
        {
            printf("\033[0;34m"); // Set the text to the color blue
            printf("Path exceeded max buffer length\n");
            printf("\033[0m"); // Resets the text to default color
        }
        else if (errno == ENOMEM)
        {
            printf("\033[0;34m"); // Set the text to the color blue
            printf("Memory cannot be acllocated for path.\n");
            printf("\033[0m"); // Resets the text to default color
        }
        else
        {
            printf("\033[0;34m"); // Set the text to the color blue
            printf("Failed to retrieve the path.\n");
            printf("\033[0m"); // Resets the text to default color
        }
    }
    else
        printf("%s\n", path);
    return;
}

int cd(char *inp)
{

    char delimit[] = "\t\r\n\v\f ";
    strtok(inp, delimit);
    char *second = strtok(NULL, delimit);
    if (second == NULL)
    {
        prev_dir = getcwd(NULL, 1024);
        if (flags == 0)
        {
            flags = 1;
        }
        strcpy(userid, initial);
        strcat(userid, tilda);
        strcat(userid, right_arrow);
        chdir(home_path);
        return 0;
    }
    else if (strcmp(second, ".") == 0)
    {
        if (flags == 0)
        {
            flags = 1;
        }
        return 0;
    }
    else if (strcmp(second, "..") == 0)
    {
        if (flags == 0)
        {
            flags = 1;
        }
        prev_dir = getcwd(NULL, 1024);
        chdir("..");
        char *another2 = (char *)malloc(1024 * sizeof(char));
        strcpy(another2, initial);
        strcat(another2, tilda);
        strcat(another2, right_arrow);
        if (strncmp(another2, userid, strlen(another2)) == 0)
        {
            char *path = getcwd(NULL, 1024);
            strcpy(userid, initial);
            strcat(userid, path);
            strcat(userid, right_arrow);
            return 0;
        }

        char *another;
        another = (char *)malloc(1024 * sizeof(char));
        strncpy(another, userid, strlen(userid) - 1);
        int i = strlen(another) - 1;
        while (another[i--] != '/')
        {
        }
        strncpy(userid, another, i);
        userid[i + 1] = '\0';
        strcat(userid, right_arrow);
    }
    else if (strcmp(second, "~") == 0)
    {
        if (flags == 0)
        {
            flags = 1;
        }
        prev_dir = getcwd(NULL, 1024);
        strcpy(userid, initial);
        strcat(userid, tilda);
        strcat(userid, right_arrow);
        chdir(home_path);
        return 0;
    }
    else if (strcmp(second, "-") == 0)
    {

        if (flags == 0)
        {
            flags = 1;
            printf("\033[0;34m"); // Set the text to the color blue
            printf("bash: cd: OLDPWD not set\n");
            printf("\033[0m"); // Resets the text to default color

            return 0;
        }
        char *temp_dir = (char *)malloc(1024 * sizeof(char));
        strcpy(temp_dir, prev_dir);
        prev_dir = getcwd(NULL, 1024);
        chdir(temp_dir);
        if (strncmp(temp_dir, home_path, strlen(home_path)) == 0)
        {
            char *execute = (char *)malloc(1024 * sizeof(char));
            strcpy(execute, initial);
            strcat(execute, tilda);
            int l = strlen(execute), k = 0;
            for (int i = strlen(home_path); i < strlen(temp_dir); ++i)
            {
                execute[l + k] = temp_dir[i];
                k++;
            }
            strcpy(userid, execute);
            strcat(userid, right_arrow);
        }
        else
        {
            if (flags == 0)
            {
                flags = 1;
            }
            char *execute = (char *)malloc(1024 * sizeof(char));
            strcpy(execute, initial);
            strcat(execute, temp_dir);
            strcat(execute, right_arrow);
            strcpy(userid, execute);
        }
        printf("%s\n", getcwd(NULL, 1024));
    }
    else
    {
        if (flags == 0)
        {
            flags = 1;
        }

        if (!opendir(second))
        {
            printf("\033[0;34m"); // Set the text to the color blue
            printf("No such file or directory.\n");
            printf("\033[0m"); // Resets the text to default color
        }
        else
        {
            prev_dir = getcwd(NULL, 1024);
            chdir(second);

            if (strncmp(home_path, second, strlen(home_path)) == 0)
            {
                char second_half[strlen(second) - strlen(home_path) + 5];
                for (int i = 0; i < strlen(second) - strlen(home_path) + 1; ++i)
                {
                    second_half[i] = second[i + strlen(home_path)];
                }

                char *another;
                another = (char *)malloc(1024 * sizeof(char));
                strcpy(another, initial);
                strcat(another, tilda);
                strcat(another, second_half);
                strcat(another, right_arrow);
                strcpy(userid, another);
            }
            else if (second[0] != '/')
            {
                char *path = getcwd(NULL, 1024);
                if (strcmp(path, home_path) == 0)
                {
                    char *another = (char *)malloc(1024 * sizeof(char));
                    strcpy(another, initial);
                    strcat(another, tilda);
                    strcat(another, right_arrow);
                    strcpy(userid, another);
                    free(another);
                    return 0;
                }
                else
                {
                    char *another = (char *)malloc(1024 * sizeof(char));
                    strncpy(another, userid, strlen(userid) - 1);
                    strcat(another, slash);
                    strcat(another, second);

                    strcat(another, right_arrow);
                    strcpy(userid, another);
                    free(another);
                    return 0;
                }
            }
            else
            {
                char *path = getcwd(NULL, 1024);
                char *temp = (char *)malloc(1024 * sizeof(char));
                strcpy(temp, initial);
                strcat(temp, path);
                strcat(temp, right_arrow);
                strcpy(userid, temp);
            }
        }
    }
    return 0;
}

void sort(int m, int n, char a[m][n], int cnt)
{
    for (int i = 0; i < cnt - 1; ++i)
    {
        for (int j = 0; j < cnt - i - 1; ++j)
        {
            if (strcasecmp(a[j], a[j + 1]) > 0)
            {
                char temp[100];
                strcpy(temp, a[j + 1]);
                strcpy(a[j + 1], a[j]);
                strcpy(a[j], temp);
            }
        }
    }
}

void forl(char *file, char *end)
{
    struct stat buf;
    stat(file, &buf);
    int arr[9];
    arr[0] = buf.st_mode & S_IRUSR; // read permission of user
    arr[1] = buf.st_mode & S_IWUSR; // write permission of user
    arr[2] = buf.st_mode & S_IXUSR; // exevute permission of user
    arr[3] = buf.st_mode & S_IRGRP; // read permission of Group
    arr[4] = buf.st_mode & S_IWGRP; // write permission of Group
    arr[5] = buf.st_mode & S_IXGRP; // exevute permission of Group
    arr[6] = buf.st_mode & S_IROTH; // read permission of Others
    arr[7] = buf.st_mode & S_IWOTH; // write permission of Others
    arr[8] = buf.st_mode & S_IXOTH; // exevute permission of Others
    if (buf.st_mode & S_IFDIR)
        printf("d");
    else
        printf("-");
    for (int j = 0; j < 9; ++j)
    {
        if (arr[j] > 0)
        {
            if (j % 3 == 0)
                printf("r");
            else if (j % 3 == 1)
                printf("w");
            else if (j % 3 == 2)
                printf("x");
        }
        else
        {
            if (j % 3 == 0)
                printf("-");
            else if (j % 3 == 1)
                printf("-");
            else if (j % 3 == 2)
                printf("-");
        }
    }
    printf("\t");
    printf("%ld\t", buf.st_nlink);
    printf("%s\t", getpwuid(buf.st_uid)->pw_name);
    printf("%s\t", getgrgid(buf.st_gid)->gr_name);
    printf("%ld\t", buf.st_size);

    time_t now = time(NULL);
    struct tm tmfile, tmnow;
    localtime_r(&buf.st_mtime, &tmfile);
    localtime_r(&now, &tmnow);
    char time_str[100];

    if (tmfile.tm_year == tmnow.tm_year)
    {
        strftime(time_str, sizeof(time_str), "%b %e %H:%M", &tmfile);
        printf("%s\t", time_str);
    }
    else
    {
        strftime(time_str, sizeof(time_str), "%b %e  %Y", &tmfile);
        printf("%s\t", time_str);
    }
    struct stat path;
    stat(end, &path);
    if (S_ISDIR(path.st_mode))
    {
        printf(BLU "%s\t\n" RESET, end);
        return;
    }
    else if (S_IXUSR & path.st_mode)
    {
        printf(GRN "%s\t\n" RESET, end);
        return;
    }
    printf("%s\n", end);
}

void ls_print(char *inp, char temp[1000][100])
{
    int a = 0;
    DIR *dir;
    dir = opendir(inp);
    if (dir == NULL)
    {
        printf("Error opening the directory.\n");
        return;
    }
    struct dirent *entity;
    entity = readdir(dir);
    while (entity != NULL)
    {
        if (entity->d_name[0] != '.')
        {
            strcpy(temp[a++], entity->d_name);
        }
        entity = readdir(dir);
    }
    sort(1000, 100, temp, a);
    for (int i = 0; i < a; ++i)
    {
        struct stat path;
        stat(temp[i], &path);
        if (S_ISDIR(path.st_mode))
        {
            printf(BLU "%s\t" RESET, temp[i]);
            continue;
        }
        else if (S_IXUSR & path.st_mode)
        {
            printf(GRN "%s\t" RESET, temp[i]);
            continue;
        }
        printf("%s\t", temp[i]);
    }
    printf("\n");
    closedir(dir);
}

void ls_print1(char *inp, char temp[1000][100])
{
    int a = 0;
    DIR *dir;
    dir = opendir(inp);
    if (dir == NULL)
    {
        printf("Error opening the directory.\n");
        return;
    }
    struct dirent *entity;
    entity = readdir(dir);
    while (entity != NULL)
    {
        strcpy(temp[a++], entity->d_name);
        entity = readdir(dir);
    }
    sort(1000, 100, temp, a);
    for (int i = 0; i < a; ++i)
    {
        struct stat path;
        stat(temp[i], &path);
        if (S_ISDIR(path.st_mode))
        {
            printf(BLU "%s\t" RESET, temp[i]);
            continue;
        }
        else if (S_IXUSR & path.st_mode)
        {
            printf(GRN "%s\t" RESET, temp[i]);
            continue;
        }
        printf("%s\t", temp[i]);
    }
    printf("\n");
    closedir(dir);
}

void ls(char *inp, int flag, int til)
{
    int inside = 0;
    char directories[500][100];
    char files[500][100];
    strtok(inp, delimiter);
    char *second = strtok(NULL, delimiter);
    DIR *dir;
    struct dirent *entity;
    int no_dir = 0, no_files = 0, fflag = 0;
    struct stat path;
    int is_it_filedir = 0;
    while (second != NULL)
    {
        inside = 1;
        struct stat path;
        stat(second, &path);
        // printf("Path: %s\n", path);
        if (strcmp("-a", second) == 0 || strcmp("-l", second) == 0 || strcmp("-al", second) == 0 || strcmp("-la", second) == 0 || strcmp("~", second) == 0)
        {
            second = strtok(NULL, delimiter);
            continue;
        }
        if (S_ISREG(path.st_mode))
        {
            is_it_filedir = 1;
            strcpy(files[no_files], second);
            no_files++;
        }
        if (S_ISDIR(path.st_mode))
        {
            is_it_filedir = 1;
            strcpy(directories[no_dir], second);
            no_dir++;
        }
        if (!S_ISDIR(path.st_mode) && !S_ISREG(path.st_mode))
        {
            fflag = 1;
            printf("No such file or directory.\n");
            second = strtok(NULL, delimiter);
            continue;
        }
        second = strtok(NULL, delimiter);
    }
    if (til)
    {
        strcpy(directories[no_dir++], home_path);
    }
    sort(500, 100, directories, no_dir);
    sort(500, 100, files, no_files);
    char total[500][100];
    for (int i = 0; i < no_files; ++i)
    {
        strcpy(total[i], files[i]);
    }
    for (int i = no_files; i < no_files + no_dir; ++i)
    {
        strcpy(total[i], directories[i - no_files]);
    }
    int totals = no_dir + no_files;
    char temp[1000][100];
    if (flag == 0)
    {
        if (!inside || (no_files == 0 && no_dir == 0 && is_it_filedir))
        {
            ls_print(".", temp);
            return;
        }
        for (int i = 0; i < no_files; ++i)
        {
            printf("%s\t", files[i]);
        }
        printf("\n\n");
        for (int i = 0; i < no_dir; ++i)
        {
            if ((no_dir + no_files) != 1 || fflag == 1)
                printf("%s: \n", directories[i]);
            ls_print(directories[i], temp);
            printf("\n");
        }
    }

    if (flag == 1)
    {
        if (no_files == 0 && no_dir == 0)
        {
            ls_print1(".", temp);
            return;
        }
        for (int i = 0; i < no_files; ++i)
        {
            printf("%s ", files[i]);
        }
        printf("\n\n");
        for (int i = 0; i < no_dir; ++i)
        {
            if ((no_dir + no_files) != 1)
                printf("%s: \n", directories[i]);
            ls_print1(directories[i], temp);
            printf("\n");
        }
    }
    if (flag == 2)
    {
        if (no_files == 0 && no_dir == 0)
        {
            g = 1;
            strcpy(directories[no_dir++], ".");
        }
        int tot = 0;
        if (no_files)
        {
            struct stat path1;

            for (int i = 0; i < no_files; ++i)
            {
                forl(files[i], files[i]);
            }
            printf("\n");
        }
        if (no_dir)
        {
            char temp1[1000][100];

            for (int i = 0; i < no_dir; ++i)
            {
                int a = 0;
                DIR *dir;
                dir = opendir(directories[i]);
                if (dir == NULL)
                {
                    printf("Error opening the directory.\n");
                    continue;
                }

                struct dirent *entity;
                entity = readdir(dir);
                while (entity != NULL)
                {
                    if (entity->d_name[0] != '.')
                    {
                        strcpy(temp1[a++], entity->d_name);
                    }
                    entity = readdir(dir);
                }
                sort(1000, 100, temp1, a);
                if (!g && (no_dir + no_files) != 1)
                    printf("%s:\n", directories[i]);
                struct stat path;
                stat(directories[i], &path);

                // printf("%d\n", a);
                for (int j = 0; j < a; ++j)
                {
                    tot += path.st_blocks;
                }
                printf("total %d\n", tot);
                for (int j = 0; j < a; ++j)
                {
                    // printf("d");
                    char *aa = (char *)malloc(100 * sizeof(char));
                    strcpy(aa, directories[i]);
                    strcat(aa, slash);
                    strcat(aa, temp1[j]);
                    forl(aa, temp1[j]);
                }
                printf("\n");
                closedir(dir);
            }
        }
    }
    if (flag == 3)
    {
        if (no_files == 0 && no_dir == 0)
        {
            g = 1;
            strcpy(directories[no_dir++], ".");
        }
        if (no_files)
        {
            for (int i = 0; i < no_files; ++i)
            {
                // printf("-");
                forl(files[i], files[i]);
            }
            printf("\n");
        }
        if (no_dir)
        {
            char temp1[1000][100];
            for (int i = 0; i < no_dir; ++i)
            {
                int a = 0;
                DIR *dir;
                dir = opendir(directories[i]);
                if (dir == NULL)
                {
                    printf("Error opening the directory.\n");
                    continue;
                }
                struct dirent *entity;
                entity = readdir(dir);
                while (entity != NULL)
                {
                    strcpy(temp1[a++], entity->d_name);
                    entity = readdir(dir);
                }
                sort(1000, 100, temp1, a);
                if (!g && (no_dir + no_files) != 1)
                    printf("%s:\n", directories[i]);
                struct stat path;
                int tot = 0;
                stat(directories[i], &path);

                // printf("%d\n", a);
                for (int j = 0; j < a; ++j)
                {
                    tot += path.st_blocks;
                }
                printf("total %d\n", tot);
                for (int j = 0; j < a; ++j)
                {
                    // printf("d");//
                    char *aa = (char *)malloc(100 * sizeof(char));
                    strcpy(aa, directories[i]);
                    strcat(aa, slash);
                    strcat(aa, temp1[j]);
                    forl(aa, temp1[j]);
                }
                printf("\n");
                //     printf("%s ", temp1[i]);
                // printf("\n");
                closedir(dir);
            }
        }
    }
    closedir(dir);
    return;
}

void history(FILE *histories)
{
    fseek(histories, 0, SEEK_SET);
    char c = fgetc(histories);
    printf("%c\n", c);
    c = fgetc(histories);
    printf("%c\n", c);
    int cnt = 0;
    char a[200];
    // while (cnt != 10)
    // {
    //     char c = fgetc(histories);
    //     if (feof(histories))
    //         break;
    //     printf("%c ", c);
    //     if (c == '\n')
    //         cnt++;
    // }
    for (int i = 0; i < 10; ++i)
    {
        // printf("1");
        fgets(a, 200, histories);
        printf("%s", a);
    }
    return;
}

void pinfo(char *inp)
{
    char path[500]; // for reading file line by line
    char *first = strtok(inp, delimiter);
    char *second = strtok(NULL, delimiter);
    int pid;
    if (second == NULL)
    {
        pid = getpid();
    }
    else
    {
        pid = atoi(second);
    }
    printf("pid : %d\n", pid);
    // printf("%d\n", pid);
    sprintf(path, "/proc/%d/status", pid);
    FILE *fp = fopen(path, "r");
    if (!fp)
        perror("Error while opening the proc/pid/status file\n");
    else
    {
        char p_state;
        char *temp = (char *)malloc(1000 * sizeof(char));
        fgets(temp, 300, fp);
        fgets(temp, 300, fp);
        fgets(temp, 300, fp);
        // printf("temp: %s\n", temp);
        sscanf(temp, "State:\t%c", &p_state);
        // printf("%s %d\n", temp, p_state);
        printf("process Status : %c\n", p_state);
        fclose(fp);
    }
    sprintf(path, "/proc/%d/statm", pid);
    fp = fopen(path, "r");
    if (!fp)
        perror("Error while opening the proc/pid/statm file\n");
    else
    {
        char *temp = (char *)malloc(1000 * sizeof(char));
        int dis = 0;
        int c = fgetc(fp);
        while (c != ' ')
        {
            temp[dis] = c;
            dis += 1;
            c = fgetc(fp);
        }
        temp[dis] = '\0';
        printf("memory : %s\n", temp);
        fclose(fp);
    }

    sprintf(path, "/proc/%d/exe", pid);
    int len = -1;
    char buf[1000];
    len = readlink(path, buf, 499);
    if (len == -1)
    {
        perror("Error while opening the proc/pid/exe file\n");
    }
    else
    {
        buf[len] = '\0';
        if (strncmp(buf, home_path, strlen(home_path)) == 0)
        {
            if (second == NULL)
                printf("~");
            else
                printf("~/");
            for (int i = strlen(home_path); i < strlen(buf); ++i)
            {
                printf("%c", buf[i]);
            }
            printf("\n");
        }
        else
        {
            printf("executable Path : %s\n", buf);
        }
        // printf("Executable Path -- %s\n", buf);
    }
}

void discover_dir(char *dir_name, char *search, char *cur_path, int flag, int search_int)
{
    DIR *dir;
    dir = opendir(dir_name);
    struct dirent *entity;
    entity = readdir(dir);

    while (entity != NULL)
    {
        if (strcmp("..", entity->d_name) == 0 || strcmp(".", entity->d_name) == 0)
        {
            entity = readdir(dir);
            continue;
        }
        struct stat path;
        char *temp = (char *)malloc(200 * sizeof(char));
        strcpy(temp, dir_name);
        strcat(temp, slash);
        strcat(temp, entity->d_name);
        stat(temp, &path);
        if (S_ISDIR(path.st_mode))
        {
            if (search_int == 1)
            {
                if (flag == 1 || flag == 4)
                {
                    if (strcmp(search, entity->d_name) == 0)
                    {
                        if (strncmp(temp, cur_path, strlen(cur_path)) == 0)
                        {
                            printf(".");
                            for (int i = strlen(cur_path); i < strlen(temp); ++i)
                            {
                                printf("%c", temp[i]);
                            }
                            printf("\n");
                            return;
                        }
                        else
                        {
                            printf("%s\n", temp);
                            return;
                        }
                    }
                }
                // discover_dir(temp, search, cur_path, flag, search_int);
            }
            else if (search_int == 0)
            {
                // printf(".\n");
                if (flag == 1 || flag == 0 || flag == 3)
                {
                    if (strncmp(temp, cur_path, strlen(cur_path)) == 0)
                    {
                        printf(".");
                        for (int i = strlen(cur_path); i < strlen(temp); ++i)
                        {
                            printf("%c", temp[i]);
                        }
                        printf("\n");
                    }
                    else
                    {
                        printf("%s\n", temp);
                    }
                }
            }
            discover_dir(temp, search, cur_path, flag, search_int);
        }
        else
        {
            if (search_int == 0 && (flag == 0 || flag == 2 || flag == 3))
            {
                if (strncmp(temp, cur_path, strlen(cur_path)) == 0)
                {
                    printf(".");
                    for (int i = strlen(cur_path); i < strlen(temp); ++i)
                    {
                        printf("%c", temp[i]);
                    }
                    printf("\n");
                }
                else
                {
                    printf("%s\n", temp);
                }
            }
            else if (search_int == 1)
            {
                if (flag == 2 || flag == 4)
                {
                    if (strcmp(entity->d_name, search) == 0)
                    {
                        if (strncmp(temp, cur_path, strlen(cur_path)) == 0)
                        {
                            printf(".");
                            for (int i = strlen(cur_path); i < strlen(temp); ++i)
                            {
                                printf("%c", temp[i]);
                            }
                            printf("\n");
                            return;
                        }
                        else
                        {
                            printf("%s\n", temp);
                            return;
                        }
                    }
                }
            }
        }
        entity = readdir(dir);
    }
}

void discover(char *inp)
{
    char *target_dir = (char *)malloc(200 * sizeof(char));
    // target_dir = ".";
    char *to_search = (char *)malloc(200 * sizeof(char));
    char *first = strtok(inp, delimiter);
    char *second = strtok(NULL, delimiter);
    char *cur_path = getcwd(NULL, 1024);

    if (second == NULL)
    {
        printf(".\n");
        char a[5] = "", b[5] = ".";
        discover_dir(cur_path, a, cur_path, 0, 0);
    }

    else
    {
        int c = 0, d = 0, f = 0, search = 0, u = 0;
        while (second != NULL)
        {
            if (strcmp("-d", second) == 0)
            {
                d = 1;
            }
            else if (strcmp("-f", second) == 0)
            {
                f = 1;
            }
            else if (strncmp(second, "\"", 1) != 0)
            {
                strcpy(target_dir, second);
                u = 1;
                second = strtok(NULL, delimiter);
                if (second == NULL)
                    break;
                else
                    continue;
            }
            // else
            // {
            //     strcpy(target_dir, second);
            // }
            else
            {
                strcpy(to_search, second);
                search = 1;
                if (!d && !f)
                {
                    c = 4; // when no flags are given and just the one to find is given
                    second = strtok(NULL, delimiter);
                    if (second == NULL)
                        break;
                    strcpy(target_dir, to_search);
                    u = 1;
                    while (second != NULL)
                    {
                        if (strcmp("-d", second) == 0)
                        {
                            search = 0;
                            d = 1;
                        }
                        else if (strcmp("-f", second) == 0)
                        {
                            search = 0;
                            f = 1;
                        }
                        else
                        {
                            strcpy(to_search, second);
                            search = 1;
                        }
                        second = strtok(NULL, delimiter);
                    }
                }
            }
            if (second == NULL)
                break;
            second = strtok(NULL, delimiter);
        }
        if (d == 1 && f == 1)
            c = 3;
        else if (d == 1)
            c = 1;
        else if (f == 1)
            c = 2;
        if (!u)
            strcpy(target_dir, ".");
        if ((c == 3 || c == 1 || c == 0) && search == 0)
            printf(".\n");
        char to_search1[200];
        if (search == 1)
        {
            // printf("%ld\n", strlen(to_search));
            int i;
            for (i = 1; i < strlen(to_search) - 1; ++i)
            {
                to_search1[i - 1] = to_search[i];
                // printf("%c", to_search1[i - 1]);
            }
            to_search1[i - 1] = '\0';
            // printf("\n%d\n", i);
            // printf("to_search: %s\n", to_search1);
            // printf("\n%ld\n", strlen(to_search1));

            strcpy(to_search, to_search1);
        }
        // printf("%s\n", to_search);
        // printf("Current path: %s\nto_search: %s\nFlag: %d\nAnything to search: %d\nTarget_dir: %s\n", cur_path, to_search, c, search, target_dir);
        discover_dir(target_dir, to_search, cur_path, c, search);
    }
}

void prompt()
{
    // char *inm = (char *)malloc(100 * sizeof(char));
    // // strcpy(inm, " took ");
    // // strcat(inm, tim);
    // // strcat(inm, "s");
    // sprintf(inm, " took %ds", tim);
    // printf("%s %s\n", inm, userid);
    // userid1 = (char *)malloc(1024 * sizeof(char));
    // if (strstr(userid, inm) != NULL)
    // {
    //     // printf("1\n");
    //     strncpy(userid1, userid, strlen(userid) - strlen(inm) - 1);
    //     // char *ints = (char *)malloc(10 * sizeof(char));
    //     // sprintf(ints, " took %ds", end - begin);
    //     // tim = end - begin;
    //     strcat(userid1, right_arrow);
    //     // strcat(userid1, right_arrow);
    //     strcpy(userid, userid1);
    // }
    printf("\033[0;31m"); // Set the text to the color red
    printf("%s", userid);
    printf("\033[0m"); // Resets the text to default color
}

int foreground(char *inp, int fl)
{
    if (strlen(inp) == 0)
        return 0;
    int ff = 0;
    for (int i = 0; i < strlen(inp); ++i)
    {
        if (inp[i] != ' ' && inp[i] != '\t')
        {
            ff = 1;
            break;
        }
    }
    if (ff == 0)
        return 0;
    int child_status;
    char *input_copy = (char *)malloc(500 * sizeof(char));
    strcpy(input_copy, inp);
    char *fir = strtok(input_copy, delimiter);
    int cnt = 0;
    while (fir != NULL)
    {
        cnt++;
        fir = strtok(NULL, delimiter);
    }
    strcpy(input_copy, inp);
    char *store[cnt + 1];
    fir = strtok(input_copy, delimiter);
    store[0] = (char *)malloc(500 * sizeof(char));
    strcpy(store[0], fir);
    fir = strtok(NULL, delimiter);
    for (int i = 1; i < cnt; ++i)
    {
        store[i] = (char *)malloc(500 * sizeof(char));
        strcpy(store[i], fir);
        fir = strtok(NULL, delimiter);
    }
    store[cnt] = NULL;
    pidt = fork();
    int begin = time(NULL);
    if (pidt == 0)
    {
        if (execvp(store[0], store) == -1)
        {
            // printf("1\n");
            perror("exec");
        }
    }
    else
    {
        if (wait(0) == -1)
        {
            perror("wait");
        }
        if (fl == 1)
            printf("Process ID: %d\n", pidt);
        int status;
        waitpid(pidt, &status, WCONTINUED);
        int end = time(NULL);
        if (fl == 1)
        {
            printf("sleep with pid = %d exited normally.\n", pidt);
        }
        if (fl == 0)
        {
            if (foregr != 0)
            {
                char *inm = (char *)malloc(100 * sizeof(char));
                sprintf(inm, " took %ds", tim);
                userid1 = (char *)malloc(1024 * sizeof(char));
                if (strstr(userid, inm) != NULL || dd == 0)
                {
                    dd = 1;
                    strncpy(userid1, userid, strlen(userid) - strlen(inm) - 1);
                    char *ints = (char *)malloc(10 * sizeof(char));
                    sprintf(ints, " took %ds", end - begin);
                    tim = end - begin;
                    strcat(userid1, ints);
                    strcat(userid1, right_arrow);
                    strcpy(userid, userid1);
                }
            }
            if (!foregr)
            {
                foregr = 1;
                char *inm = (char *)malloc(100 * sizeof(char));
                // sprintf(inm, " took %ds", tim);
                userid1 = (char *)malloc(1024 * sizeof(char));
                strncpy(userid1, userid, strlen(userid) - 1);
                char *ints = (char *)malloc(10 * sizeof(char));
                sprintf(ints, " took %ds", end - begin);
                tim = end - begin;
                strcat(userid1, ints);
                strcat(userid1, right_arrow);
                strcpy(userid, userid1);
            }
        }
    }
    return pidt;
}

void bg2(int cn, char *store[cn + 1], int i)
{
    pid_t pid = fork();
    // pid_t p1;
    int begin = time(NULL);
    if (pid == 0)
    {
        // p1 = getpid();
        if (execvp(store[0], store) == -1)
        {
            perror("exec");
        }
    }
    else
    {
        pid_st[i] = pid;
        printf("Process ID: %d\n", pid);
        // printf("%d\n",pid_st[i]);

        int status;
        int end = time(NULL);
    }
}

int *bg(int cn, char *inp[cn + 1])
{
    // printf("%d\n",cn);
    // for(int i = 0; i < cn; ++i)
    // {
    //     printf("%s\n", inp[i]);
    // }
    for (int i = 0; i < cn; ++i)
    {
        int child_status;
        char *input_copy = (char *)malloc(500 * sizeof(char));
        strcpy(input_copy, inp[i]);
        char *fir = strtok(input_copy, delimiter);
        int cnt = 0;
        while (fir != NULL)
        {
            cnt++;
            fir = strtok(NULL, delimiter);
        }
        strcpy(input_copy, inp[i]);
        char *store[cnt + 1];
        fir = strtok(input_copy, delimiter);
        store[0] = (char *)malloc(500 * sizeof(char));
        strcpy(store[0], fir);
        fir = strtok(NULL, delimiter);
        for (int i = 1; i < cnt; ++i)
        {
            store[i] = (char *)malloc(500 * sizeof(char));
            strcpy(store[i], fir);
            fir = strtok(NULL, delimiter);
        }
        store[cnt] = NULL;
        bg2(cn, store, i);
    }
    return pid_st;
}

int *background(char *inp)
{
    int tf = 0;
    for (int i = strlen(inp) - 1; i >= 0; --i)
    {
        if (inp[i] == ' ' || inp[i] == '\t')
            continue;
        else if (inp[i] == '&')
        {
            tf = 1;
        }
        else
            break;
    }
    char del[] = "&";
    char *inps = (char *)malloc(1024 * sizeof(char));
    strcpy(inps, inp);
    char *f = strtok(inps, del);
    char *g = strtok(NULL, del);
    cnn = 1;

    while (g != NULL)
    {
        cnn++;
        g = strtok(NULL, del);
    }
    strcpy(inps, inp);
    char *store[cnn + 1];
    g = strtok(inps, del);
    for (int i = 0; i < cnn; ++i)
    {
        store[i] = (char *)malloc(500 * sizeof(char));
        strcpy(store[i], g);
        g = strtok(NULL, del);
    }
    if (tf == 0)
    {
        // if (are_we_redirecting == 1)
        // {
        //     printf("1\n");
        //     dup2(original, STDOUT_FILENO);
        // }
        foreground(store[cnn - 1], 0);
        cnn--;
    }
    if (tf == 1)
    {
        // if (are_we_redirecting == 1)
        // {
        //     printf("1\n");
        //     dup2(original, STDOUT_FILENO);
        // }
        foregr == 1;
        char *inm = (char *)malloc(100 * sizeof(char));
        sprintf(inm, " took %ds", tim);
        // printf("%s\n", inm);
        userid1 = (char *)malloc(1024 * sizeof(char));
        if (strstr(userid, inm) != NULL)
        {
            strncpy(userid1, userid, strlen(userid) - strlen(inm) - 1);
            strcat(userid1, right_arrow);
            strcpy(userid, userid1);
        }
    }
    return bg(cnn, store);
}

void sigfn()
{
    // printf("%d\n", cnn);
    // for(int i = 0; i < cnn; ++i)
    // {
    //     printf("%d\n", pid_st[i]);
    // }
    for (int i = 0; i < 2000; ++i)
    {
        // printf("%d\n", pid_st[i]);
        if (pid_st[i] == 0)
            continue;
        int p = pid_st[i], status;
        if (waitpid(WAIT_ANY, &status, WNOHANG) == p)
        {
            if (WEXITSTATUS(status) == 0)
            {
                printf("\n%s with pid %d exited normally\n", "Process", p);
                // prompt();
                yy = 1;
                cnn--;
                // goto here;
            }
            else
            {
                printf("\n%s with pid %d exited abnormally\n", "Process", p);
                // prompt();
                yy = 1;
                cnn--;
                // goto here;
            }
            pid_st[i] = 0;
            // break;
            // printf("%d\n", arr[i]);
        }
        // printf("\n%s", userid);
    }
    return;
}

void ctrl_c(int signo)
{
    pid_t p = getpid();
    if (p != shellid)
        return;
    if (pidt != -1)
        kill(pidt, SIGINT);
    signal(SIGINT, ctrl_c);
}

int main()
{
    shellid = getpid();
    printf("\e[1;1H\e[2J");
    home_path = getcwd(NULL, 1024);
    char delimit[] = "\t\r\n\v\f ";
    struct utsname uts;
    if (uname(&uts) != 0)
        return EXIT_FAILURE;
    char *sysname;
    sysname = uts.nodename;
    userid = getlogin();

    str = (char *)malloc(100 * sizeof(char));
    str[0] = '<';
    str[1] = '\0';
    strcat(str, userid);
    strcpy(userid, str);
    strcat(userid, at);
    strcat(userid, sysname);
    strcat(userid, colon);

    initial = (char *)malloc(100 * sizeof(char));
    strcpy(initial, userid);
    strcat(userid, tilda);
    strcat(userid, right_arrow);
    prev_dir = (char *)malloc(1024 * sizeof(char));
    strcpy(prev_dir, userid);
    char *forhistory = (char *)malloc(200 * sizeof(char));
    strcpy(forhistory, home_path);
    strcat(forhistory, slash);
    strcat(forhistory, "history.txt");
    FILE *temp1 = fopen(forhistory, "a+");
    char *prev_command = (char *)malloc(100 * sizeof(char));
    char *curr_command = (char *)malloc(100 * sizeof(char));
    int his_cn = 0;
    char arr[200];
    char *aa = fgets(arr, 200, temp1);
    his_cn++;
    while (aa != NULL)
    {
        strcpy(prev_command, aa);
        aa = fgets(arr, 200, temp1);
        his_cn++;
    }
    his_cn--;
    // printf("%d\n", his_cn);
    FILE *temp = fopen(forhistory, "a+");
    int his_cnt = his_cn, his_flag = 0;

    for (int i = 0; i < 2000; ++i)
        pid_st[i] = 0;
    int a = 0;
    while (1)
    {
        signal(SIGINT, ctrl_c);
        if (are_we_redirecting == 1)
        {
            are_we_redirecting = 0;
            // printf("1\n");
            dup2(original, STDOUT_FILENO);
        }
        sigfn();
        prompt();

        FILE *his = fopen(forhistory, "a+");
        char *inputs = (char *)malloc(1024 * sizeof(char));
        char *input_copy = (char *)malloc(1024 * sizeof(char));
        char *input_copy2 = (char *)malloc(1024 * sizeof(char));
        // while(scanf("%[^\n]s", inputs) != EOF)
        // {}
        int scanned = scanf("%[^\n]s", inputs);
        // if(scanned == EOF)
        //     exit(0);
        getchar();
        strcpy(input_copy2, inputs);
        int y = 0, x = 0;
        for (int i = 0; i < strlen(input_copy2); ++i)
        {
            if (input_copy2[i] != ' ' && input_copy2[i] != '\t' && input_copy2[i] != ';')
            {
                y = 1;
            }
            else if (input_copy2[i] == ';')
                x = 1;
        }
        if (y == 0 && x == 0)
        {
            continue;
        }
        if (y == 0 && x == 1)
        {
            printf("bash: syntax error near unexpected token `;'\n");
            continue;
        }
        if (!his_flag)
        {
            strcpy(prev_command, inputs);
            fprintf(his, "%s\n", inputs);
            his_cnt++;
            his_flag = 1;
        }
        else
        {
            if (strcmp(prev_command, inputs) != 0)
            {
                if (his_cnt >= 20)
                {
                    char his1[20][200];
                    int his_c = 0;
                    fseek(his, 0, SEEK_SET);
                    char c = fgetc(his);
                    while (c != '\n')
                    {
                        if (feof(his))
                            break;
                        c = fgetc(his);
                    }
                    char t[100];
                    for (int i = 0; i < 19; ++i)
                    {
                        fgets(his1[i], 100, his);
                    }
                    fclose(temp);
                    temp = fopen(forhistory, "w");
                    fclose(temp);
                    his = fopen(forhistory, "w+");
                    for (int i = 0; i < 19; ++i)
                    {
                        fprintf(his, "%s", his1[i]);
                    }
                    fprintf(his, "%s\n", inputs);
                }
                else
                {
                    his_cnt++;
                    fprintf(his, "%s\n", inputs);
                }
                strcpy(prev_command, inputs);
            }
        }
        strcpy(input_copy, inputs);
        strcpy(input_copy2, inputs);
        char del[] = ";";
        char *d = strtok(input_copy2, del);
        int cntt = 0;
        while (d != NULL)
        {
            cntt++;
            d = strtok(NULL, del);
        }
        char *store[cntt + 1];
        strcpy(input_copy2, inputs);
        char *fir = strtok(input_copy2, del);
        store[0] = (char *)malloc(500 * sizeof(char));
        strcpy(store[0], fir);
        fir = strtok(NULL, del);
        for (int i = 1; i < cntt; ++i)
        {
            store[i] = (char *)malloc(500 * sizeof(char));
            strcpy(store[i], fir);
            fir = strtok(NULL, del);
        }
        int space = 0;
        int cnt = 1;
        for (int i = 0; i < cntt; ++i)
        {
            char *inputy = (char *)malloc(500 * sizeof(char));
            strcpy(inputy, store[i]);
            char *f = strtok(inputy, "&");
            char *g = strtok(NULL, "&");
            // printf("%d\n", cntt);
            char *input = (char *)malloc(500 * sizeof(char));
            strcpy(input, store[i]);
            char *inpcopy = (char *)malloc(1024 * sizeof(char));
            char *redirected_file = (char *)malloc(1024 * sizeof(char));
            int v = 0, w = 0;
            int redirect = 0;
            int doubley = 0;

            int flag_left = 0, flag_doubleright = 0, flag_singleright = 0;
            for (int i = 0; i < strlen(input); ++i)
            {
                if (input[i] == '<')
                    flag_left = 1;
                else if (input[i] == '>')
                {
                    // printf("44\n");
                    i += 1;
                    if (input[i] == '>')
                    {
                        flag_doubleright = 1;
                        continue;
                    }
                    else
                        flag_singleright = 1;
                }
            }
            char the_first_command[100];
            int b = 0, spacee = 0;
            for (int i = 0; i < strlen(input); ++i)
            {
                if (spacee == 0 && (input[i] == ' ' || input[i] == '\t'))
                {
                    spacee = 1;
                    continue;
                }

                else if (input[i] == '<' || input[i] == '>')
                    break;
                spacee = 1;
                the_first_command[b++] = input[i];
            }
            the_first_command[b - 1] = '\0';
            // printf("%s\n", the_first_command);
            char file_to_copy_to[500];
            int vv = 0;
            spacee = 0;
            if (strncmp(the_first_command, "echo", 4) == 0 || strncmp(the_first_command, "pwd", 4) == 0 || strncmp(the_first_command, "pinfo", 5) == 0 || strncmp(the_first_command, "history", 7) == 0 || strncmp(the_first_command, "discover", 8) == 0 || strncmp(the_first_command, "ls", 2) == 0)
            {
                if (flag_singleright == 1 || flag_doubleright == 1)
                {
                    for (int i = 0; i < strlen(input); ++i)
                    {
                        if (spacee == 0 && (input[i] == ' ' || input[i] == '\t'))
                            continue;
                        if (input[i++] == '>')
                        {
                            spacee = 1;
                            if (flag_doubleright == 1)
                                i++;
                            for (; i < strlen(input); ++i)
                            {
                                if (input[i] == ' ' || input[i] == '\t')
                                    continue;
                                file_to_copy_to[vv++] = input[i];
                            }
                            file_to_copy_to[vv] = '\0';
                            // printf("%s\n", file_to_copy_to);
                        }
                    }
                    int file, file2;
                    // printf("%d %d\n", flag_doubleright, flag_singleright);
                    dup2(STDOUT_FILENO, original);
                    if (flag_singleright == 1 && flag_doubleright == 0)
                    {
                        FILE *t = fopen(file_to_copy_to, "w+");
                        fclose(t);
                        file = open(file_to_copy_to, O_CREAT | O_WRONLY, 0644);
                        if (file < 0)
                        {
                            perror("Failed to open file.");
                            continue;
                        }
                        file2 = dup2(file, STDOUT_FILENO);
                    }
                    else if (flag_doubleright == 1)
                    {
                        // printf("1\n");
                        // printf("%s\n", file_to_copy_to);
                        file = open(file_to_copy_to, O_CREAT | O_APPEND, 0644);
                        // fseek(his, 0, SEEK_END);
                        // printf("%d\n", file);
                        if (file < 0)
                        {
                            perror("Failed to open file.");
                            continue;
                        }
                        file2 = dup2(file, STDOUT_FILENO);
                    }
                    // printf("%d\n", file);
                    // printf("1\n");
                    // printf("%d %s\n", file, file_to_copy_to);
                    if (file2 < 0)
                    {
                        perror("Failed to open file.");
                        continue;
                    }
                    // if (strcmp(the_first_command, "ls") == 0)
                    // {
                    //     ls(input, flag, til);
                    // }
                    // printf("1->%s\n", the_first_command);
                    if (strncmp(the_first_command, "echo", 4) == 0)
                    {
                        echo(the_first_command);
                    }
                    else if (strncmp(the_first_command, "pwd", 3) == 0)
                        pwd();
                    else if (strncmp(the_first_command, "cd", 2) == 0)
                    {
                        cd(the_first_command);
                    }
                    else if (strncmp(the_first_command, "pinfo", 5) == 0)
                    {
                        pinfo(the_first_command);
                    }
                    else if (strncmp(the_first_command, "discover", 8) == 0)
                    {
                        discover(the_first_command);
                    }
                    dup2(original, STDOUT_FILENO);
                    continue;
                }
            }

            // if(strncmp(the_first_command, "cat", 3)==0 || strncmp(the_first_command, "sort")==0)
            // {

            // }
            if (strncmp(the_first_command, "cat", 3) == 0 || strncmp(the_first_command, "sort", 4) == 0)
            {
                for (int i = 0; i < strlen(input); ++i)
                {
                    if (input[i] == '<')
                        continue;
                    if (input[i] == '>')
                    {
                        if (input[i + 1] == '>')
                        {
                            // printf("1\n");
                            doubley = 1;
                            // i += 3;
                        }
                        redirect = 1;

                        for (; i < strlen(input); ++i)
                        {
                            // printf("1");
                            redirected_file[w++] = input[i];
                        }
                        // printf("\n");
                        redirected_file[w++] = '\0';
                    }
                    if (i < strlen(input))
                        inpcopy[v++] = input[i];
                }
                inpcopy[v++] = '\0';
                // printf("%s\n", inpcopy);

                dup2(STDOUT_FILENO, original);
                // printf("%d\n", original);
                // printf("%s\n", redirected_file);
                if (redirect == 1)
                {
                    // printf("2\n");
                    redirect = 0;
                    int file;
                    are_we_redirecting = 1;
                    if (doubley == 1)
                    {
                        // printf("1\n");
                        file = open(redirected_file, O_CREAT | O_APPEND, 0644);
                        // printf("%d\n", file);
                    }
                    else
                    {
                        file = open(redirected_file, O_CREAT | O_WRONLY, 0644);
                    }
                    if (file < 0)
                    {
                        perror("Failed to open file.");
                        continue;
                    }
                    // printf("%d\n", file);
                    int file2 = dup2(file, STDOUT_FILENO);
                    if (file2 < 0)
                    {
                        // printf("1\n");
                        perror("Failed to open file.");
                        continue;
                    }
                    // printf("1\n");
                    background(inpcopy);

                    // close(file);
                    // close(file2);
                }
                else
                {
                    background(inpcopy);
                }
                continue;
            }
            dup2(original, STDOUT_FILENO);
            strcpy(inputy, store[i]);
            if (g == NULL && inputy[strlen(inputy) - 1] == '&')
            {
                // printf("1\n");
                background(f);
            }
            else if (g != NULL)
            {
                int *arr;
                arr = background(store[i]);
                continue;
            }
            // char *input = (char *)malloc(500 * sizeof(char));
            strcpy(input, store[i]);

            int a = 0, l = 0, la = 0, til = 0, _ = 0;

            char copied[1024];
            strcpy(copied, input);
            char *add = strtok(copied, delimit);
            char *second = strtok(NULL, delimiter);
            while (second != NULL)
            {
                if (!a && strcmp("-a", second) == 0)
                {
                    a = 1;
                }
                else if (!l && strcmp("-l", second) == 0)
                {
                    l = 1;
                }
                else if (!la && (strcmp("-al", second) == 0 || strcmp("-la", second) == 0))
                {
                    la = 1;
                }
                else if (!_ && (strcmp("~", second) == 0))
                {
                    til = 1;
                }
                second = strtok(NULL, delimit);
            }
            int flag = 0;
            if ((a == 1 && l == 1) || la == 1)
                flag = 3;
            else if (a == 1)
                flag = 1;
            else if (l == 1)
                flag = 2;
            // printf("%s\n", add);
            if (strcmp(add, "ls") == 0)
            {
                ls(input, flag, til);
            }
            else if (strcmp(add, "echo") == 0)
            {
                echo(input);
            }
            else if (strcmp(add, "pwd") == 0)
                pwd();
            else if (strcmp(add, "cd") == 0)
            {
                cd(input);
            }
            else if (strcmp(add, "history") == 0)
            {
                // history(his);
                fseek(his, 0, SEEK_SET);
                int cnt = 0;
                char a[200];

                int hh = 0, gg = his_cnt;
                if (his_cnt >= 10)
                {
                    hh = his_cnt - 10;
                    gg = 10;
                }

                while (cnt != hh)
                {
                    char c = fgetc(his);
                    if (feof(his))
                        break;
                    // printf("%c ", c);
                    if (c == '\n')
                        cnt++;
                }
                for (int i = 0; i < gg; ++i)
                {
                    // printf("1");
                    fgets(a, 200, his);
                    printf("%s", a);
                }
            }
            else if (strcmp(add, "pinfo") == 0)
            {
                pinfo(input);
            }
            else if (strcmp(add, "discover") == 0)
            {
                discover(input);
            }
            else if (strcmp(add, "-1") == 0)
            {
                return 0;
            }
            else if (1)
            {
                foreground(input, 0);
            }
            else
            {
                printf(RED "Invalid Command.\n" RESET);
            }
        }

        fclose(his);
        // free(input);
    }
}
