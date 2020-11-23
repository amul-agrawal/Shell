#include "../utils.h"
#include "commands.h"

void cd()
{
    char *curr_dir = (char *) malloc(BUFFER*sizeof(char));
    getcwd(curr_dir,BUFFER);
    if(command[1]==NULL)
    {
        if(chdir(HOME)!=0)
        {
            perror("cd error");
            status_of_last_command=-1;
        }
    }
    else if(command[1][0]=='-' && strlen(command[1])==1)
    {
        if(strstr(last_working_dir,HOME)!=NULL)
        {   
            printf("~%s",last_working_dir+strlen(HOME));
        }
        else
        {
            printf("%s\n",last_working_dir);
        }
        if(chdir(last_working_dir)!=0)
        {
            perror("cd error");
            status_of_last_command=-1;
        }
    }
    else if(command[1][0]=='~')
    {
        char absolute_path[BUFFER];
        strcpy(absolute_path,HOME);
        strcat(absolute_path,command[1]+1);
        strcat(absolute_path,"\0");
        if(chdir(absolute_path)!=0)
        {
            perror("cd error");
            status_of_last_command=-1;
        }
    }
    else 
    {
        if(chdir(command[1])!=0)
        {
            perror("cd error");
            status_of_last_command=-1;
        }
    }
    strcpy(last_working_dir,curr_dir);
    free(curr_dir);
}