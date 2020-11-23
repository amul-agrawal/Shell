// This file contains all the global variables and function declarations.
#ifndef UTILS
#define UTILS

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h> // hostname, username, chdir.
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <signal.h>

    #define BUFFER 1024
    #define SMALL_BUFFER 256
    #define MAX_PROMPT_SIZE 1024
    #define MAX_HOSTNAME_SIZE 256
    #define MAX_USERNAME_SIZE 256
    #define MAX_PWD_SIZE 256
    #define MAX_BACKGROUND_JOBS 100
    #define NUM_OF_HISTORY 20   
    #define DEF_HISTORY_SHOW 10

    char* PROMPT;
    char* HOME;
    char* last_working_dir;
    char* command[BUFFER];
    char* commands[BUFFER];
    char* pipe_command[BUFFER];
    int num_of_args;
    
    struct BackgroundJobs
    {
        int pid;
        char p_name[BUFFER];
    };
    struct BackgroundJobs BJobs[MAX_BACKGROUND_JOBS];
    int num_of_bg_jobs;

    char history[NUM_OF_HISTORY][BUFFER];
    int idx_of_history;
    char history_path[BUFFER];
    pid_t shell_id;
    int status_of_last_command;

    void print_prompt();
    int input();
    int tokenize_commands(char* string);
    int tokenize_command(char* string);
    int tokenize_for_pipe(char* string);
    void run_command(int curr);
    void add_process(char** name,int pid);
    int is_background();
    int IsRedirection(int curr);
    int IsPiped(int curr);
#endif

