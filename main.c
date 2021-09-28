#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>

void runSingleCommand(char **pString);

void runPipeCommand(char **pString);

char *getInput(int size) {
    char input[36];

    //Get input
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s $ ", cwd);
        fgets(input, 36, stdin);
    }
}


char **splitString(char *array, char *delim) {
    int i = 0;
    char *p = strtok(array, delim);
    char **newarray = malloc(256);

    while (p != NULL) {
        newarray[i++] = p;
        p = strtok(NULL, delim);
    }

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
            printf("\t\t%s\n", sep[k]);
            k++;
        }
        i++;
    }
    for (int j = 0; j < sizeof(holder); ++j) {
        if (holder[j] == '|'){
            pipesFound++;
        }
    }
    if (pipesFound == 0){
        runSingleCommand(holder);
    }
    else if (pipesFound > 0){
        runPipeCommand(holder);
    }
}
void runSingleCommand(char **command) {
    int pid = fork();
    // If forking failed
    if (pid < 0){
        fprintf(stderr, "Fork failed. \n");
        exit(1);
    }
        // Child process creation succeeded
    else if (pid == 0){
        execvp(command[0],command);
        printf("I am the child process with pid=%d\n", getpid());
    }
        // Child returns to parent
    else if (pid > 1){
        printf("I am the parent process with pid=%d\n", pid, getpid());
    }
}

void runPipeCommand(char **pipeCommand) {
    int pipefd[2];
    int pid = fork();
    char recv[32];
    pipe(pipefd);
    switch (pid) {
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
            fprintf(out, "This is a message to the stream from childpid:%d\n", (int) getpid());
            exit(0);
            break;
            // In parent process
        default:
            // Close writing pipefd
            close(pipefd[1]);
            printf("Wait status: %d\n", wait(NULL));
            printf("Read string: %s", recv);
            // Open pipe as stream for reading
            FILE *in = fdopen(pipefd[0], "r");
            // Read from stream
            fscanf(in, "%s", recv);
            printf(" Hello parent (pid:%d) received %s\n", (int) getpid(), recv);

    }

}


