#include "../utils.h"
#include "commands.h"

void fg()
{
    if(num_of_args!=2)
    {
        status_of_last_command=-1;
        fprintf(stderr,"Invalid Number of arguments\n");
        return;
    }
    int id = atoi(command[1]);
    if(id<=0 || id>num_of_bg_jobs)
    {
        status_of_last_command=-1;
        fprintf(stderr,"Invalid process id\n");
        return;
    }
    // remove from background data structure
    // give it the cantrol of terminal
    // send cont signal
    // wait() for it to execute.
    // revert back terminal cantrol
    // check if it was stopped again or it exited.
    int pid = BJobs[id-1].pid;
    char *pname = (char *)malloc(BUFFER*sizeof(char));
    strcpy(pname,BJobs[id-1].p_name);

    for(int i=0;i<num_of_bg_jobs;i++)
    {
        if(BJobs[i].pid == pid)
        {
            for(int j=i+1;j<num_of_bg_jobs;j++)
            {
                strcpy(BJobs[j-1].p_name,BJobs[j].p_name);
                BJobs[j-1].pid = BJobs[j].pid;
            }
            BJobs[num_of_bg_jobs-1].pid = -1;
            num_of_bg_jobs--;
            break;
        }
    }
    signal(SIGTTIN,SIG_IGN); signal(SIGTTOU,SIG_IGN); 
    tcsetpgrp(STDIN_FILENO,pid);

    kill(pid,SIGCONT);

    int status;
    waitpid(pid,&status,WUNTRACED);

    tcsetpgrp(STDIN_FILENO,getpgrp());
    signal(SIGTTIN,SIG_DFL); signal(SIGTTOU,SIG_DFL);

    if(WIFSTOPPED(status))
    {
        status_of_last_command=-1;
        printf("Process with id: %d stopped\n",pid);
        strcpy(BJobs[num_of_bg_jobs].p_name,pname);
        BJobs[num_of_bg_jobs].pid = pid;
        num_of_bg_jobs++;

    }

}