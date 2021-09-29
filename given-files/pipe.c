//
// Created by server on 9/29/21.
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <fcntl.h>

char * piperering(char ** args){
    printf("Entering\n");
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
            //close(pipefd[0]);
            // Open pipe as stream for writing
            //FILE *out = fdopen(pipefd[1], "w");
            dup2(pipefd[0],0);
            close(pipefd[1]);
            execvp(&args[0][0],&args[0]);
            // Write to stream
            //fprintf(out, "howyoudoing(childpid:%d)\n", (int) getpid());
            break;
            // In parent process
        default:
            // Close writing pipefd
            //close(pipefd[1]);
            // Open pipe as stream for reading
            //FILE *in = fdopen(pipefd[0], "r");
            // Read from stream
            //fscanf(in, "%s", recv2);
            //int re = open(recv2,O_WRONLY | O_APPEND);
            dup2(pipefd[1],1);
            close(pipefd[0]);
            wait(NULL);
            printf(" Hello parent (pid:%d) received %s\n", (int) getpid(), recv2);
            return recv2;
    }
}