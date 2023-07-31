#include "headers.h"
#include "pinfo.h"
void pinfo()
{
    long long int memory;
    char buf1[10000];
    char buf2[10000];
    char filename[10000];
    char status;
    token[1] = strtok(NULL, " \n\t");
    //Storing process ID
    pid_t pid;
    pid=(token[1] != NULL)?atoi(token[1]):getpid();
    // if (token[1] != NULL)
    //     pid = atoi(token[1]); 
    // else
    //     pid = getpid();
    token[2] = strtok(NULL, " \n\t");
    // More than two argumnets
    if(token[2]!=NULL)
    {
        printf("Error: Extra Argumnets\n");
        return;
    }
    //Getting the process file which has information about it
    sprintf(filename, "/proc/%d/stat", pid);
    // Opening the file and checking errors
    FILE* f = fopen(filename, "r");
    if (f==NULL)
        perror("\033[1;31mError: no such process exists\n");
    else
    {
        long long int x,y;
        fscanf(f, "%*d %*s %c %*d %lld %*d %*d %lld %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %lld %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d", &status,&x,&y, &memory);
        fclose(f);
        printf("PID : %d\n", pid);
        // Checking if it is a '+' or not
        if(x==y)
            printf("Process Status : %c+\n", status);
        else
            printf("Process Status : %c\n", status);
        printf("Memory : %lld\n", memory);
        // For executable path
        sprintf(filename, "/proc/%d/exe", pid); 
        //printf("%s\n", filename);
        //Creating path of the executable file
        long long int x1 = readlink(filename, buf1, sizeof(buf1) - 1);
        // Check if address is same as home address
        if (strcmp(buf1, home) == 0)
            strcpy(buf1, "~");
        else
        {
            long long int ii = 0;
            if(strncmp(home,buf1,strlen(home))==0)
            {
                buf2[0] = '~';
                long long int j=ii;
                for(long long int i=1;buf1[j] != '\0';i++,j++)
                    buf2[i] = buf1[j];
                buf2[j] = '\0';
            }
            else
                strcpy(buf2, buf1);
        }
        printf("Executable Path : %s\n", buf2);
    }
}