#include "elsh.h"
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <locale.h>

#define ELSH_NO_OF_BUILTIN_CMDS 3

builtin_command_t commands[10];

char **argv_buffer;

void init_builtin_commands(){
    commands[0].command = "exit"; commands[0].function = &builtin_exit;
    commands[1].command = "cd"; commands[1].function = &builtin_cd;
    commands[2].command = "help"; commands[2].function = &builtin_help;
}

void builtin_exit(){
    kill(getpid(), SIGKILL); 
}

int builtin_cd(){

    if(argv_buffer[1] == NULL){
        printf("cd: missing argument \n");
        return 1;
    }
    if(chdir(argv_buffer[1]) == -1){
        printf("cd: %s: %s\n", argv_buffer[1], strerror(errno));
        return errno;
    }
    return 0;
}

void builtin_help(){
    printf("elsh version %d.%d | compiled at %s %s\n", ELSH_VERSION_MAJOR, ELSH_VERSION_MINOR, __DATE__, __TIME__);
    printf("elsh is a shell designed for the epic linux experience, the use of this shell guarantees an epic shell experience \n");
    return;
}

bool execute_builtin_command(char *command, char** argv){
    for(uint16_t i = 0; i < ELSH_NO_OF_BUILTIN_CMDS; i++){
        if(strcmp(commands[i].command, command) == 0){
            int (*func_ptr)();
            func_ptr = commands[i].function;
            argv_buffer = argv;
            int err = func_ptr();
            memset(argv_buffer, 0 , BUFFER_SIZE);
            return true;
        }
    }

    return false;
}