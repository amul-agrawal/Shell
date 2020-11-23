#include "commands.h"
#include "../utils.h"

void quit()
{
    save_history();
    overkill();
    exit(EXIT_SUCCESS);
}