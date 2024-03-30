#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include "elsh.h"

bool called = false;

void sigint_handler_child(){
    kill(getpid(), SIGINT);
    return;
}

void sigint_handler(){
    called = true;
    signal(SIGINT, sigint_handler);
    return;
}

void print_prompt(){
    /* hostname is max 253 characters*/
    char hostname[254]; 
    gethostname(hostname, 254);
    char *wd;
    wd = cwd2rel(getcwd(wd, PATH_MAX));
    if(wd == NULL){
        printf("error: working directory couldn't be determined\n");
        return;
    }
    printf("%s%s%s%s %s%s%s", ELSH_PROMPT_LIMIT_LEFT, getlogin(), ELSH_PROMPT_DELIMITER, hostname, wd, ELSH_PROMPT_LIMIT_RIGHT, ELSH_PROMPT_SYMBOL);
}

int main(){
    char **argv;
    char buffer[BUFFER_SIZE];
    init_builtin_commands();
    putenv("TERM=elsh");
    for(;;){
        print_prompt();
        signal(SIGINT, sigint_handler);
        if(called == true){
            called = false;
        }
        
        fgets(buffer, BUFFER_SIZE, stdin);

        buffer[strcspn(buffer, "\n")] = 0;
        argv = parse_args(buffer);
        /* check for blank line so we dont do pointless computation */
        if(strcmp(argv[0], "") == 0){
            continue;
        }

        /* first check if theres a builtin command for that*/
        if(execute_builtin_command(argv[0], argv)){
            continue;
        }

        pid_t child_proc, wait_proc;
        int status = 0;
        /* child */
        if((child_proc=fork()) == 0){
            signal(SIGINT, sigint_handler_child);
            if(execvp(argv[0], argv) == -1){
                int errsv = errno;
                printf("elsh: could not execute `%s', ", argv[0]);
                switch (errsv) {
                    case 7:
                        printf("errno: E2BIG \n");
                        return 1;
                    case 13:
                        printf("errno: EACCES \n");
                        return 1;
                    case 2:
                        printf("errno: ENOENT \n");
                        return 1;
                    default:
                        printf("errno: %d \n", errsv);
                        return 1;

                }
            }
        }

        /* wait for child to finish */
        while ((wait_proc = wait(&status)) > 0);

        /* kill child */
        kill(child_proc, SIGTERM);

        /* clear buffers */
        argv = NULL;
        extern char *arg_buffer[BUFFER_SIZE];
        memset(arg_buffer, 0, BUFFER_SIZE);
    }
}
/* converts cwd to relative path */
char *cwd2rel(char *cwd){
    char *token = NULL;
    char *buffer[NAME_MAX];
    //printf("cwd: %s \n", cwd);
    token = strtok(cwd, "/");
    buffer[0] = token;
    uint16_t i = 1;
    while( token != NULL ) {
        token = strtok(NULL, "/");
        buffer[i] = token;
        i++;
    }

    uint16_t j;
    for(j = 0; j < i; j++){
        //printf("buffer[%d]: %s\n", j, buffer[j]);
        
        if(buffer[0] == NULL){
            return "/";
        }

        if(buffer[j] == NULL){
            return buffer[j-=1];
        }
    }
    return NULL;
}