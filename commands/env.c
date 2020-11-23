#include "../utils.h"
#include "commands.h"


void Fsetenv()
{
    if(num_of_args!=2 && num_of_args!=3)
    {
        status_of_last_command=-1;
        fprintf(stderr,"Invalid number of arguments\n");
        return ;
    }
    char* val=(char *)malloc(BUFFER*sizeof(char));
    strcpy(val,"\0");
    if(num_of_args==3) strcpy(val,command[2]);
    if(setenv(command[1],val,1)<0)
    {
        status_of_last_command=-1;
        perror("Setenv error\n");
        return ;
    }
    free(val);
}

void Funsetenv()
{
    if(num_of_args!=2)
    {   
        fprintf(stderr,"Invalid number of arguments\n");
        status_of_last_command=-1;
        return ;
    }
    if(unsetenv(command[1])<0)
    {
        perror("Unsetenv error\n");
        status_of_last_command=-1;
        return;
    }
}