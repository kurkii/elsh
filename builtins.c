#include "elsh.h"
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define ELSH_NO_OF_BUILTIN_CMDS 1

builtin_command_t commands[10];

char **argv_buffer;

void init_builtin_commands(){
    commands[0].command = "exit"; commands[0].function = &builtin_exit;
}

void builtin_exit(){
    kill(getpid(), SIGKILL); 
}

void function(){}

bool execute_builtin_command(char *command, char** argv){
    for(uint16_t i = 0; i < ELSH_NO_OF_BUILTIN_CMDS; i++){
        if(strcmp(commands[i].command, command) == 0){
            void (*func_ptr)();
            func_ptr = commands[i].function;
            argv_buffer = argv;
            func_ptr();
            memset(argv_buffer, 0 , BUFFER_SIZE);
            return true;
        }
    }

    return false;
}