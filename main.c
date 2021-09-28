#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>

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
    char *p = strtok(array, delim);
    char **newarray = malloc(256);

    while (p != NULL) {
        newarray[i++] = p;
        p = strtok(NULL, delim);
    }
    for (int j = 0; j < 256; ++j) {
        if(newarray[0][j] == '\n'){
            newarray[0][j] = NULL;
            break;
        }
    }
    return newarray;
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

void runPipeCommand(char **pipeCommand) {
    int pipefd[2];
    pid_t process1, process2;
    if (pipe(pipefd) < 0){
        printf("\nPiping failed.");
        return;
    }
    process1 = fork();
    switch(process1){
        case -1:
            printf("\nForking failed.");
            return;
        case 0:
            close(pipefd[0]);
            // Using "STDOUT_FILENO" as it is an integer file descriptor (actually, the integer 1)
            dup2(pipefd[1],STDOUT_FILENO);
            close(pipefd[1]);
            if (execvp(pipeCommand[0], pipeCommand) < 0){
                printf("\nExecution of first command failed.");
                exit(0);
            }
        default:
            process2 = fork();
            switch (process2) {
                case -1:
                    printf("\nForking failed.");
                    return;
                case 0:
                    close(pipefd[1]);
                    // Using "STDOUT_FILENO" as it is an integer file descriptor (actually, the integer 1)
                    dup2(pipefd[0],STDOUT_FILENO);
                    close(pipefd[0]);
                    if (execvp(pipeCommand[0], pipeCommand) < 0){
                        printf("\nExecution of second command failed.");
                        exit(0);
                    }
                default:
                    wait(NULL);
                    wait(NULL);
            }
            break;
    }
}

/***
 *
 * @return
 */
int main() {
    //Split for multiple args
    int pipesFound = 0;
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    do {
        char **holder = splitString(getInput(256), "|");
        printf("Listing: \n");
        int i = 0;
        /**
         * This while loop splits from '|', so
         * ls "|"
         * ws
         */
        while (holder[i] != NULL) {
            char ** sep = splitString(holder[i]," ");
            int k = 0;
            printf("\tSep: \n");
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
            runSingleCommand(sep);
            i++;
    }
        if (i<-1){
            return 1;
        }

    } while (1);
#pragma clang diagnostic pop
    /*for (int j = 0; j < sizeof(holder); ++j) {
        if (holder[j] == '|'){
            pipesFound++;
        }
    }
    if (pipesFound == 0){
        runSingleCommand(holder);
    }
    else if (pipesFound > 0){
        runPipeCommand(holder);
    }*/
}
