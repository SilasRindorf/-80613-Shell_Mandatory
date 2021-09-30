#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>
#include "given-files/pipe.h"

#define WRITE_END 1
#define READ_END 0


void runSingleCommand(char **command);

void runPipeCommand(char **pipeCommand);


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

char **splitStringArray(char *array, char *delim) {
    int i = 0;
    char *p = strtok(array, delim);
    char **newarray = malloc(sizeof(array));
    while (p != NULL) {
        newarray[i++] = p;

        p = strtok(NULL, delim);
    }
    int k = 0;
    p = strtok(newarray[k], "\n");
    while (p != NULL) {
        newarray[k][strlen(p)] = '\0';
        p = strtok(NULL, delim);
    }

    return newarray;
}

/***
 *
 * @return
 */
int main() {


    char **holder = splitStringArray(getInput(256), "|");
    runPipeCommand(holder);


    //Split for multiple args
    //int i = 0;
    /**
     * This while loop splits from '|', so
     * ls "|"
     * ws
     */
    /*
   while (holder[i] != NULL) {
       char **sep = splitStringArray(holder[i], " ");
       int k = 0;
       printf("\tSerperating: %s\n", holder[i]);
       /**
        * This while loop splits from ' ', so
        * w " "
        * s
        *//*
        while (sep[k] != NULL) {
            printf("\t\tsep with k=%d: ", k);
            printf("%s\n", sep[k]);
            k++;
        }
        i++;
    }*/


}

void runPipeCommand(char **pipeCommand) {
    int pipefd[2];
    pid_t cpid;
    char buf;
    char *recv2 = malloc(32);
    int fdin = 0;

    char * earg = malloc(256);
    int i = 0;
    //while (*pipeCommand[i] != NULL) {
    while (pipeCommand[i] != NULL) {
        char **sep = splitStringArray(pipeCommand[i], " ");
        int k = 0;
        //printf("\ti=%i, serperating: %s\n", i,pipeCommand[i]);
        /**
         * This while loop splits from ' ', so
         * w " "
         * s
         */
        while (sep[k] != NULL) {
            /*printf("\t\tsep with k=%d: ", k);
            printf("%s\n", sep[k]);*/
            k++;
        }
        if (pipe(pipefd) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        cpid = fork();
        if (cpid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (cpid == 0) {    /* Child write to pipe */
            /*
                dup2(pd[0], 0);
                close(pd[0]);
                close(pd[1]);
                execlp("wc", "wc", "-l", (char *)NULL);
                fprintf(stderr, "Failed to execute 'wc'\n");
                exit(1);
            */
            dup2(fdin, STDIN_FILENO);
            //dup2(pipefd[WRITE_END], STDIN_FILENO);
            close(pipefd[WRITE_END]);
            //if (*(pipeCommand[i] + 1) != NULL)                    // if we're not executing the last command,
            //dup2(pipefd[0], STDOUT_FILENO);
            printf("\nHello parent (pid:%d) and hallo child (pid:%d). \nCommand run: %s \nReceived: %s\n",
                   getppid(), (int) getpid(), &sep[0][0],recv2);
            close(pipefd[READ_END]);
            execvp(&sep[0][0], &sep[0]);
            exit(EXIT_FAILURE);
        } else {            /* Parent reads from pipe */
            close(pipefd[WRITE_END]);
            waitpid(cpid, NULL, 0);
            fdin = pipefd[READ_END];
            //close(pipefd[READ_END]);
            //printf("fdin %s\n and recv2 %s", fdin, recv2);
        }
        i++;
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


