#include "../utils.h"
#include "commands.h"

char* Arg(char* file, int n)
{
    char* token = strtok(file," \t\r");
    for(int i=2;i<=n;i++)
    {
        token=strtok(NULL, " \r\t");
    }
    return token;
}

void jobs()
{
    char *ProcFilePath = (char *)malloc(BUFFER*sizeof(char));
    for(int now=0;now<num_of_bg_jobs;now++)
    {
        sprintf(ProcFilePath,"/proc/%d/stat",BJobs[now].pid);
        FILE* openfile = fopen(ProcFilePath,"r");
        if(openfile==NULL)
        {
            status_of_last_command=-1;
            fprintf(stderr,"Unable to open process directory for process %d",BJobs[now].pid);
            continue;
        }
        char* content=(char *)malloc(BUFFER*sizeof(char));
        fgets(content,BUFFER,openfile); 
        char* state = (char *)malloc(BUFFER*sizeof(char));
        strcpy(state,Arg(content,3));
        free(content);
        if(strcmp(state,"I")==0)
        {
            strcpy(state,"Interrupted");
        }
        else if(strcmp(state,"T")==0)
        {
            strcpy(state,"Stopped");
        }
        else
        {
            strcpy(state,"Running");
        }
        printf("[%d] %s %s [%d]\n",now+1, state, BJobs[now].p_name, BJobs[now].pid);
        free(state);
    }   
    free(ProcFilePath); 
}