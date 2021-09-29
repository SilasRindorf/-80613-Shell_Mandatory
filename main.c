#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>
#include "given-files/pipe.h"

void runSingleCommand(char **command);

char *runPipeCommand(char **pipeCommand);

char *piperering(char **args);

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
    char **newarray = malloc(sizeof(array));

    while (p) {
        newarray[i++] = p;
        printf("Help=%li\n",strlen(p));
        newarray[i] = realloc(newarray[i],sizeof(char*) * (strlen(p)-1));
        //newarray[i][strlen(p)-1] = NULL;
        p = strtok(NULL, delim);
    }
    //newarray = realloc(newarray, sizeof(char**) * (i+1));
    /*int k = 0;
    while (newarray[k] != NULL){
        printf("Help=%li\n",(&newarray[k])[1] - newarray[k]);
        k++;
    }*/
    return newarray;
}

void runSingleCommand2(char** command) {
    // Forking a child
    pid_t pid = fork();

    if (pid == -1) {
        printf("\nFailed forking child 8=>");
        return;
    } else if (pid == 0) {
        if (execvp(command[0], command) < 0) {
            printf("\nCould not execute command");
        }
        exit(0);
    } else {
        // waiting for child to terminate
        wait(NULL);
        return;
    }
}
void runSingleCommand(char **command) {
    printf("Command is: %s\n", command[0]);
    int rc = fork();
    // If forking failed
    if (rc < 0) {
        fprintf(stderr, "Fork failed. \n");
        exit(1);
    }
        // Child process creation succeeded
    else if (rc == 0) {
        execvp(&command[0][0], &command[0]);
        // Does not execute and should not
        printf("I am the child process with pid=%d\n", getpid());
    }
        // Child returns to parent
    else if (rc > 1) {
        int vc = wait(NULL);
        printf("I am the parent process with pid=%d\n", rc, getpid());
    }
}

/***
 *
 * @return
 */
int main() {
    while (1) {
        //Split for multiple args
        char **holder = splitString(getInput(256), "|");
        int i = 0;
        /**
         * This while loop splits from '|', so
         * ls "|"
         * ws
         */

        while (holder[i] != NULL) {
            char **sep = splitString(holder[i], " ");
            int k = 0;
            printf("\tSerperating: %s\n",holder[i]);
            /**
             * This while loop splits from ' ', so
             * w " "
             * s
             */
            while (sep[k] != NULL) {
                printf("\t\tsep with k=%d: ",k);
                printf("%s\n", sep[k]);
                k++;
            }
            sep[k-1] = NULL;
            //char *recv = runPipeCommand(sep);
            runSingleCommand2(sep);
            i++;
            //piperering();
            //piperering(sep);
            //char *recv = getInput(256);
            //printf("in main recv=%s",recv);
        }
    }
}


