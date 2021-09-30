#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>

#define WRITE_END 1
#define READ_END 0

/**
 * This function is used for getting the user input
 * @param size is the size of the input in bytes. Is used for memory allocation.
 * @return the input as a pointer character.
 */
char *getInput(int size) {
    // Prepare string for size
    char *input = malloc(size);
    // Array for maximum path that can be shown from your directory
    char cwd[PATH_MAX];
    // Get the directory if it is not NULL
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        // Print your input
        printf("%s $ ", cwd);
        // Get input from console
        fgets(input, size, stdin);
    }
    // Return input
    return input;
}

/**
 * Used for splitting an array using a delimiter. E.g. " " or "|".
 * @param array the array given to be split
 * @param delim the delimiter we want to split by
 * @return a splitted array divided by the delimiter
 */
char **splitStringArray(char *array, char *delim) {
    // Integer used to know where we are in array
    int i = 0;
    // Split array for the first delim found
    char *p = strtok(array, delim);
    // Preparing the pointer pointer array to be the maximum size of the array given in the start
    char **newarray = malloc(sizeof(array));
    // While loop that loops until there are no more delimiters to be found
    while (p != NULL) {
        // The new array getting an added value in regards to the delimiter
        newarray[i++] = p;
        // p is used for splitting the string from the original where the delimiter is
        p = strtok(NULL, delim);
    }
    // The current string p, is splitted by \n
    p = strtok(newarray[0], "\n");
    // The newarray at the last index of the double array is set to be null byte indicating end of array
    newarray[0][strlen(p)] = '\0';
    // Returns the new array
    return newarray;
}
/**
 * Used to run the command given by the user
 * @param command the command given by the user
 */
void runCommand(char **command) {
    // File fp used for creating a file
    FILE *fp;
    // Creating an int array to be used for the writing and reading for the pipe later
    int pipefd[2];
    // Beginning to prepare for a process which is not initialized yet
    pid_t cpid;
    // Opening/Creating a file called temp.txt which is used later on
    fp = fopen("temp.txt", "w");
    // Initializing earg, extra arguments, with the memory allocation of 1024 bytes
    char *earg = malloc(1024);
    // Initializing integer i to be 0
    int i = 0;
    // This while loop loops until there are no more commands
    while (command[i] != NULL) {
        // Creating a seperated list of the input given by the user. This is done in regards to space
        char **sep = splitStringArray(command[i], " ");
        // Initializing the integer k to be 0
        int k = 0;
        // Checking from sep[0]...sep[k] to get the length of sep[k]
        while (sep[k] != NULL) {
            // Incrementing k with 1
            k++;
        }
        // Checking if i equals zero. If it does not then sep[k] is set to be "temp.txt"
        if (i != 0 ) {
            // Setting sep[k] to be "temp.txt"
            sep[k] = "temp.txt";
        }
        // Creating the pipe in regards to the pipefd. If it equals -1 then an error has occured and the program will exit
        if (pipe(pipefd) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        // Forking the parent to create a child process, cpid
        cpid = fork();
        // Checking if the child process equals -1. If so an error has occured and the program will exit
        if (cpid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        // Checking if the child process equals 0. If so then the child should write to the pipe.
        if (cpid == 0) {
            // dup2 is used for writing to the pipefd in the write end
            dup2(pipefd[WRITE_END], STDOUT_FILENO);
            // The write end of the pipe is closed
            close(pipefd[WRITE_END]);
            // The read end of the pipe is closed
            close(pipefd[READ_END]);
            // Used to show parent and child process
            // printf("Hello parent (pid:%d) and hallo child (pid:%d)\n", getppid(), (int) getpid());
            // The first command in the seperated command list is executed
            execvp(&sep[0][0], &sep[0]);
        }
        // Else statement that will make the parent read from the pibe
        else
        {
            // Closing the write end of the pibe
            close(pipefd[WRITE_END]);
            // Waiting for the child process to finish its task
            waitpid(cpid, NULL, 0);
            // Reading from the pibe in the readend and adding the answer to earg
            read(pipefd[READ_END],earg,1024);
            // Closing the pipe in the read end
            close(pipefd[READ_END]);
            // Opens the temp.txt file with mode w which clears the temp.txt file
            fp = fopen("temp.txt", "w");
            // The earg with the result stored from before is now added to the temp.txt file
            fputs(earg,fp);
            // Closing the file stream again
            fclose(fp);
            /* Normally by using dup2 you would avoid using a file as an intermediate, but as we had issues doing so we did a work around (using a file) */
        }
        // Incrementing the i by 1
        i++;
    }
    // Checking if i equals 0 if so then earg is printed out
    if(i == 1){
        printf("%s",earg);
    }
    // Else the first line of the "temp.txt" file is read and saved in char c and then printed out afterwards.
    else{
        fp = fopen("temp.txt", "r+");
        char c[1000];
        fscanf(fp,"%[^\n]",c);
        fclose(fp);
        printf("%s \n",c);
    }
}


/***
 * Used to run the program
 */
int main() {
    // Used to make the program run. Always!
    while(1) {
        // Used to split the input in regards to the pipe token (if there is one)
        char **holder = splitStringArray(getInput(256), "|");
        // Runs the command given by the user
        runCommand(holder);
    }
}