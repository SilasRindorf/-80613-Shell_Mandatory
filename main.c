#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

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
    char **newarray = malloc(32);

    while (p != NULL) {
        newarray[i++] = p;
        p = strtok(NULL, "|");
    }

    return newarray;
}

/***
 *
 * @return
 */
int main() {
    //Split for multiple args
    char **holder = splitString(getInput(32), "|");
    //printf("Holder holds: %s", holder[0]);

    int i = 0;
    while (holder[i] != NULL) {
        printf("Holder holds: %s", holder[i]);
        i++;
    }

}

