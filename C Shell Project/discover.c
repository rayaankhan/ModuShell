#include "headers.h"
#include "prompt.h"
long long int find = -1;
int isDir1(const char *fileName)
{
    struct stat path;

    stat(fileName, &path);

    return S_ISREG(path.st_mode);
}
void discover_1(char *Dirct)
{
    char path[1000];
    DIR *dh = opendir(Dirct);
    struct dirent *FileList;
    if (!dh)
        return;
    while ((FileList = readdir(dh)) != NULL)
    {
        if (FileList->d_name[0] != '.')
        {
            // printf("%s\n", FileList->d_name);
            // Construct new path from our base path
            strcpy(path, Dirct);
            strcat(path, "/");
            strcat(path, FileList->d_name);
            //if(strcmp(FileList->d_name,token[find])==0)
            struct stat buffer;
            //DIR *dh1 = opendir(FileList->d_name);
            if (find == -1)
            {
                if (f_present == 0 && d_present == 0 || f_present == 1 && d_present == 1)
                    printf("%s\n", path);
                else if (isDir1(FileList->d_name) == 0)
                {
                    if (d_present == 1)
                        printf("%s\n", path);
                }
                else
                {
                    if (f_present == 1)
                        printf("%s\n", path);
                }
            }
            else
            {
                //printf("%s\n",token[find]);
                if(strcmp(FileList->d_name,token[find])==0)
                {
                    if (f_present == 0 && d_present == 0 || f_present == 1 && d_present == 1)
                    printf("%s\n", path);
                    else if (isDir1(FileList->d_name) == 0)
                    {
                        if (d_present == 1)
                            printf("%s\n", path);
                    }
                    else
                    {
                        if (f_present == 1)
                            printf("%s\n", path);
                    }
                }
            }
            // if(f_present==1 && stat (FileList->d_name, &buffer) == 0 && !dh)
            //     printf("%s\n", path);
            // if(d_present==1 && stat (FileList->d_name, &buffer) != 0 && isDir1(FileList->d_name)==0)
            //     printf("%s\n", path);
            discover_1(path);
        }
    }
    closedir(dh);
}
void discover_2(long long int no_of_flag_and_dir)
{
    char *target = strdup(".");
    for (long long int i = 1; i < no_of_flag_and_dir; i++)
    {
        if (token[i][0] != '-')
        {
            if (token[i][0] != '"')
            {
                target = strdup(token[i]);
            }
            else
            {
                find = i;
                //strtok(token[find], "\"");
            }
        }
    }
    if(find != -1)
    {
        strtok(token[find],"\"");
        token[find]=strtok(token[find],"\"");
        //printf("%s\n",token[find]);
    }
    //printf("%s\n",target);
    if((f_present==0 && d_present==0 || f_present==1 && d_present==1 || f_present==0 && d_present==1 ) && find==-1)
        printf("%s\n",target);
    discover_1(target);
        //discover_3(target);
}
void discover()
{
    f_present = 0;
    d_present = 0;
    long long int i = 1;
    token[1] = strtok(NULL, " \n\t");
    if (token[1] == NULL)
    {
        printf(".\n");
        discover_1(".");
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
                    if (flg == 'f')
                        f_present = 1;
                    else if (flg == 'd')
                        d_present = 1;
                    else
                        printf("'-%c' flag not found\n", flg);
                }
                else
                {
                    printf("\033[1;31m'%s' flag not found\n\033[0m", token[i]);
                    return;
                }
            }
            token[++i] = strtok(NULL, " \n\t");
            if (token[i] == NULL)
            {
                break;
            }
        }
        discover_2(i);
    }
}
/*
**File

*/