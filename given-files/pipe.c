//
// Created by server on 9/29/21.
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

char * piperering(char ** args){
    int pipefd[2];
    int pid;
    char recv[32];
    char *recv2 = malloc(32);

    pipe(pipefd);
    switch (pid=fork()) {
        case -1:
            perror("fork");
            exit(1);
            // In child process
        case 0:
            // Close reading pipefd

            dup2(pipefd[0],0);
            close(pipefd[1]);
            execvp(&args[0][0],&args[0]);
            //If execvp fails
            exit(EXIT_FAILURE);
            // In parent process
        default:

            waitpid(pid,NULL,0);
            close(pipefd[0]);
            printf(" Hello parent (pid:%d) received %s\n", (int) getpid(), recv2);
            return recv2;
    }
}