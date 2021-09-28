//
// Created by s195481 on 9/28/21.
//
#include "painIsTheNewMain.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>

#define MAXCOM 1000 // max number of letters to be supported
#define MAXLIST 100 // max number of commands to be supported

// Greeting shell during startup
void init(){
}
// Function to take input
int takeInput(char* str){
}
// Function to print Current Directory.
void printDir(){
}
// Function where the system command is executed
void execArgs(char** parsed) {
}
// Function where the piped system commands is executed
void execArgsPiped(char** parsed, char** parsedpipe){
}
// Function to execute builtin commands
int ownCmdHandler(char** parsed){
}
// function for finding pipe
int parsePipe(char* str, char** strpiped){
}
// function for parsing command words
void parseSpace(char* str, char** parsed){
}
int processString(char* str, char** parsed, char** parsedpipe){
}
int main(){
}