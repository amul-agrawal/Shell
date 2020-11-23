#include "../utils.h"
#include "commands.h"

void Kjobs()
{
    if(num_of_args!=3)
    {
        status_of_last_command=-1;
        fprintf(stderr,"Invalid Number of Arguments\n");
        return;
    }
    int signal=atoi(command[2]);
    int JobNo=atoi(command[1]);
    if(JobNo<=0 || JobNo>num_of_bg_jobs)
    {
        status_of_last_command=-1;
        fprintf(stderr,"Invalid Job Number\n");
        return;
    }
    else if(signal<=0)
    {
        status_of_last_command=-1;
        fprintf(stderr,"Invalid Signal Number\n");
        return;
    }
    kill(BJobs[JobNo-1].pid,signal);
}