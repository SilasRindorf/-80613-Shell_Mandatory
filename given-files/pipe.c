//
// Created by server on 9/29/21.
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#define OUT 0
#define IN 1
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

            dup2(pipefd[OUT],OUT);
            close(pipefd[IN]);
            execvp(&args[0][0],&args[0]);
            //If execvp fails
            exit(EXIT_FAILURE);
            // In parent process
        default:

            waitpid(pid,NULL,0);
            //dup2(pipefd[1],0);
            dup2(pipefd[0],IN);
            close(pipefd[OUT]);
            char buf;
            while(read(pipefd[IN],&buf,1) != -1){
                recv2 += buf;
                write(STDOUT_FILENO, &buf, 1);
                printf("Char=%c",buf);
            }

            printf(" Hello parent (pid:%d) received %s\n", (int) getpid(), recv2);
            return recv2;
    }
}