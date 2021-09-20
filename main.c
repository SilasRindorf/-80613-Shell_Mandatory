#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <limits.h>

void runCommand(char input[36]){
    printf("Parent process booting up with pid=%d!\n",getpid());
    int rc = fork();
    if (rc < 0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0){
        char *nargs[2];
        nargs[0] = strdup("ls");
        nargs[1] = NULL;
        execvp(nargs[0],nargs);
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
