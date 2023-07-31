#include "headers.h"
#include "prompt.h"
void make_history(char *command0)
{
    char* history[30];
    // Opening file in reading mode
    int front =0;
    char* buffer=strdup(home);
    strcat(buffer,"/history.txt");
    ptr = fopen(buffer, "r");
    if (ptr==NULL)
    {
        printf("file can't be opened \n");
        return;
    }
    char *buff = (char *)malloc(sizeof(char) * 100);
    long long int n = 0;
    size_t len;
    while (getline(&buff, &len, ptr) != -1)
    {
        if(strcmp(buff,"\n")!=0 )
        {
            history[n] = (char *)malloc(sizeof(char) * 100);
            strcpy(history[n++], buff);
        }
    }
    if(n>20)
        front+=n-20;
    fclose(ptr);
    //printf("%lld", n);
    ptr1 = fopen("history.txt", "w");
    if (n == 0)
    {
        history[n] = (char *)malloc(sizeof(char) * 100);
        strcpy(history[n++], command0);
    }
    else
    {
        //strcat(command0,"\n");
        char temp[500];
        strcpy(temp,command0);
        strcat(temp,"\n");
        if (strcmp(history[n - 1],temp) != 0)
        {
            history[n] = (char *)malloc(sizeof(char) * 100);
            strcpy(history[n++], temp);
        }
    }
    // for(long long int i=0;i<n;i++)
    //     if(strcmp(history[i],"\n")!=0)
    //         printf("%s",history[i]);
    // printf("\n");
    for(long long int i=front;i<n;i++)
        fprintf(ptr1,"%s\n",history[i]);
    fclose(ptr1);
}
void history_print()
{
    char* history[30];
    ptr = fopen("history.txt", "r");
    if (NULL == ptr)
    {
        printf("file can't be opened \n");
    }
    char *buff = (char *)malloc(sizeof(char) * 100);
    long long int n = 0;
    size_t len;
    while (getline(&buff, &len, ptr) != -1)
    {
        if(strcmp(buff,"\n")!=0)
        {
            history[n] = (char *)malloc(sizeof(char) * 100);
            strcpy(history[n++], buff);
        }
    }
    fclose(ptr);
    for(int i=n-10>0?n-10:0;i<=n-1;i++)
        printf("%s",history[i]);
    printf("\n");
    //long long int i = 0;
    // while (history[i] != NULL && i <= 10)
    //     printf("%s\n", history[i++]);
}