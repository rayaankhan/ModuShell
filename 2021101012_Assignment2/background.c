#include "prompt.h"
#include "headers.h"
void background()
{
    for (long long int i = 0; i < bg_count; i++)
    {
        pid_t pid = fork();
        pid_t p;
        time_t t;
        char *duff[4];
        for (int j = 0; j < 4; j++)
            duff[j] = (char *)malloc(sizeof(char) * 1000);
        duff[0] = strtok(token_and[i], " \n\t");
        duff[1] = strtok(NULL, " \n\t");
        duff[2] = NULL;
        //token_and[i] = strtok(NULL, " \n\t");
        if (pid == -1)
            perror("Error: Fork Failed\n");
        else if (pid == 0)
        {
            //p = getpid();
            //pid_store[i]=p;
            //printf("[%lld] %d\n", i + 1, pid_store[i]);
            int f = execvp(duff[0], duff);
            //sleep(atoi(token[1]));
            if (f < 0)
            {
                perror("\033[1;31mError: exec failed");
                printf("Invalid command\n");
                exit(1);
            }
            
            exit(0);
        }
        else
        {
            pid_store[i]=pid;
            bg_names[i]=strdup(duff[0]);
            if(duff[1]!=NULL)
            bg_time[i]=atoi(duff[1]);
            child_count++;
            printf("[%lld] %d\n", child_count, pid_store[i]);
        }
    }
}
