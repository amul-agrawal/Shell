#include "../utils.h"
#include "commands.h"

void echo()
{
    int idx=1;
    while(command[idx]!=NULL)
    {
        printf("%s ",command[idx++]);
    }
    printf("\n");
}