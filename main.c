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

int checkPipe(char *array){
    int pipes = 0;
        if (strchr( array, '|') != NULL){
            pipes = 1;
        }
        else pipes = 0;
        return pipes;
}

char **splitString(char *array, char *delim) {
    int i = 0;
    char *p = strtok(array, delim);
    char **newarray = malloc(256);

    while (p != NULL) {
        newarray[i++] = p;
        p = strtok(NULL, delim);
    }
    for (int j = 0; j < 255; ++j) {
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
            // Close reading pipefd
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
void runPipeCommandV2(char **pipeCommand, char **pipeCommand2) {
    int pipefd[2];
    pid_t process1, process2;
    if (pipe(pipefd) < 0){
        printf("\nPiping failed.");
        return;
    }
    process1 = fork();
    if (process1 < 0 ){
        printf("\nForking failed.");
    }
    if (process1 == 0 ) {
        close(pipefd[0]);
        // Using "STDOUT_FILENO" as it is an integer file descriptor (actually, the integer 1)
        FILE *out = fdopen(pipefd[1], "w"); // ope pipe as stream for writing
        // dup2(pipefd[1],STDOUT_FILENO);
        close(pipefd[1]);

        printf("\nExecution of first command did stuffs.");
        if (execvp(pipeCommand[0], pipeCommand) < 0){
            printf("\nExecution of first command failed.");
            exit(0);
        }
    } else {
        process2 = fork();
        if (process2 < 0 ){
            printf("\nForking failed.");
        }
        if (process2 == 0){
            close(pipefd[1]);
            // Using "STDOUT_FILENO" as it is an integer file descriptor (actually, the integer 1)
            dup2(pipefd[0],STDIN_FILENO);
            close(pipefd[0]);
            if (execvp(pipeCommand2[0], pipeCommand2) < 0){
                printf("\nExecution of second command failed.");
                exit(0);
            }
        } else {
            wait(NULL);
            wait(NULL);
        }
    }
}
void runPipeCommandV3(char **pipeCommand){
    int pipefd[2];
    int pid;
    char recv[32];

    pipe(pipefd);

    switch(pid= fork()) {
        case -1: perror("fork");
            exit(1);

        case 0:    // in child process
            close(pipefd[0]);       //close reading pipefd
            FILE *out = fdopen(pipefd[1], "w"); // ope pipe as stream for writing
            fprintf (out, "Howyoudoing(childpid:%d)\n", (int) getpid()); // write to stream
            break;
        default:               // in parent process
            close(pipefd[1]);      	//close	writing	pipefd
            FILE *in = fdopen(pipefd[0], "r"); // ope pipe as stream for reading
            fscanf	(in, "%s", recv); // write to stream
            printf (" Hello parent (pid:%d) received %s\n",(int)getpid(), recv);
            break;

    }
}

void runPipeCommandV4(char **pipecommand){
    int pipefd[2];
    pid_t cpid;
    char buf;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        exit(EXIT_FAILURE);
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

    if (cpid == 0) {    /* Child reads from pipe */
        close(pipefd[1]);          /* Close unused write end */

        while (read(pipefd[0], &buf, 1) > 0)
            write(STDOUT_FILENO, &buf, 1);

        write(STDOUT_FILENO, "\n", 1);
        close(pipefd[0]);
        _exit(EXIT_SUCCESS);

    } else {            /* Parent writes argv[1] to pipe */
        close(pipefd[0]);          /* Close unused read end */
        write(pipefd[1], argv[1], strlen(argv[1]));
        close(pipefd[1]);          /* Reader will see EOF */
        wait(NULL);                /* Wait for child */
        exit(EXIT_SUCCESS);
    }
}

/***
 *
 * @return
 */
int main() {
    //Split for multiple args
    int pipesFound = 0;
    char *tempchar;
    do {
        tempchar = getInput(256);
        pipesFound = checkPipe(tempchar);
        char **holder = splitString(tempchar, "|");
        //  char **holder = splitString(getInput(256), "|");

        printf("Listing: \n");
        int i = 0;
        /**
         * This while loop splits from '|', so
         * ls "|"
         * ws
         */
      //  while (holder[i] != NULL) {
            char ** sep2;
            char ** sep = splitString(holder[i]," ");
            if (holder[i+1] != NULL ){
                sep2 = splitString(holder[i+1]," ");
            }

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
            int v = 0;
            while (sep2[v] != NULL) {
                printf("\t\tsep with v=%d: ",v);
                printf("%s\n", sep2[v]);
                v++;
        }
            if(pipesFound == 0){
                runSingleCommand(sep);
            }
            else{
                runPipeCommandV2(sep, sep2);
            }
            i++;
  //  }
        if (i<-1){
            return 1;
        }

    } while (1);
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
