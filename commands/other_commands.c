#include "commands.h"
#include "../utils.h"


void other_commands(int is_bg)
{
    int status;
    int pid = fork();
    if(pid<0)
    {
        status_of_last_command=-1;
        perror("forking error");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0) // child
    {
        signal(SIGINT,SIG_DFL);
        signal(SIGTSTP,SIG_DFL);
        
        setpgid(0,0);
    
        if(execvp(command[0],command)<0)
        {
            perror("Command Not Found");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }
    else
    {
        if(!is_bg)
        {
            signal(SIGTTIN,SIG_IGN); signal(SIGTTOU,SIG_IGN);
 
            tcsetpgrp(STDIN_FILENO,pid); // child process ko terminal cantrol

            waitpid(pid, &status, WUNTRACED);
            if((status/255)==1)
            {
                status_of_last_command=-1;
            } // wait
            tcsetpgrp(STDIN_FILENO,getpgrp()); 

            signal(SIGTTIN,SIG_DFL); signal(SIGTTOU,SIG_DFL); // This is necessary as without this tcsetgrp() will not be able to resume control to child as sigttin will 
            
            if(WIFSTOPPED(status))
            {
                status_of_last_command=-1;
                printf("Process with id: %d stopped\n",pid);
                add_process(command, pid);
            }
        }
        else
        {
            add_process(command, pid);
            printf("[%d] %d\n", num_of_bg_jobs, pid);
        }
        
    }
    
    
}