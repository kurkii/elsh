#include <linux/limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>



int pipe_two_processes(char **argv_a, char **argv_b){
    /* fd[0] is for reading, fd[1] is for writing*/
    int fd[2];
    pid_t child_a, child_b;

    if((child_a = fork()) == -1){
        return -1;
    }else if (child_a == 0) {
        close(fd[0]);
        write(fd[1], "yooo", 5);
        return 0;
    }

    if((child_b = fork()) == -1){
        return -1;
    }else if (child_b == 0) {
        close(fd[1]);
        char buffer[512];
        if(read(fd[1], buffer, 512) < 0){
            return -1;
        }else{
            printf("i got: %s\n", buffer);
            return 0;
        }
    }
    close(fd[0]);
    close(fd[1]);

    if (child_a >= 0) {
        wait(NULL);
        if (child_b >= 0) {
            wait(NULL);
        }
    }
    return 0;
}