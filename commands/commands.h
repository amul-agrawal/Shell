#ifndef COMMANDS
#define COMMANDS

    void cd();
    void pwd();
    void echo();
    void ls();
    void other_commands(int is_bg);
    void pinfo();
    void load_history();
    void update_history(char* commands);
    void save_history();
    void print_history(int count);
    void quit();
    void nightswatch();
    void Redirection(int curr);
    void Piped(int curr);
    void Fsetenv();
    void Funsetenv();
    void jobs();
    void Kjobs();
    void overkill();
    void bg();
    void fg();
#endif