#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <limits.h>

void runCommand(char input[]){
    //Array is always a min of 1 char to indicate end of array
    int count = 1;
    //Count how many arguments
    for (int i=0; i < strlen(input); i++){
        if(input[i] == ' '){
            count++;
        }
            //Remove non argument char
        else if(input[i] == '\n')
            input[i] = NULL;
    }
    int pipeLength = 0;
    for (int i = 0; sizeof(input) > i; i++){
        if (input[i] == '|'){
            pipeLength++;
        }
    }
    char *nargs[pipeLength][count];
    //Last pos is NULL to indicate end of array
    //Since it's a pointer array
    nargs[0][count] = NULL;
    //Reuse counter
    count = 0;
    //Convert to pointer array
    nargs[0][count] = strtok(input," ");
    count++;
    //When we hit end of array
    while (nargs[0][count] != NULL)
    {
        nargs[0][count] = strtok (NULL, " ");
        count++;
    }
    count = 0;
    //Prints and should print, status message
    printf("Parent process booting up with pid=%d and command='%s'\n",getpid(),nargs[0][0]);

    //Print argument temp print
    while (nargs[0][count] != NULL){
        printf("\tArgument %d=%s\n",count,nargs[0][count]);
        count++;
    }

    //Create child process
    int rc = fork();
    //Fork-ing failed
    if (rc < 0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0){ // Child process creation succeeded
        //Execute command, first argument is the command to be executed, 2nd argument is arguments for the command being executed
        execvp(nargs[0][0],nargs[0]);
        //Does not execute and should not
        printf("I am the child process with pid=%d!\n", getpid());

    } else { //Child returns to parent
        int wc = wait(NULL);
        printf("I am the parent process of %d. I have pid=%d\n",rc, getpid());
    }
}
/***
 *
 * @return
 */
int main() {
    char input[36];
    //Loop so on exit after excecution of command
    do {
        //Get input
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s $ ", cwd);
            fgets(input,36, stdin);
            //Run our method
            runCommand(input);
        } else { //Read error
            perror("getcwd() error");
            return 1;
        }
    }while(1);


}
