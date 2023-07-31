# OSN : LINUX shell implementation in C

## Overview

C-Shell is a user-defined interactive shell written in C that works similar to the original shell and it can execeute many of the linux shell commands.The semicolon seperated commands,input output redirections,piping,signal handling are also supported.The shell can also create and manage new processes.Builtin commands like cd,ls,echo,pwd,history are implemented and it also supports running of background and foreground processes.

## Running the Shell

    1. run `make` in the extracted directory 'C Shell Project'
    2. run `./a.out`

## Commands emulated :

The commands that are supported are as follows :

* pwd : prints the absolute path of the present working directory.
* cd  : changes the directory based on the flags given.

  * ``` bash
    cd
    cd .
    cd ..
    cd ~
    cd -
    cd <directory>
    ```
* ls : lists all the files and directories in current or given directory
  * ``` bash
    ls
    ls <directory>
    ls -a/-l/-al/-la 
    ls -a/-l/-al/-la <directory>
    ls <directory_path>
    ls <filename>
    ```
* echo : prints a message on the terminal
  * ```bash
    echo <"Strings with tabs and spaces are handled">
    Assumption : "" and '' are not handled
    ```
* discover : command should search for files in a directory hierarchy.
  * ``` bash
    discover
    discover <target_dir> <type_flags> <file_name>
    discover <target_dir> -d/-f/-d -f
    discover -d/-f/-d -f <file_name>
    discover <file_name>
    discover <target_dir>
    Assumption : Does not search hidden files
    ```

* history : prints the last ten command given by the user

* pinfo : prints the status ,pid, virtual memory and executable path of the process
  * ``` bash
    pinfo
    pinfo <pid>
    ```
* others : sleep ,vim ,gedit ,mkdir etc implemented in fore ground or background

  ``` bash
  Assumption : Time in foreground is printed as a sum of all foreground processes
  ```

## Exiting From the shell

To exit from the shell, use the command

  * ```bash
    exit
    ```


## Code , Files and Functions

* prompt.c :

  * prompt(): function to print the shell promt.

  * pwd(): function that prints the path of present working directory.
  
  * cd (): The directory will be changed the argument provided if it is valid.

  * echo(): Prints a string argument on the terminal. Handles cases with and without quotes.

  * ls(): lists all the files and directories in current or given directory

* foreground.c :

  * foreground(): runs the process in foreground

* background.c :

  * background(): runs the process in background which are succeded by '&' 

* discover.c :

  * discover(): returns all the files and directories along with their path with respect to given directory or to search a file in a directory

* history.c :

  * make_history(): writes the input command in "history.txt"

* pinfo.c :

    * pinfo(): prints the status ,pid, virtual memory and executable path of the process

* main.c :

  * Has the main function and takes the input then compares with a set of commands and then   calls the appropriate fuction

* headers.h :
  * Include all predefined header files required and declaration of all global variables

* prompt.h :
  * The functions in prompt.c and defined here

* pinfo.h :
  * The function in pinfo.c is defined here

* makefile :
  * Command to run and compile the code
