## Introduction
It's a shell implemented in C.

## Functionality
- Built-in Commands: `ls [al]`, `echo`, `pwd`, `cd`, `quit`, `history`, `pinfo`, `nightswatch`, `setenv`, `unsetenv`, `jobs`, `kjobs`, `overkill`, `fg`, `bg`, `cronjob`, `quit`.
- Process management including foreground and background processes and switching between them.
- Piping and redirection.
- Signal handling for signals like `SIGINT` and `SIGTSTP`.
- A prompt that shows the username, hostname and the current working directory
- command history.

## Make Instructions
In the directory where the source files reside, execute:
```
$ make
$ ./shell
```
To clean up object and binary files, execute:
```
$ rm shell
```

## File structure
```
.
├── commands
│   ├── bg.c
│   ├── cd.c
│   ├── commands.h
│   ├── echo.c
│   ├── env.c
│   ├── fg.c
│   ├── history.c
│   ├── jobs.c
│   ├── Kjobs.c
│   ├── ls.c
│   ├── nightswatch.c
│   ├── other_commands.c
│   ├── overkill.c
│   ├── pinfo.c
│   ├── Piped.c
│   ├── pwd.c
│   ├── quit.c
│   └── Redirection.c
├── history.txt
├── makefile
├── readme.md
├── shell.c
├── utils.c
└── utils.h
```
    
