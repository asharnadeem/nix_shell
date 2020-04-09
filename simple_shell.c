/*
    Ashar Nadeem
    CMSC 421
    Gerald Tompkins
    Homework 1
*/

// Libraries, Imports, and Global Variables
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include "utils.h"
#include <sys/types.h>

// Function Prototypes
void runCommands(char *input, char *commands, char *type);
void interactiveShell();

// Initialize shell
void init_shell(){
    printf("\nWelcome to this shell.\n");
}

// Get user input in the interactive shell
char *getInput(){

    // Initial declarations and allocations of memory
    char *input = malloc(1);
    int character;

    /* Read input until the line ends*/
    int i = 0;
    while((character = getchar()) != '\n' && character != EOF){
        // Reallocate space for the string
        input[i++] = character;
        input = realloc(input, i+1);
    }

    // Terminate the string
    input[i] = '\0';
    
    // Assign the unescaped input to a new pointer
    // and free the old one, so I can keep track of memory
    // that has been allocated
    char *parsedInput = unescape(input,stderr);
    free(input);

    return parsedInput;
}

// Check commands for "exit" or blank input
bool validateCommands(char *input){

    // Check to see if the user entered anything at all
    if(input[0] == '\0'){
        printf("Please enter a command\n");
        free(input);
        return false;
    }
    // Check to see if the user wants to exit the program
    if(!strcmp(input,"exit")){
        printf("Thank you for using the shell!\n\n");
        free(input);
        exit(0);
    }
    return true;
}

// Run the commands in a child process
void runCommands(char *input, char *commands, char *type){
    
    // Fork a child process
    pid_t pid = fork();

    // Failed child process
    if(pid == -1){
        printf("Process failed!\n");
        free(input);
        exit(1);
    }
    // Successful
    else if(pid == 0){
        execvp(input, commands);
        // If exec command does not work, the below code will be run
        
        // Quits the program if in file mode and invalid command is present
        if(!strcmp(type,"file")){
            free(input);
            printf("Invalid command, please check your file for errors...\n");
            kill(pid, SIGKILL);
        }

        // Prints an error statement in shell mode and continues running the shell
        printf("That command did not work...\n");
        free(input);
        exit(1);
    }

    // Waiting for child to terminate 
    else {
        wait(NULL);
        free(input);
        if(!strcmp(type,"interactive")){
            if(!validateCommands(input)){interactiveShell();}
        }
    }
}

// Continuously run an interactive shell to get user input
void interactiveShell(){

    // While the interactive shell is running
    while (1){

        // Styling
        printf(">> ");

        // Get user input and validate it
        char *input = getInput();
        if(!validateCommands(input)){interactiveShell();} 

        //Allocate the same memory to individual commands as that of total input (+1 for trailing NULL)
        char* commands[count_spaces(input)+1];  

        // Tokenize input by spaces
        char *token;
        token = strtok(input," ");
        int i=0;
        while(token!=NULL){
            commands[i]=token;      
            token = strtok(NULL," ");
            i++;
        }
        // In order to use execvp, set the last value to NULL
        commands[i] = NULL;

        runCommands(input, commands, "interactive");
    }
}

// Read commands from file
void fileRead(char *fileArg){

    // Open and validate the user file
    FILE *file = fopen(fileArg, "r");
    if (file == NULL){
        printf("Could not open file %s\n",fileArg);
        exit(1);
    }

    // Loop through the file
    while (!feof(file)) {

        // Needed variables
        int index;
        int j = 0;
        size_t buff_len = 0;
        char *line = malloc(buff_len + 1);

        // Loop through each individual line
        while ((index = fgetc(file)) != '\n' && index != EOF) {
            buff_len++;
            void *tmp = realloc(line, buff_len + 1);
            line = tmp;
            line[j] = (char) index;
            j++;        
        }

        // Unescaped input and free memory
        char *parsedLine = unescape(line, stderr);
        free(line);

        // Tokenize parsed input by spaces
        char* commands[count_spaces(parsedLine)+1];  
        char *token;
        token = strtok(parsedLine," ");
        int i=0;
        while(token!=NULL){
            commands[i]=token;      
            token = strtok(NULL," ");
            i++;
        }
        // In order to use execvp, set the last value to NULL
        commands[i] = NULL;

        // Run lines
        runCommands(parsedLine, commands, "file");
    }
    // Close the file
    fclose(file);
    exit(0);
}

// Main function
int main(int argc, char *argv[]){

    switch (argc){
        // No arguments
        case 1:
            init_shell();
            interactiveShell();
            break;
        // 1 Argument
        case 2:
            fileRead(argv[1]);
            break;
        // More than 1 Argument
        default:
            fprintf(stderr, "\nERROR: The shell can only be run in interactive mode or with 1 argument.\n\n");
            exit(1);
    }

    // Exit the shell
    return 0;
}
