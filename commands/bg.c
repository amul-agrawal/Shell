#include "../utils.h"
#include "commands.h"

void bg()
{
    if(num_of_args!=2)
    {
        status_of_last_command=-1;
        fprintf(stderr,"Invalid Number of Arguments\n");
        return;
    }
    int num = atoi(command[1]);
    if(num<=0 || num>num_of_bg_jobs)
    {
        status_of_last_command=-1;
        fprintf(stderr,"Invalid Job Number\n");
        return;
    }
    kill(BJobs[num-1].pid,SIGCONT); 
}