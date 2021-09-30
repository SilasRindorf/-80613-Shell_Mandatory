#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>

#define WRITE_END 1
#define READ_END 0

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
    newarray[k][strlen(p)] = '\0';
    return newarray;
}

/***
 *
 * @return
 */
int main() {
    while(1) {
        char **holder = splitStringArray(getInput(256), "|");
        runPipeCommand(holder);
    }
}

void runPipeCommand(char **pipeCommand) {
    FILE *fp;
    int pipefd[2];
    pid_t cpid;
    char buf;
    fp = fopen("test.txt", "w");
    char *earg = malloc(1024);
    int i = 0;
    while (pipeCommand[i] != NULL) {
        char **sep = splitStringArray(pipeCommand[i], " ");
        int k = 0;
        printf("\ti=%i, serperating: %s\n", i,pipeCommand[i]);
        /**
         * This while loop splits from ' ', so
         * w " "
         * s
         */
        while (sep[k] != NULL) {
            printf("\t\tsep with k=%d: \n", k);
            k++;
        }
        if (i != 0 ) {
            sep[k] = "test.txt";
        //    sep[k] = earg;
        //    sep[k][strlen(sep[k])] = '\0';
        //    printf("new sep[k]=%s\n",sep[k]);
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
            dup2(pipefd[WRITE_END], STDOUT_FILENO);           /*pipefd[WRITE_END] STDOUT_FILENO */
            close(pipefd[WRITE_END]);          /* Close unused */
            close(pipefd[READ_END]);
            execvp(&sep[0][0], &sep[0]);

        } else {            /* Parent reads from pipe */
            close(pipefd[WRITE_END]);          /* Close unused write end */
            waitpid(cpid, NULL, 0);
            read(pipefd[READ_END],earg,1024);
            close(pipefd[READ_END]);
            fp = fopen("test.txt", "w");
            fputs(earg,fp);
            fclose(fp);
        }

        i++;
    }
    if(i == 1){
        printf("%s",earg);
    }
    else{
        fp = fopen("test.txt", "r+");
        char c[1000];
        fscanf(fp,"%[^\n]",c);
        fclose(fp);
        printf("%s \n",c);
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