#include "utils.h"
#include "commands/commands.h"

// ADDS THE PROCESS TO THE LIST OF BACKGROUND PROCESSES.
void add_process(char** name,int pid)
{
    strcpy(BJobs[num_of_bg_jobs].p_name,name[0]);
    int idx=1;
    while(name[idx]!=NULL)
    {
        strcat(BJobs[num_of_bg_jobs].p_name," ");
        strcat(BJobs[num_of_bg_jobs].p_name,name[idx]);
        idx++;
    }
    BJobs[num_of_bg_jobs].pid = pid;
    num_of_bg_jobs++;
}

// PRINTS THE PROMPT.
void print_prompt()
{
    if(PROMPT) free(PROMPT);
    PROMPT = (char *)malloc(MAX_PROMPT_SIZE*sizeof(char));
    char *hostname, *username, *pwd, rel_loc[BUFFER];
    hostname = (char *)malloc(MAX_HOSTNAME_SIZE*sizeof(char)); gethostname(hostname, MAX_HOSTNAME_SIZE);
    username = (char *)malloc(MAX_USERNAME_SIZE*sizeof(char)); username=getlogin();
    pwd = (char *)malloc(MAX_PWD_SIZE*sizeof(char)); getcwd(pwd,MAX_PWD_SIZE);

    char* home_sub = strstr(pwd,HOME);
    if(home_sub)
    {
        strcpy(rel_loc,"~");
        strcat(rel_loc, pwd + strlen(HOME));
    }   
    else
    {
        strcpy(rel_loc,pwd);
    }

    sprintf(PROMPT,"<\033[1;38;2;0;179;0m%s@%s:\033[1;38;2;230;0;0m%s\033[0m> ",username,hostname,rel_loc);
    if(status_of_last_command==0) // success
    {
        printf("\n:')%s",PROMPT); fflush(stdout);
    }
    else if(status_of_last_command==-1) // failure
    {
        printf("\n:'(%s",PROMPT); fflush(stdout);
    }
    else // start.
    {
        printf("\n%s",PROMPT); fflush(stdout);
    }
    
    
}

// BREAKS MULTIPLE COMMANDS.
int tokenize_commands(char* string)
{
    int idx = 0;
    for (int i = 0; i < BUFFER; i++)
    {
        commands[i]=(char *)malloc(BUFFER*sizeof(char));
    }   
    strcpy(commands[idx],strtok(string,";"));
    while(commands[idx]!=NULL)
    {
        char *x = strtok(NULL,";");
        if(x==NULL) 
        {
            commands[++idx]=NULL;
            break;
        }
        strcpy(commands[++idx],x);
    }
    return idx+1;
}

// BREAKS SINGLE COMMAND
int tokenize_command(char* string)
{
    if(string == NULL || strcmp(string,"\n")==0) return -1;
    int idx = 0;
    for (int i = 0; i < BUFFER; i++)
    {
        command[i]=(char *)malloc(BUFFER*sizeof(char));
    }
    strcpy(command[idx],strtok(string," \r\n\t"));
    while(command[idx])
    {
        char *x = strtok(NULL," \r\t\n");
        if(x==NULL) 
        {
            command[++idx]=NULL;
            break;
        }
        strcpy(command[++idx],x);

    }
    return idx+1;
}

int tokenize_for_pipe(char* string)
{
    if(string == NULL || strcmp(string,"\n")==0) return -1;
    int idx = 0;
    for (int i = 0; i < BUFFER; i++)
    {
        pipe_command[i]=(char *)malloc(BUFFER*sizeof(char));
    }
    strcpy(pipe_command[idx],strtok(string,"|"));
    while(pipe_command[idx])
    {
        char *x = strtok(NULL,"|");
        if(x==NULL) 
        {
            pipe_command[++idx]=NULL;
            break;
        }
        strcpy(pipe_command[++idx],x);

    }
    return idx;
}

// CHECKS WHETHER THE COMMAND IS FOR BACKGROUND PROCESS.
int is_background()
{
    int yes=0;
    if(strcmp(command[num_of_args-1],"&")==0)
    {
        command[num_of_args-1]=NULL;
        yes=1;
    }
    else if(command[num_of_args-1][strlen(command[num_of_args-1])-1]=='&')
    {
        command[num_of_args-1][strlen(command[num_of_args-1])-1] = '\0';
        yes=1;
    }
    return yes;
}

// RETURN'S ARGUMENT OF HISTORY COMMAND.    
int get_count()
{
    if(num_of_args==1) return DEF_HISTORY_SHOW;
    if(num_of_args>2) return -1;
    return atoi(command[1]);
}

int IsPiped(int curr)
{
    if(strstr(commands[curr],"|")!=NULL) return 1;
    else return 0;
}

int IsRedirection(int curr)
{
    if(strstr(commands[curr],">")!=NULL || strstr(commands[curr],"<")!=NULL) return 1;
    else return 0;
}

// CALLS THE CORRESPONDING COMMAND FILE FOR COMMAND EXECUTION.
void run_command(int curr)
{
    // printf("%s\n",commands[curr]);
    if(IsPiped(curr))
    {
        Piped(curr);
        return;
    }
    // printf("%s\n",commands[curr]);
    status_of_last_command=0;
    if(IsRedirection(curr))
    {
        Redirection(curr);
        return ;
    }
    if(strcmp(command[0], "cd")==0)
    {
        cd();  
    } 
    else if(strcmp(command[0], "pwd")==0) 
    {
        pwd();
    }
    else if(strcmp(command[0], "echo")==0) 
    {
        echo();
    }
    else if(strcmp(command[0], "ls")==0) 
    {
        ls();
    }
    else if(strcmp(command[0], "pinfo")==0)
    {
        pinfo();
    }
    else if(strcmp(command[0], "history")==0)
    {
        int k=get_count();
        print_history(k);
    }
    else if(strcmp(command[0], "nightswatch")==0)
    {
        nightswatch();
    }
    else if(strcmp(command[0], "quit")==0)
    {
        quit();
    }
    else if(strcmp(command[0],"setenv")==0)
    {
        Fsetenv();
    }
    else if(strcmp(command[0],"unsetenv")==0)
    {
        Funsetenv();
    }
    else if(strcmp(command[0],"jobs")==0)
    {
        jobs();
    }
    else if(strcmp(command[0],"kjob")==0)
    {
        Kjobs();
    }
    else if(strcmp(command[0],"overkill")==0)
    {
        overkill();
    }
    else if(strcmp(command[0],"bg")==0)
    {
        bg();
    }
    else if(strcmp(command[0],"fg")==0)
    {
        fg();
    }
    else if(is_background())
    {
        
        other_commands(1);
    }
    else 
    {
        other_commands(0);
    }
}



