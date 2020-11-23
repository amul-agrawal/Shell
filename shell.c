#include <utils.h>
#include "commands/commands.h"

void child_signal()
{
    int status;
    int pid = waitpid(-1,&status, WNOHANG);// WNOHANG is this is just a check, we don't want to wait for child process to terminate.

    if(pid > 0)
    {
        // delete the process from our list.
        char p_name[BUFFER];
        int idx=0;

        for(int i=0;i<num_of_bg_jobs;i++)
        {
            if(BJobs[i].pid == pid)
            {
                strcpy(p_name,BJobs[i].p_name);
                idx=i;
                for(int j=i+1;j<num_of_bg_jobs;j++)
                {
                    strcpy(BJobs[j-1].p_name,BJobs[j].p_name);
                    BJobs[j-1].pid = BJobs[j].pid;
                }
                BJobs[num_of_bg_jobs-1].pid = -1;
                num_of_bg_jobs--;
                break;
            }
        }
        // give a message.
        if(WIFEXITED(status) && WEXITSTATUS(status)==EXIT_SUCCESS)
        {
            fprintf(stderr,"\033[1;31m\n%s with pid %d exited normally.\n\033[0m",p_name,pid);
        }
        else
        {
            fprintf(stderr,"\033[1;31m\n%s with pid %d failed to exit normally.\033[0m\n",p_name,pid);
        }
        print_prompt();
        fflush(stdout);
    }
}

void set_signal_handlers()
{
    signal(SIGTSTP,SIG_IGN);
    signal(SIGINT,SIG_IGN);
    signal(SIGCHLD,child_signal);
    // https://stackoverflow.com/questions/50280498/how-to-only-kill-the-child-process-in-the-foreground
}

void initialize_variables()
{
    set_signal_handlers();
    status_of_last_command=1; 
    num_of_bg_jobs = 0;
    HOME=(char *)malloc(BUFFER*sizeof(char)); getcwd(HOME,BUFFER);
    last_working_dir=(char *)malloc(BUFFER*sizeof(char)); strcpy(last_working_dir,HOME);
    strcpy(history_path,HOME);
    strcat(history_path,"/history.txt");
    shell_id=getpid();
}

void run()
{
    status_of_last_command = 1;
    char all_commands[BUFFER];
    if(fgets(all_commands, BUFFER, stdin)==NULL)
    {
        quit();
    }
    if(strcmp(all_commands,"\n")==0) return; // Return if user presses ENTER.
    status_of_last_command = 0;

    // save the command in history
    update_history(all_commands);

    int num_of_commands = tokenize_commands(all_commands);
    int curr=0;
    while(commands[curr])
    {
        char *temp=(char *)malloc(BUFFER*sizeof(char));
        strcpy(temp,commands[curr]);
        num_of_args = tokenize_command(temp)-1;
        free(temp);
        if(num_of_args<0) break;
        // RUN THE COMMAND.
        run_command(curr); 
        curr++;
    }
}

int main()
{
    initialize_variables();
    load_history();
    while(1)
    {
        // PROMPT PRINTED.
        print_prompt();
        // HANDLE INPUT and execute them.
        run();
    }
    return 0;
}
