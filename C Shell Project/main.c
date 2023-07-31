#include "prompt.h"
#include "headers.h"
#include "pinfo.h"
void Sort_bg()
{
    char temp1[1000];
    for(int i=0;i<1000-1;i++)
    {
        for(int j=i+1;j<1000 && bg_names[j]!=NULL;j++)
        {
            if(bg_time[j]<bg_time[i])
            {
                int temp=bg_time[j];
                bg_time[j]=bg_time[i];
                bg_time[i]=temp;
                strcpy(temp1,bg_names[j]);
                strcpy(bg_names[j],bg_names[i]);
                strcpy(bg_names[i],temp1);
                pid_t temp2=pid_store[j];
                pid_store[j]=pid_store[i];
                pid_store[i]=temp2;
            }
        }
    }
}
void sig_func(int signal)
{
    for (int i=0;i<1000;i++)
    {
        if (!pid_store[i])
            continue;
        int status,p=pid_store[i];
        if (waitpid(-1, &status, WNOHANG)==p)
        {
            if (WEXITSTATUS(status)==0)
                printf("%s with pid = %d exited normally\n",bg_names[i],pid_store[i]);
            else
                printf("%s with pid = %d exited abnorrmally\n",bg_names[i],pid_store[i]);
            pid_store[i]=0;
            child_count--;
            break;
        }
    }
}
int main()
{
    child_count=0;
    while (1)
    {
        signal(SIGCHLD, sig_func);
        //Printing the prompt 
        prompt();
        //Taking Input
        size_t k1 = 1000;
        getline(&Input, &k1, stdin);
        k = strlen(Input) - 1;
        Input[k] = '\0';
        //Declaring the array of commands separated by ";"
        char *command_array[100000];
        //Writing the Input in history.txt 
        make_history(Input);
        // Separating commands from Input and storing in the command_array and keeping a count of commands
        long long int no_of_commands = 0;
        char *command = strtok(Input, ";");
        while (command != NULL)
        {
            command_array[no_of_commands++] = command;
            command = strtok(NULL, ";");
        }
        //Initializing background count
        bg_count = 0;
        //Initializing clock time
        clock_time = 0;
        //Running all commands in command array
        for (long long int count = 0; count < no_of_commands; count++)
        {
            //Loop to check the presence of '&' to know if we have a background process or not
            for (long long int i = 0; i < strlen(command_array[count]); i++)
                if (command_array[count][i] == '&')
                    bg_count++;
            // If there is 0 '&' symbols that is 0 background process
            if (bg_count == 0)
            {
                //Take out the first space separated token and then match which command it is
                token[0] = strtok(command_array[count], " \n\t");
                //Initializing foreground flag
                fg_present = 0;
                if (token[0] == NULL)
                    continue;
                else if (strcmp(token[0], "echo") == 0)
                {
                    echo();
                }
                else if (strcmp(token[0], "cd") == 0)
                {
                    cd();
                }
                else if (strcmp(token[0], "pwd") == 0)
                {
                    pwd();
                }
                else if (strcmp(token[0], "ls") == 0)
                {
                    ls();
                }
                else if (strcmp(token[0], "exit") == 0)
                {
                    exit(0);
                }
                else if (strcmp(token[0], "history") == 0)
                {
                    history_print();
                }
                else if (strcmp(token[0], "pinfo") == 0)
                {
                    pinfo();
                }
                else if (strcmp(token[0], "discover") == 0)
                {
                    discover();
                }
                else
                {
                    //Foreground commands like sleep , vim etc
                    //Calculating the time taken in the foreground process
                    time_t begin = time(NULL);
                    foreground();
                    time_t end = time(NULL);
                    clock_time += (long long int)(end - begin);
                }
                // Clearing the token array
                memset(token, '\0', sizeof(char *) * 10000);
            }
            else
            {
                long long int i=0;
                token_and[0]=strtok(command_array[count],"&");
                while(1)
                {
                    token_and[++i]=strtok(NULL,"&");
                    if(token_and[i]==NULL)
                        break;
                }
                //printf("%lld\n",i);
                background();
                if(i>bg_count)
                {
                    time_t begin = time(NULL);
                    foreground();
                    time_t end = time(NULL);
                    clock_time += (long long int)(end - begin);
                }
                Sort_bg();
            }
            //break;
        }
        if (clock_time >= 1)
            fg_present = 1;
    }
    return 0;
}
