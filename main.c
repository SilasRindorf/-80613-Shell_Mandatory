#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <limits.h>

char *runCommand(char input[]) {
    //Array is always a min of 1 char to indicate end of array
    int count = 1;
    int pipecheck = 0;

    for (int i = 0; i < strlen(input); i++) {
        if (input[i] == ' ') {
            count++;
        }
            //Count how many arguments
        else if (input[i] == '|') {
            pipecheck = 1;
            input[i] = NULL;
        }
            //Remove non argument char
        else if (input[i] == '\n')
            input[i] = NULL;
    }
    char *nargs[count];
    //Last pos is NULL to indicate end of array
    //Since it's a pointer array
    nargs[count] = NULL;
    //Reuse counter
    count = 0;
    //Convert to pointer array
    nargs[count] = strtok(input, " ");
    count++;
    //When we hit end of array
    while (nargs[count] != NULL) {
        nargs[count] = strtok(NULL, " ");
        count++;
    }
    count = 1;
    //Prints and should print, status message
    printf("Parent process booting up with pid=%d and command='%s'\n", getpid(), nargs[0]);

    //Print argument temp print
    while (nargs[count] != NULL) {
        printf("\tArgument %d=%s\n", count, nargs[count]);
        count++;
    }
    if (pipecheck == 0)
        for (int i = 0; sizeof(input) > i; i++) {
            if (input == "|") {
                pipecheck = 1;
            }
        }

        //Create child process
        int rc = fork();
        //Fork-ing failed
        if (rc < 0) {
            fprintf(stderr, "fork failed\n");
            exit(1);
        } else if (rc == 0) { // Child process creation succeeded
            //Execute command, first argument is the command to be executed, 2nd argument is arguments for the command being executed
            execvp(nargs[0], nargs);
            //Does not execute and should not
            printf("I am the child process with pid=%d!\n", getpid());

        } else { //Child returns to parent
            int wc = wait(NULL);
            printf("I am the parent process of %d. I have pid=%d\n", rc, getpid());
        }
    }
    if (pipecheck == 1){
        int pipefd[2], nbytes;
        int pid = fork();
        char recv[32];
        pipe(pipefd);
        switch (pid) {
            case -1:
                perror("fork");
                exit(1);

            case 0:    // in child process
                close(pipefd[0]);       //close reading pipefd
                //write(fd[1], string, (strlen(string)+1));
                FILE *out = fdopen(pipefd[1], "w"); // ope pipe as stream for writing
                fprintf(out, "Howyoudoing(childpid:%d)\n", (int) getpid()); // write to stream
                exit(0);
                break;
            default:               // in parent process
                close(pipefd[1]);        //close	writing	pipefd
                ///* Read in a string from the pipe */
                nbytes = read(pipefd[0], recv, sizeof(recv));
                printf("Received string: %s", recv);
                printf("Wait status: %d\n", wait(NULL));
                printf("Read string: %s", recv);
                FILE *in = fdopen(pipefd[0], "r"); // ope pipe as stream for reading
                fscanf(in, "%s", recv); // write to stream
                printf(" Hello parent (pid:%d) received %s\n", (int) getpid(), recv);

        }
    }

}

/***
 *
 * @return
 */
int main() {
    char input[36];
    do {
        //Get input
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s $ ", cwd);
            fgets(input, 36, stdin);
            //Run our method
            runCommand(input);
        } else { //Read error
            perror("getcwd() error");
            return 1;
        }
    } while (1);

}

void runCommandWithPipe(char input[]) {
    //Array is always a min of 1 char to indicate end of array
    int count = 1;
    int pipecheck = 0;

    for (int i = 0; i < strlen(input); i++) {
        if (input[i] == ' ') {
            count++;
        }
            //Count how many arguments
        else if (input[i] == '|') {
            pipecheck = 1;
            input[i] = NULL;
        }
            //Remove non argument char
        else if (input[i] == '\n')
            input[i] = NULL;
    }
    char *nargs[count];
    //Last pos is NULL to indicate end of array
    //Since it's a pointer array
    nargs[count] = NULL;
    //Reuse counter
    count = 0;
    //Convert to pointer array
    nargs[count] = strtok(input, " ");
    count++;
    //When we hit end of array
    while (nargs[count] != NULL) {
        nargs[count] = strtok(NULL, " ");
        count++;
    }
    count = 1;
    //Prints and should print, status message
    printf("Parent process booting up with pid=%d and command='%s'\n", getpid(), nargs[0]);

    //Print argument temp print
    while (nargs[count] != NULL) {
        printf("\tArgument %d=%s\n", count, nargs[count]);
        count++;
    }


    count = 0;
    while(1){
        if ((nargs[count] = "|" )&& (nargs[count+1] != NULL)){

        }
        count++;
    }
}