#include "../utils.h"
#include "commands.h"

void pwd()
{
    char* cwd;
    cwd = (char *)malloc(BUFFER*sizeof(char));
    if(getcwd(cwd,BUFFER)==NULL)
    {
        status_of_last_command=-1;
        perror("Error displaying directory\n");
    }
    else 
    {
        printf("%s\n",cwd);
    }
}
