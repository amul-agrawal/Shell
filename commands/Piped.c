#include "../utils.h"
#include "commands.h"


void Piped(int curr)
{
    int num_of_pipes = tokenize_for_pipe(commands[curr]);

    int pipefd[2],fd_in=STDIN_FILENO;
    int save_out = dup(STDOUT_FILENO),save_in = dup(STDOUT_FILENO);

    for(int i=0;i<num_of_pipes;i++)
    {
        if(pipe(pipefd)<0)
        {
            status_of_last_command=-1;
            perror("piping error\n");
            return ;
        }
        pid_t child_proc = fork();
        if(child_proc<0)
        {
            status_of_last_command=-1;
            perror("Forking error\n");
            return ;
        }
        else if(child_proc==0) // child
        {
            dup2(fd_in,STDIN_FILENO); // reading from the pipe of last command.
            if(i+1 != num_of_pipes) dup2(pipefd[1],STDOUT_FILENO); // stdout will write to the curr pipe
            close(pipefd[0]);
            {
                strcpy(commands[curr],pipe_command[i]);
                char* tmp=(char *)malloc(BUFFER*sizeof(char));
                strcpy(tmp,commands[curr]);
                tokenize_command(tmp);
                free(tmp);
                run_command(curr);
            }
            exit(EXIT_SUCCESS);
        }
        else  // parent;
        {
            wait(NULL); // wait for child process to finish;
            close(pipefd[1]); // closing the write end of pipe.
            fd_in = pipefd[0]; // saving the read end of pipe.
        }
    }
    dup2(save_in,STDIN_FILENO);
    dup2(save_out,STDOUT_FILENO);
    return;
}


// strcpy(commands[curr],Pipe[i]);
// tokenize_command(Pipe[i]); cool.
// run_command(curr);