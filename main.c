#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

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


char **splitString(char *array, char *delim) {
    int i = 0;
    char *p = strtok(array, delim);
    char **newarray = malloc(256);

    while (p != NULL) {
        newarray[i++] = p;
        p = strtok(NULL, delim);
    }
    newarray[0][i] = NULL;
    return newarray;
}

/***
 *
 * @return
 */
int main() {
    //Split for multiple args
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
            printf("\t\tsep with k=%d: ",k);
            printf("%s\n", sep[k]);

            k++;
        }
        i++;
    }


}

