#include "commands.h"
#include "../utils.h"

char* arg(char* file, int n)
{
    char* token = strtok(file," \t\r");
    for(int i=2;i<=n;i++)
    {
        token=strtok(NULL, " \r\t");
    }
    return token;
}

void pinfo()
{
    if(num_of_args > 2)
    {
        status_of_last_command=-1;
        perror("Invalid number of arguments\n");
        return ;
    }
    char stat_file_path[BUFFER],statm_file_path[BUFFER],stat_file[BUFFER],statm_file[BUFFER];
    int pid=getpid();
    if(command[1]!=NULL)
    {
        pid = atoi(command[1]);
    }
    sprintf(stat_file_path,"/proc/%d/stat",pid);
    sprintf(statm_file_path,"/proc/%d/statm",pid);
    FILE *fstat = fopen(stat_file_path,"r"), *fstatm = fopen(statm_file_path,"r");
    if(fstat==NULL) 
    {
        status_of_last_command=-1;
        fprintf(stderr,"Process with pid: %d doesn't exists.",pid);
        return;
    }
    fgets(stat_file,BUFFER,fstat);
    fgets(statm_file,BUFFER,fstatm);

    printf("pid -- %d\n",pid);
    printf("Process status -- %s\n",arg(stat_file,3));
    printf("memory -- %s\n",arg(statm_file,1));

    char exec_path[BUFFER],exec[BUFFER];
    sprintf(exec,"/proc/%d/exe",pid);
    if(readlink(exec,exec_path,BUFFER)==-1)
    {
        printf("No Executable Found\n");    
    }
    else
    {
        strcat(exec_path,"\0");
        char* home_sub = strstr(exec_path,HOME);
        if(home_sub)
        {
            home_sub += strlen(HOME);
            strcat(home_sub,"\0");
            printf("Executable Path -- ~%s\n",home_sub);
        }
        else
        {
            printf("Executable Path -- %s\n",exec_path);
        }
        
    }
}


// readlink() to read exe file for executable path.