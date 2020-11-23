#include "../utils.h"
#include "commands.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int IsFile(char* file)
{
    struct stat f;
    if(stat(file, &f) == 0 && !S_ISDIR(f.st_mode)) return 1;
    else return 0;
}


// checks if input is redirected. if yes dup2 the stdin.
int input_redirection(int curr)
{
    int isredirected = 0;
    if(strstr(commands[curr],"<")!=NULL)
    {
        isredirected = 1;
    }
    else return 0;
    char* Temp = (char *)malloc(BUFFER*sizeof(char));
    char* Temp2 = (char *)malloc(BUFFER*sizeof(char));
    strcpy(Temp2,commands[curr]);
    Temp = strtok(Temp2,"<");
    Temp = strtok(NULL,"<");
    char* InputFile = (char *)malloc(BUFFER*sizeof(char));
    InputFile = strtok(Temp," \n\t\r");
    if(InputFile==NULL || !IsFile(InputFile)) 
    {
        status_of_last_command=-1;
        fprintf(stderr,"Input File not found\n"); 
        return -1;
    }
    int fd_in = open(InputFile,O_RDONLY);
    if(fd_in<0)
    {
        status_of_last_command=-1;
        perror("Error opening input file\n");
        return -1;
    }
    dup2(fd_in, STDIN_FILENO);
    close(fd_in);
    return isredirected;
}

int output_redirection(int curr)
{
    int isredirected = 0;
    if(strstr(commands[curr],">")!=NULL)
    {
        isredirected=1;
    }
    else return 0;
    int Type = 0;
    char* Temp=(char *)malloc(BUFFER*sizeof(char));
    if(strstr(commands[curr],">>")!=NULL)
    {
        Type=1;
    }
    char* Temp2 = (char *)malloc(BUFFER*sizeof(char));
    strcpy(Temp2,commands[curr]);
    if(Type==1)
    {
        Temp = strtok(Temp2,">>");
        Temp = strtok(NULL,">>");

    }
    else if(Type==0)
    {
        Temp = strtok(Temp2,">");
        Temp = strtok(NULL,">");
    }
    char* OutputFile = (char *)malloc(BUFFER*sizeof(char));
    OutputFile = strtok(Temp," \n\t\r");
    if(OutputFile == NULL)
    {
        fprintf(stderr,"Enter Ouput File\n");
        return -1;
    }
    int fd_out=1;
    if(Type==0)
    {
        fd_out = open(OutputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    }
    else
    {
        fd_out = open(OutputFile, O_WRONLY | O_CREAT | O_APPEND, 0644);
    }
    if(fd_out<0) 
    {
        status_of_last_command=-1;
        fprintf(stderr,"Cannot open output file\n");
        return -1;
    }
    dup2(fd_out, STDOUT_FILENO);
    close(fd_out);
    return isredirected;
}

void update_command(int curr)
{
    int skipnext=0,idx=0,i=0;
    char* curr_command[BUFFER];
    while(command[idx]!=NULL)
    {
        if(skipnext)
        {
            skipnext = 0;
        }
        else if(command[idx][0]=='>')
        {
            int len = strlen(command[idx]);
            if(len==2)
            {
                if(command[idx][1]!='>') continue;
                else skipnext=1;
            }
            else if(len<2) skipnext=1;
            
        }
        else if(command[idx][0]=='<')
        {
            int len = strlen(command[idx]);
            if(len==1) skipnext=1;
        }
        else
        {
            curr_command[i]=(char *)malloc(BUFFER*sizeof(char));
            strcpy(curr_command[i],command[idx]);
            i++;  
        }
        idx++;
    }
    num_of_args=i;
    for(int i=0;i<BUFFER;i++)
    {
        free(command[i]);
        command[i]=(char *)malloc(BUFFER*sizeof(char));
    }
    for (int i = 0; i < num_of_args; i++)
    {
        strcpy(command[i],curr_command[i]);
    }
    command[num_of_args]=NULL;
    strcpy(commands[curr],command[0]);
    for(int i=1;i<num_of_args;i++)
    {
        strcat(commands[curr]," ");
        strcat(commands[curr],command[i]);
    }
}

void Redirection(int curr)
{
    int save_in = dup(STDIN_FILENO), save_out = dup(STDOUT_FILENO);

    int FlagIn = input_redirection(curr);
    if(FlagIn==-1) return ;

    int FlagOut = output_redirection(curr);
    if(FlagOut==-1) return ;

    update_command(curr);
    
    run_command(curr);

    dup2(save_in,STDIN_FILENO);
    close(save_in);
    dup2(save_out,STDOUT_FILENO);
    close(save_out);
}