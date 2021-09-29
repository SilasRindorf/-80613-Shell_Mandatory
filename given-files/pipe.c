//
// Created by server on 9/29/21.
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <string.h>

#define OUT 0
#define IN 1


void piperering2(char ** args){
    int pipefd[2];
    int pid;
    char recv[32];
    char *recv2 = malloc(32);
        int fdin = 0;
        pipe(pipefd);
        switch (pid = fork()) {
            case -1:
                perror("fork");
                exit(1);
                // In child process
            case 0:
                // Close reading pipefd

                dup2(fdin, 0);
                if (*(args + 1) != NULL)                    // if we're not executing the last command,
                    dup2(pipefd[1], 1);
                close(pipefd[0]);

        //        execvp(arg[0], args);
                //If execvp fails
                exit(EXIT_FAILURE);
                // In parent process
            default:

                waitpid(pid, NULL, 0);
                //dup2(pipefd[1],0);
                //  dup2(pipefd[0],IN);
                close(pipefd[1]);
                //   char buf;
                /*    while(read(pipefd[IN],&buf,1) != -1){
                        recv2 += buf;
                        write(STDOUT_FILENO, &buf, 1);
                        printf("Char=%c",buf);
                    }
                    */

                printf(" Hello parent (pid:%d) received %s\n", (int) getpid(), recv2);
                //     return recv2;
                fdin = pipefd[0];
                args++;
        }
    }
