#include "../utils.h"
#include "commands.h"

void overkill()
{
    for(int i=0;i<num_of_bg_jobs;i++)
    {
        kill(BJobs[i].pid,SIGKILL);
    }
    num_of_bg_jobs=0;   
}