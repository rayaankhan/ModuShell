#include "prompt.h"
#include "headers.h"

int InitialState = 0;
void prompt()
{
    //User_name
    getlogin_r(User_name, sizeof(User_name));      
    //System_name
    gethostname(System_name, sizeof(System_name)); 
    if (InitialState == 0)
    {
        //Storing the path of the folder in which our code is as home
        getcwd(home, sizeof(home));
        InitialState = 1;
    }
    //Getting the current directory path
    getcwd(cwd, sizeof(cwd));
    //printf("%s\n",cwd);
    if (strcmp(home, cwd) == 0)
    {
        // If cwd is home
        if(fg_present)
        {
            printf("\033[0;35m<%s@%s:\033[0m\033[1;33m%c took %llds\033[0;35m>\033[0m  ", User_name, System_name, '~',clock_time);
            fg_present=0;
        }
        else
            printf("\033[0;35m<%s@%s:\033[0m\033[1;33m%c\033[0;35m>\033[0m  ", User_name, System_name, '~');
    }
    else
    {
        // If cwd is inside home
        if (strncmp(cwd, home, strlen(home)) == 0)
        {
            long long int l = strlen(home);
            long long int r = strlen(cwd);
            for (long long int i = 0; i < r - l; i++)
            {
                Relative_path[i] = cwd[l + i];
            }
            if(fg_present)
            {
                printf("\033[0;35m<%s@%s:\033[0m\033[1;33m%c%s took %llds\033[0;35m>\033[0m  ", User_name, System_name, '~', Relative_path,clock_time);
                fg_present=0;
            }
            else
            printf("\033[0;35m<%s@%s:\033[0m\033[1;33m%c%s\033[0;35m>\033[0m  ", User_name, System_name, '~', Relative_path);
        }
        // If cwd is outside home
        else
        {
            if(fg_present)
            {
                printf("\033[0;35m<%s@%s:\033[0m\033[1;33m%s took %llds\033[0;35m>\033[0m  ", User_name, System_name,cwd,clock_time);
                fg_present=0;
            }
            //printf("hi\n");
            printf("\033[0;35m<%s@%s:\033[0m\033[1;33m%s\033[0;35m>\033[0m  ", User_name, System_name, cwd);
        }
        // Clearing the Relative_path
        memset(Relative_path, '\0', sizeof(char) * 100000);
    }
}
//=================================================================================================================================
// function for echo command
void echo()
{
    char *token1 = strtok(NULL, " \t");
    while (token1 != NULL)
    {
        printf("%s ", token1);
        token1 = strtok(NULL, " \t");
    }
    printf("\n");
}
//=================================================================================================================================
// function for cd command
void cd()
{
    char curr[1000];
    token[1] = strtok(NULL, " \n\t");
    // If we have only cd command
    if (token[1] == NULL)
    {
        getcwd(Previous_directory, sizeof(Previous_directory));
        chdir(home);
        getcwd(curr, sizeof(curr));
        //printf("%s\n",curr);
        strcpy(Current_directory, curr);
    }
    //token[1] = strtok(NULL, " \n\t");
    // if we have cd ~ command
    else if (strcmp(token[1], "~") == 0)
    {
        getcwd(Previous_directory, sizeof(Previous_directory));
        chdir(home);
        getcwd(curr, sizeof(curr));
        //printf("%s\n",curr);
        strcpy(Current_directory, curr);
    }
    // if we have cd - command
    else if (strcmp(token[1], "-") == 0)
    {
        chdir(Previous_directory);
        printf("%s\n", Previous_directory);
        strcpy(Previous_directory, Current_directory);
        getcwd(Current_directory, sizeof(Current_directory));
    }
    // if we have cd <directory> command
    else
    {
        int xx = 0;
        if (token[1][0] == '~')
        {
            char t1[10000] = "";
            for (long long int i = 1; token[1][i] != '\0'; i++)
                t1[i - 1] = token[1][i];
            //printf("%s\n",t1);
            for (long long int i = 0; i < strlen(token[1]); i++)
                token[1][i] = '\0';
            long long int i = 0;
            for (; i < strlen(home); i++)
                token[1][i] = home[i];
            token[1][i] = '\0';
            // printf("%d\n",strlen(token[1]));
            // printf("%s\n",home);
            strcat(token[1], t1);
            // printf("%s\n",token[1]);
        }
        DIR *check = opendir(token[1]);
        if (!check)
        {
            if (errno == ENOENT)
                perror("\033[1;31mDirectory does not exist");
            else
                perror("\033[1;31mUnable to read directory");
            free(check);
            xx = 1;
        }
        if (xx == 0)
        {
            getcwd(Previous_directory, sizeof(Previous_directory));
            chdir(token[1]);
            getcwd(curr, sizeof(curr));
            //printf("%s\n",curr);
            strcpy(Current_directory, curr);
            //printf("%s\n",token[1]);
        }
    } 
    // }
}
//=================================================================================================================================
// function for pwd command
void pwd()
{
    getcwd(cwd, sizeof(cwd));
    printf("%s\n", cwd);
}
//=================================================================================================================================
//Checking if it is a Directory or not
int isDir(const char *fileName)
{
    struct stat path;

    stat(fileName, &path);

    return S_ISREG(path.st_mode);
}
// Defining comparator function as per the requirement
static int Compare(const void *a, const void *b)
{
    // setting up rules for comparison
    return strcasecmp(*(const char **)a, *(const char **)b);
}
// Function to sort the array
void Sort_list(char **arr, long long int ss)
{
    qsort(arr, ss, sizeof(const char *), Compare);
}
//Function to print time in ls -l
void time_print(char *Dirct)
{
    void dump_tm(struct tm * t);
    time_t now = time(NULL);
    struct stat statf;
    struct tm time_of_file; 
    struct tm time_now;
    stat(Dirct, &statf);
    localtime_r(&statf.st_mtime, &time_of_file); /* get struct tm for file */
    localtime_r(&now, &time_now);          /* and now */
    char time_string[64] = "";
    if (time_of_file.tm_year == time_now.tm_year)
    {                                                                 /* compare year values  */
        strftime(time_string, sizeof(time_string), "%b %e %H:%M", &time_of_file); /* if year is current output date/time  */
    }
    else
    { /* if year is not current, output time/year */
        strftime(time_string, sizeof(time_string), "%b %e  %Y", &time_of_file);
    }
    char *c = time_string;
    printf("%s ", c);
}
//Printing all the directories and files in given directory according to the flags given
void ls_part_11(char *Dirct)
{
    struct stat sdir;
    stat(Dirct, &sdir);
    if (!a_present && Dirct[0] == '.')
        return;
    else
    {
        if (l_present == 1)
        {
            off_t size = sdir.st_size;
            if (isDir(Dirct) == 0)
                Permissions[0] = 'd';
            else
                Permissions[0] = '-';
            //Owner permissions:
            Permissions[1] = (sdir.st_mode & S_IRUSR) ? 'r' : '-';
            Permissions[2] = (sdir.st_mode & S_IWUSR) ? 'w' : '-';
            Permissions[3] = (sdir.st_mode & S_IXUSR) ? 'x' : '-';
            //Group permissions:
            Permissions[4] = (sdir.st_mode & S_IRGRP) ? 'r' : '-';
            Permissions[5] = (sdir.st_mode & S_IWGRP) ? 'w' : '-';
            Permissions[6] = (sdir.st_mode & S_IXGRP) ? 'x' : '-';
            //Others permissions:
            Permissions[7] = (sdir.st_mode & S_IROTH) ? 'r' : '-';
            Permissions[8] = (sdir.st_mode & S_IWOTH) ? 'w' : '-';
            Permissions[9] = (sdir.st_mode & S_IXOTH) ? 'x' : '-';
            Permissions[10] = '\0';
            //DIR *dh = opendir(Dirct);
            //printf("hi\n");
            if (Permissions[0] == 'd')
            {
                //printf("hi1\n");
                printf("%s  %hu  %s  %s  %lld \t", Permissions, sdir.st_nlink, User_name, System_name, size);
                time_print(Dirct);
                printf("\033[1;36m%s\n\033[0m", Dirct);
            }
            else
            {
                if (sdir.st_mode & S_IXOTH)
                {
                    printf("%s  %hu  %s  %s  %lld \t", Permissions, sdir.st_nlink, User_name, System_name, size);
                    time_print(Dirct);
                    printf("\033[1;32m%s\n\033[0m", Dirct);
                }
                else
                {
                    printf("%s  %hu  %s  %s  %lld \t", Permissions, sdir.st_nlink, User_name, System_name, size);
                    time_print(Dirct);
                    printf("%s\n", Dirct);
                }
            }
        }
        else
        {
            if (isDir(Dirct) == 0)
                printf("\033[1;36m%s\n\033[0m", Dirct);
            else
            {
                if (sdir.st_mode & S_IXOTH)
                    printf("\033[1;32m%s\n\033[0m", Dirct);
                else
                    printf("%s\n", Dirct);
            }
        }
    }
    return;
}
// Checking if the file exists or not and print respective permissions etc
void Check_If_File(char *Dirct)
{
    struct stat sfile;
    if (stat(Dirct, &sfile) == -1)
    {
        perror("Directory/file not found");
        return;
    }
    else
    {
        if (l_present == 1)
        {
            off_t size = sfile.st_size;
            Permissions[0] = '-';
            //Owner permissions:
            Permissions[1] = (sfile.st_mode & S_IRUSR) ? 'r' : '-';
            Permissions[2] = (sfile.st_mode & S_IWUSR) ? 'w' : '-';
            Permissions[3] = (sfile.st_mode & S_IXUSR) ? 'x' : '-';
            //Group permissions:
            Permissions[4] = (sfile.st_mode & S_IRGRP) ? 'r' : '-';
            Permissions[5] = (sfile.st_mode & S_IWGRP) ? 'w' : '-';
            Permissions[6] = (sfile.st_mode & S_IXGRP) ? 'x' : '-';
            //Others permissions:
            Permissions[7] = (sfile.st_mode & S_IROTH) ? 'r' : '-';
            Permissions[8] = (sfile.st_mode & S_IWOTH) ? 'w' : '-';
            Permissions[9] = (sfile.st_mode & S_IXOTH) ? 'x' : '-';
            Permissions[10] = '\0';
            if (Permissions[9] == 'x')
            {
                printf("%s  %hu  %s  %s  %lld \t", Permissions, sfile.st_nlink, User_name, System_name, size);
                time_print(Dirct);
                printf("\033[1;32m%s\n\033[0m", Dirct);
            }
            else
            {
                printf("%s  %hu  %s  %s  %lld \t", Permissions, sfile.st_nlink, User_name, System_name, size);
                time_print(Dirct);
                printf("%s\n", Dirct);
            }
        }
        else
        {
            if (sfile.st_mode & S_IXOTH)
                printf("\033[1;32m%s\n\033[0m", Dirct);
            else
                printf("%s\n", Dirct);
        }
        return;
    }
}
//Moving inside the given directory and and calculating total for ls
int ls_part_10(char *Dirct)
{
    DIR *dh = opendir(Dirct);
    if (!dh)
    {
        if (errno != ENOENT)
        {
            Check_If_File(Dirct);
        }
        else
        {
            //error checking
            perror("\033[1;31mUnable to read directory");
            //return 0;
        }
        free(dh);
        return 0;
    }
    else
    {
        //printf("%s:\n", Dirct);
        struct dirent *FileList;
        struct stat fileStat;
        long long int size = 0;
        no_of_dir = 0;
        // DIR *dh = opendir(Dirct);
        while ((FileList = readdir(dh)) != NULL)
        {
            Directory_list[no_of_dir++] = FileList->d_name;
            stat(FileList->d_name, &fileStat);
            size += fileStat.st_blocks;
        }
        if(l_present==1)
            printf("Total:%lld\n", size);
        return 1;
    }
}
// calls ls_part_11 function
void ls_part_11_caller()
{
    for (long long int i = 0; i < no_of_dir; i++)
    {
        ls_part_11(Directory_list[i]);
    }
}
// calls ls_part_10 function
void ls_part_10_caller(long long int no_of_flag_and_dir)
{
    char *SortIt[1000];
    total = 0;
    int n = 0;
    for (long long int i = 1; i < no_of_flag_and_dir; i++)
    {
        if (token[i][0] != '-')
        {
            n = 1;
            SortIt[total++] = token[i];
            // if (ls_part_10(token[i]))
            // {
            //     Sort_list();
            //     ls_part_11_caller();
            // }
            //printf("\n");
        }
    }
    if (n == 0)
    {
        ls_part_10(".");
        Sort_list(Directory_list, no_of_dir);
        ls_part_11_caller();
    }
    else
    {
        Sort_list(SortIt, total);
        if (total > 1)
        {
            for (long long int i = 0; i < total; i++)
            {
                printf("%s :\n",SortIt[i]);
                if (ls_part_10(SortIt[i]))
                {
                    Sort_list(Directory_list, no_of_dir);
                    ls_part_11_caller();
                }
                printf("\n");
            }
        }
        else
        {
            if(ls_part_10(SortIt[0]))
                ls_part_11_caller();
        }
    }
    return;
}
// Main ls function which sets flags and stores all tokens then call the apropriate function
void ls()
{
    a_present = 0;
    l_present = 0;
    long long int i = 1;
    token[1] = strtok(NULL, " \n\t");
    if (token[1] == NULL)
    {
        ls_part_10(".");
        Sort_list(Directory_list, no_of_dir);
        ls_part_11_caller();
    }
    else
    {
        while (1)
        {
            if (token[i][0] == '-')
            {
                if (token[i][2] == '\0')
                {
                    char flg = token[i][1];
                    if (flg == 'a')
                        a_present = 1;
                    else if (flg == 'l')
                        l_present = 1;
                    else
                        printf("'-%c' flag not found\n", flg);
                }
                else
                {
                    if ((strcmp(token[i], "-al") && strcmp(token[i], "-la")) == 0)
                    {
                        a_present = 1;
                        l_present = 1;
                    }
                    else
                    {
                        printf("'%s' flag not found\n", token[i]);
                    }
                }
            }
            
            token[++i] = strtok(NULL, " \n\t");
            if (token[i] == NULL)
            {
                break;
            }
        }
        ls_part_10_caller(i);
    }
}
