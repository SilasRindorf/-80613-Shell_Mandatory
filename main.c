#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>

void runSingleCommand(char **command);

char *runPipeCommand(char **pipeCommand);

void piperering(char **args);

char *getInput(int size) {
    char *input = malloc(size);

    //Get input
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s $ ", cwd);
        fgets(input, size, stdin);
    }
    int endofstring = strlen(input) - 1;
    if (input[endofstring] == '\n') {
        input[endofstring] = '\0';
    }
    return input;
}


char **splitString(char *array, char *delim) {
    char **res = NULL;

    char *intermediate = strtok(array, delim);
    int spaces = 0;

    while (intermediate) {
        res = realloc(res, sizeof(char *) * ++spaces);

        if (res == NULL)
            exit(-1);
        res[spaces - 1] = intermediate;
        intermediate = strtok(NULL, delim);
    }

    res = realloc(res, sizeof(char *) * (spaces + 1));
    res[spaces] = 0;

    return res;

    /*
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
        ;
    }
  //  newarray = realloc(newarray, sizeof(char**) * (i+1));
   // int k = 0;
   // while (newarray[k] != NULL){
  //      printf("Help=%li\n",(&newarray[k])[1] - newarray[k]);
 //       k++;
 //   }

    return newarray;
     */
}

void piperering(char **args) {
    int pipefd[2];
    int pid;
    char recv[32];
    char *recv2 = malloc(32);
    int fdin = 0;

    while (*args != NULL) {
        char **arg = splitString(args[0], " ");

        pipe(pipefd);
        switch (pid = fork()) {
            case -1:
                perror("fork");
                exit(1);
                // In child process
            case 0:
                // Close reading pipefd

                dup2(fdin, STDIN_FILENO);
                if (*(args + 1) != NULL)                    // if we're not executing the last command,
                    dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[0]);

                execvp(arg[0], arg);
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
            printf("\tSerperating: %s\n", holder[i]);
            /**
             * This while loop splits from ' ', so
             * w " "
             * s
             */
            while (sep[k] != NULL) {
                printf("\t\tsep with k=%d: ", k);
                printf("%s\n", sep[k]);
                k++;
            }


            //char *recv = runPipeCommand(sep);
            // runSingleCommand(sep);
            i++;
            //piperering();
            piperering(holder);
            //char *recv = getInput(256);
            //printf("in main recv=%s",recv);
        }
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
        wait(NULL);
        printf("I am the parent process with pid=%d\n", rc, getpid());
    }
}


