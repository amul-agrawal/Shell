#include "../utils.h"
#include "commands.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>

char modevalue[10];

void permissions(char *file){
    struct stat st;
    if(stat(file, &st) == 0){
        mode_t perm = st.st_mode;
        modevalue[0] = (perm & S_IRUSR) ? 'r' : '-';
        modevalue[1] = (perm & S_IWUSR) ? 'w' : '-';
        modevalue[2] = (perm & S_IXUSR) ? 'x' : '-';
        modevalue[3] = (perm & S_IRGRP) ? 'r' : '-';
        modevalue[4] = (perm & S_IWGRP) ? 'w' : '-';
        modevalue[5] = (perm & S_IXGRP) ? 'x' : '-';
        modevalue[6] = (perm & S_IROTH) ? 'r' : '-';
        modevalue[7] = (perm & S_IWOTH) ? 'w' : '-';
        modevalue[8] = (perm & S_IXOTH) ? 'x' : '-';
        modevalue[9] = '\0';
    }
    else{
    }   
}

void listed_show(char* dir_address,char* file)
{
    // display : <type,access_mode> <number_of_links> <owner> <group> <size> <modification_date_and_time> <name>
    char* file_address;
    file_address = (char *)malloc(BUFFER*sizeof(char));
    strcpy(file_address,dir_address);
    strcat(file_address,"/");
    strcat(file_address,file);
    struct stat data;
    if(stat(file_address,&data)!=-1)
    {
        permissions(file_address);
        struct group *gr = getgrgid(data.st_gid);
        struct passwd *pw = getpwuid(data.st_uid);
        char *time = (char *)malloc(sizeof(char) * 50); 
        strftime(time, 50, "%b  %d %H:%M", localtime( &data.st_mtime));
        if(S_ISDIR(data.st_mode)) //blue
        {
            printf("d%s\t%ld\t%s\t%s\t%ld\t %s\t\033[1;34m%s\033[0m\n", modevalue, data.st_nlink, pw->pw_name,gr->gr_name, data.st_size, time, file);
        }
        else if(modevalue[2]=='x') // green
        {
            printf("d%s\t%ld\t%s\t%s\t%ld\t %s\t\033[1;32m%s\033[0m\n", modevalue, data.st_nlink, pw->pw_name,gr->gr_name, data.st_size, time, file);
        }
        else // white       
        {
            printf("d%s\t%ld\t%s\t%s\t%ld\t %s\t%s\n", modevalue, data.st_nlink, pw->pw_name,gr->gr_name, data.st_size, time, file);
        }
        free(file_address);
    }
    else
    {
        status_of_last_command=-1;
        perror("stat");
        return;
    }
    
}

void ls_show(char* dir_address,int hidden,int listed)
{
    struct dirent *file;
    DIR *dir=opendir(dir_address);
    if(dir==NULL)
    {
        status_of_last_command=-1;
        perror("Error opening directory");
        return ;
    }
    else
    {
        printf("\033[38;2;153;153;102m%s\033[0m:\n",dir_address); 
        while ((file=readdir(dir))!=NULL)
        {
            if(!hidden && (file->d_name[0]=='.')) continue;
            if(!listed)
            {
                char* file_address;
                file_address = (char *)malloc(BUFFER*sizeof(char));
                strcpy(file_address,dir_address);
                strcat(file_address,"/");
                strcat(file_address,file->d_name);
                permissions(file_address);
                struct stat data;
                if((stat(file_address,&data)==0)&&S_ISDIR(data.st_mode)) 
                {
                    printf("\033[1;34m%s\033[0m\t",file->d_name);
                }
                else if(modevalue[2]=='x')
                {
                    printf("\033[1;32m%s\033[0m\t",file->d_name);
                }
                else 
                {
                    printf("%s\t",file->d_name);
                }
                free(file_address);
            }
            else
            {
                listed_show(dir_address,file->d_name);
            }
        }
        if(!listed) printf("\n");
        closedir(dir);
    }
    

}

void ls()
{
    int printed = 0, hidden = 0, listed = 0, idx=1;
    while(command[idx]!=NULL)  
    {
        printed=0;
        if(command[idx][0]=='-') // if there is a flag.
        {
            for(int i=0;i<strlen(command[idx]);i++)
            {
                if(command[idx][i]=='a') hidden=1;
                else if(command[idx][i]=='l') listed=1;
            }
        }
        else // there is a directory;
        {
            char* dir_address;
            dir_address = (char *)malloc(BUFFER*sizeof(char));
            if(command[idx][0]=='~')
            {
                char absolute_path[BUFFER];
                strcpy(dir_address,HOME);
                strcat(dir_address,command[idx]+1);
                strcat(dir_address,"\0");
            }
            else
            {
                strcpy(dir_address,command[idx]);
            }
            ls_show(dir_address,hidden,listed);
            free(dir_address);
            printed=1;
            hidden=0; listed=0;
            printf("\n");
        }
        idx++;
    }
    if(!printed)
    {
        ls_show(".",hidden,listed);
    }
}