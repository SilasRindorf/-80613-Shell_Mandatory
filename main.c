#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>
#include "given-files/pipe.h"

void runSingleCommand(char **command);
char * runPipeCommand(char **pipeCommand);
char * piperering(char ** args);

char *getInput(int size) {
    char *input = malloc(size);

    //Get input
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s $ ", cwd);
        fgets(input, size, stdin);
    }
    return input;
}


char **splitString(char *array, char *delim) {
    int i = 0;
    char *p = strtok(array, "\n");
    p = strtok(p, delim);
    char **newarray = malloc(256);

    while (p != NULL) {
        newarray[i++] = p;
        p = strtok(NULL, delim);
    }

    newarray[0][255] = NULL;
    return newarray;
}

/***
 *
 * @return
 */
int main() {

    //Split for multiple args
    int pipesFound = 0;
    char **holder = splitString(getInput(256), "|");
    int i = 0;
    /**
     * This while loop splits from '|', so
     * ls "|"
     * ws
     */

    while (holder[i] != NULL) {
        char ** sep = splitString(holder[i]," ");
        int k = 0;
        //printf("\tSep: \n");*/
        /**
         * This while loop splits from ' ', so
         * w " "
         * s
         */
        while (sep[k] != NULL) {
            //printf("\t\tsep with k=%d: ",k);
            //printf("%s\n", sep[k]);
            k++;
        }
        //char *recv = runPipeCommand(sep);
        //runSingleCommand(sep);
        i++;
        //piperering();
        char *recv = piperering(sep);
        printf("in main recv=%s",recv);
    }

}
void runSingleCommand(char **command) {
    printf("Command is: %s\n", command[0]);
    int rc = fork();
    // If forking failed
    if (rc < 0){
        fprintf(stderr, "Fork failed. \n");
        exit(1);
    }
    // Child process creation succeeded
    else if (rc == 0){
        execvp(&command[0][0],&command[0]);
        // Does not execute and should not
        printf("I am the child process with pid=%d\n", getpid());
    }
    // Child returns to parent
    else if (rc > 1){
        int vc = wait(NULL);
        printf("I am the parent process with pid=%d\n", rc, getpid());
    }
}

char * runPipeCommand(char **pipeCommand) {
    int pipefd[2];
    int pid;
    char recv[32];

    pipe(pipefd);
    switch (pid=fork()) {
        case -1:
            perror("fork");
            exit(1);
            // In child process
        case 0:
            // Close reading pipefd
            close(pipefd[0]);
            // Open pipe as stream for writing
            FILE *out = fdopen(pipefd[1], "w");
            // Write to stream
            fprintf(out, "howyoudoing(childpid:%d)\n", (int) getpid());
            break;
            // In parent process
        default:
            // Close writing pipefd
            close(pipefd[1]);
            // Open pipe as stream for reading
            FILE *in = fdopen(pipefd[0], "r");
            // Read from stream
            fscanf(in, "%s", recv);
            printf(" Hello parent (pid:%d) received %s\n", (int) getpid(), recv);
    }
}
