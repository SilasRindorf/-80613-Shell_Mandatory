#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <limits.h>

void runCommand(char input[]){
    //Array is always a min of 1 char to indicate end of array
    int count = 1;
    for (int i=0; i < strlen(input); i++){
        if(input[i] == ' '){
            count++;
        }
        else if(input[i] == '\n')
            input[i] = NULL;
    }
    char *nargs[count];
    nargs[count] = NULL;
    count = 0;
    nargs[count] = strtok(input," ");
    count++;
    while (nargs[count] != NULL)
    {
        nargs[count] = strtok (NULL, " ");
        count++;
    }
    count = 1;
    printf("Parent process booting up with pid=%d and command='%s'\n",getpid(),nargs[0]);
    while (nargs[count] != NULL){
        printf("\tArgument %d=%s\n",count,nargs[count]);
        count++;
    }

    int rc = fork();
    if (rc < 0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0){

        /*nargs[0] = strdup(input);
        nargs[count++] = NULL;*/
        execvp(nargs[0],nargs);
        //Does not execute and should not
        printf("I am the child process with pid=%d!\n", getpid());

    } else {
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
    do {
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s $ ", cwd);
            fgets(input,36, stdin);
            runCommand(input);
        } else {
            perror("getcwd() error");
            return 1;
        }
    }while(1);


}




/**
 * {}
 * []
 * Sejr GitTest
 */
