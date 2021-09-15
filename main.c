#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/***
 *
 * @return
 */
int main() {
    printf("Parent process booting up with pid=%d!\n",getpid());
    int rc = fork();
    if (rc < 0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0){
        printf("I am the child process with pid=%d!\n", getpid());
    } else {
        printf("I am the parent process of %d. I have pid=%d",rc, getpid());
    }
    return 0;
}


/**
 * {}
 * Sejr GitTest
 */
