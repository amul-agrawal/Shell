#include "commands.h"
#include "../utils.h"

// LOAD HISTORY FROM FILE
void load_history()
{
    FILE* f = fopen(history_path,"r+");
    if(f==NULL) 
    {
        perror("history.txt");
        return;
    }
    idx_of_history = 0;
    while(idx_of_history<20 && fgets(history[idx_of_history],BUFFER,f)!=NULL )
    {
        idx_of_history++;
    }
    fclose(f);
}

// UPDATE HISTORY. O(20*STRLEN(COMMAND))
void update_history(char* commands) 
{
    if(idx_of_history==NUM_OF_HISTORY)
    {
        for(int i=0;i<NUM_OF_HISTORY-1;i++)
        {
            strcpy(history[i],history[i+1]);
        }
        idx_of_history--;
    }
    strcpy(history[idx_of_history],commands);
    idx_of_history++;
}

// SAVE HISTORY WHEN USER CLOSES TERMINAL.
void save_history()
{
    FILE* f = fopen(history_path,"r+");
    for(int i=0;i<idx_of_history;i++)
    {
        fputs(history[i],f);
    }
    fclose(f);
}

// PRINT HISTORY
void print_history(int count)
{
    if(count<=0)
    {
        status_of_last_command=-1;
        fprintf(stderr,"Invalid argument.\n");
        return;
    }
    int start = (idx_of_history>=count)?(idx_of_history-count):0;
    for(int i=start;i<idx_of_history;i++)
    {
        printf("%s",history[i]);
    }
}