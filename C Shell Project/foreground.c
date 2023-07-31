#include "prompt.h"
#include "headers.h"

void foreground()
{
    char* duff[4];
    for(int i=0;i<4;i++)
    duff[i]=(char*)malloc(sizeof(char)*1000);
    //char* duff1=strdup(token_and[bg_count]);
    pid_t pid = fork();
    if(bg_count==0)
    token[1] = strtok(NULL, " \n\t");
    else
    {
        duff[0]=strtok(token_and[bg_count]," \n\t");
        duff[1]=strtok(NULL," \n\t");
    }
    duff[2]=NULL;
    // if(token[1]==NULL)
    // {
    //     printf("Invalid command\n");
    //     exit(0)
    // }
    // printf("%s\n",duff[0]);
    // printf("%s\n",duff[1]);
    if (pid == -1)
        perror("Error: Fork Failed\n");
    else if (pid == 0)
    {
        int f = bg_count==0 ? execvp(token[0], token):execvp(duff[0],duff);
        //sleep(atoi(token[1]));
        if (f < 0)
        {
            perror("\033[1;31mError: exec failed");
            printf("Invalid command\n");
            exit(0);
        }
        exit(0);
    }
    else
    {
        int status;
        waitpid(pid, &status, WUNTRACED);
    }
}