#include "../utils.h"
#include "commands.h"
#include <sys/time.h>
#include <sys/types.h>

void nightswatchhelp()
{
    status_of_last_command=-1;
    fprintf(stderr,"Input Format: nightswatch [-n freq] (interrupt/newborn)\n");
}

void interrupt(int sec)
{
    FILE *f = fopen("/proc/interrupts","r");
    if(f==NULL)
    {
        status_of_last_command=-1;
        fprintf(stderr,"Can't open /proc/interrupts file\n");
        return;
    }
    char buf[BUFFER];
    fgets(buf,BUFFER,f);
    printf("%s\n",buf);
    while(1)
    {
        FILE *f = fopen("/proc/interrupts","r");
        char line[BUFFER];
        for(int i=0;i<3;i++)
        {
            fgets(line,BUFFER,f);
        }
        int reach=0;
        for(int i=0; i < strlen(line); i++)
        {
            if(!reach)
            {
                if(line[i]==':') reach=1;
                line[i]=' ';
            }
            if(line[i]=='I')
            {
                line[i] = '\0';
                break;
            }
        }
        printf("%s\n",line);

        fd_set rfds;
        struct timeval tv;
        int retval;
        /* Watch stdin (fd 0) to see when it has input. */
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);

        /* Wait up to five seconds. */
        tv.tv_sec = 0;
        tv.tv_usec = 0;

        retval = select(1, &rfds, NULL, NULL, &tv);
        if(retval==-1)
        {
            status_of_last_command=-1;
            perror("select()");
            return;
        }
        else if(retval)
        {
            char ch = fgetc(stdin);
            char dump[BUFFER];
            fgets(dump,BUFFER,stdin);
            if(ch=='q') return;
        }
        
        sleep(sec);
    }
}

void newborn(int sec)
{
    while(1)
    {
        FILE *f = fopen("/proc/loadavg","r");
        if(f==NULL)
        {
            status_of_last_command=-1;
            fprintf(stderr,"Can't open /proc/loadavg file\n");
            return;
        }
        char data[BUFFER];
        fgets(data,BUFFER,f);
        char* val=strtok(data," ");
        for(int i=0;i<4;i++)
        {
            val = strtok(NULL," ");
        }
        printf("%s",val);

        fd_set rfds;
        struct timeval tv;
        int retval;
        /* Watch stdin (fd 0) to see when it has input. */
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);

        /* Wait up to five seconds. */
        tv.tv_sec = 0;
        tv.tv_usec = 0;

        retval = select(1, &rfds, NULL, NULL, &tv);
        if(retval==-1)
        {
            status_of_last_command=-1;
            perror("select()");
            return;
        }
        else if(retval)
        {
            char ch = fgetc(stdin);
            char dump[BUFFER];
            fgets(dump,BUFFER,stdin);
            if(ch=='q') return;
        }
        sleep(sec);
        // now agar input aaye to terminate.
    }
}

void nightswatch()
{
    int valid_command = 1,is_interrupt=0,is_newborn=0;
    if(valid_command && num_of_args!=4) valid_command = 0;
    if(valid_command && strcmp(command[1],"-n")) valid_command = 0;
    if(valid_command && strcmp(command[3],"interrupt")==0) is_interrupt=1;
    else if(valid_command && strcmp(command[3],"newborn")==0) is_newborn=1;
    else valid_command = 0;
    int sec;
    if(valid_command)
    {
        sec = atoi(command[2]);
        if(sec==0) valid_command = 0;
    }
    if(!valid_command)
    {
        nightswatchhelp();
        return;
    }
    if(is_interrupt) interrupt(sec);
    else newborn(sec);
}