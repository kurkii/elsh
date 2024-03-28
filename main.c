#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include "elsh.h"

void print_prompt(){
    /* hostname is max 253 characters*/
    char hostname[254]; 
    gethostname(hostname, 254);
    char *wd = "";
    printf("%s%s%s%s%s%s ", ELSH_PROMPT_LIMIT_LEFT, getlogin(), ELSH_PROMPT_DELIMITER, hostname, wd, ELSH_PROMPT_LIMIT_RIGHT);
}

int main(){
    char **argv;
    char buffer[BUFFER_SIZE];

    init_builtin_commands();

    for(;;){
        print_prompt();
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        argv = parse_args(buffer);

        /* first check if theres a builtin command for that*/
        if(execute_builtin_command(argv[0], argv)){
            continue;
        }

        pid_t child_proc, wait_proc;
        int status = 0;
        /* child */
        if((child_proc=fork()) == 0){ 
            if(execvp(argv[0], argv) == -1){
                int errsv = errno;
                printf("elsh: could not execute `%s', ", argv[0]);
                switch (errsv) {
                    case 7:
                        printf("errno: E2BIG \n");
                    case 13:
                        printf("errno: EACCES \n");
                    case 2:
                        printf("errno: ENOENT \n");

                }
            return 0;
            }
        }
        /* wait for child to finish */
        while ((wait_proc = wait(&status)) > 0); 

        /* clear buffers*/
        argv = NULL;
        extern char *arg_buffer[BUFFER_SIZE];
        memset(arg_buffer, 0, BUFFER_SIZE);
    }
}

