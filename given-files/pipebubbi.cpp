//
// Created by s195481 on 9/30/21.
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>
#include "given-files/pipe.h"

#define WRITE_END 1
#define READ_END 0

void runPipeCommand(char **pipeCommand) {
    int pipefd[2];
    int pid;
    char recv[32];
    char *recv2 = malloc(32);
    int fdin = 0;

    while (*pipeCommand != NULL) {
        char **arg = splitString(pipeCommand[0], " ");
        pipe(pipefd);
        switch (pid = fork()) {
            case -1:
                perror("fork");
                exit(1);
                // In child process
            case 0:
                // Close reading pipefd

                dup2(fdin, STDIN_FILENO);
                if (*(pipeCommand + 1) != NULL)                    // if we're not executing the last command,
                    dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[0]);
                printf(" Hello parent (pid:%d) and hallo child (pid:%d). Received: %s\n", getppid(), (int) getpid(), recv2);
                execvp(arg[0], arg);
                //If execvp fails
                exit(EXIT_FAILURE);
                // In parent process
            default:
                waitpid(pid, NULL, 0);
                close(pipefd[1]);
                fdin = pipefd[0];
                pipeCommand++;
        }
    }
}
