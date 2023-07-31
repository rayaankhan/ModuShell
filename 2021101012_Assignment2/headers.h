#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <signal.h>
#include <sys/stat.h>
#include <grp.h>
#include <time.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/utsname.h>

char home[10000];
char System_name[100];
char User_name[100];
char cwd[100000];
char Relative_path[100000];
char* token[100000];
char Current_directory[1000000];
char Previous_directory[1000000];
char* Input;
char* command;
char Permissions[100];
long long int k;
int a_present;
int l_present;
int f_present;
int d_present;
int fg_present;
long long int bg_count;
char* Directory_list[100000];
char* token_and[100000];
long long int no_of_dir;
FILE* ptr;
FILE* ptr1 ;
long long int total;
long long int clock_time;
long long int child_count;
pid_t pid_store[1000];
char* bg_names[1000];
int bg_time[1000];