#include <stdio.h>  // printf(), fgets()
#include <string.h> // strtok(), strcmp(), strdup()
#include <stdlib.h> // free()
#include <unistd.h> // fork()
#include <sys/types.h>
#include <sys/wait.h> // waitpid()
#include <sys/stat.h>
#include <fcntl.h> // open(), creat(), close()
#include <time.h>
#include <errno.h>
#define MAXLINE 4096
#define PATH_MAX 260
#define MAX_PARAMS 10


void getCurrentFolder(char folderName[PATH_MAX]) {
    char fullpath[PATH_MAX];
    
    if (getcwd(fullpath, sizeof(fullpath)) != NULL) {
        strcpy(folderName, fullpath); //if no '/' found then return path
        //for Windows
        //char* lastSlash = strrchr(fullpath, '\\');
        //for Linux
        char* lastSlash = strrchr(fullpath, '/');
        if (lastSlash != NULL) {
            strcpy(folderName, lastSlash + 1);
        }
    }    
}

char* combineStrings(const char *str1, const char *str2) {
    // Calculate the length of the new string
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    size_t total_len = len1 + len2 + 1; // +1 for the null terminator

    // Allocate memory for the new string
    char *result = malloc(total_len * sizeof(char));
    if (result == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    // Copy the first string to the result
    strcpy(result, str1);
    // Append the second string to the result
    strcat(result, str2);

    return result;
}
char* combineStrings3(const char *str1, const char *str2, const char *str3) {
    // Calculate the length of the new string
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    size_t len3 = strlen(str3);
    size_t total_len = len1 + len2 + len3 + 1; // +1 for the null terminator

    // Allocate memory for the new string
    char *result = malloc(total_len * sizeof(char));
    if (result == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    // Copy the first string to the result
    strcpy(result, str1);
    // Append the second string to the result
    strcat(result, str2);
    // Append the third string to the result
    strcat(result, str3);

    return result;
}

void printShellDirectory(char directory_path[]){
    printf("%s $ ", directory_path);
}

// Function to print the pathname environments
void initPathnameEnvs(char *array[], int size) {
    //printf("Pathname environments:\n");
    for (int i = 0; i < size; i++) {
        array[i] = NULL;
    }
}

// Function to print the pathname environments
void printPathnameEnvs(char *array[], int size) {
    if(size == 0){
        printf("Pathname environments is empty.\n");
    }
    else{
        for (int i = 0; i < size; i++) {
            printf("%s\n", array[i]);
        }
    }
}

// Function to add a pathname environment
void addPathnameEnv(char *array[], int *size, const char *newPath) {
    if (*size < PATH_MAX) {
        array[*size] = strdup(newPath);
        if (array[*size] == NULL) {
            perror("Failed to duplicate string");
            exit(EXIT_FAILURE);
        }
        (*size)++;
        printf("Add pathname \"%s\" successful.\n", newPath);
    } else {
        printf("Array is full. Cannot add more pathname.\n");
    }
}

// Function to remove a pathname environment
void removePathnameEnv(char *array[], int *size, const char *pathToRemove) {
    for (int i = 0; i < *size; i++) {
        // if found path to remove, remove it, then moves the elements after that to left by one position
        if (strcmp(array[i], pathToRemove) == 0) {
            free(array[i]);
            for (int j = i; j < *size - 1; j++) {
                array[j] = array[j + 1];
            }
            (*size)--;
            printf("Remove pathname \"%s\" successful.\n", pathToRemove);
            return;
        }
    }
    //printf("Path \"%s\" not found.\n", pathToRemove);
}

int main(void) {
    char   buf[MAXLINE];
    pid_t  pid;
    int    status;
    char directory_path[PATH_MAX];
    int argc;
    char *argv[MAX_PARAMS]; //array of arguments input by user
    char *pathnameEnvs[PATH_MAX];// pathname environments
    int pathnameEnvCount; // number of pathname environments

    //init pathname environments
    initPathnameEnvs(pathnameEnvs, pathnameEnvCount);
    // get current folder    
    getCurrentFolder(directory_path);    
    printShellDirectory(directory_path);//printf("%s $ ", directory_path);
   
    while (fgets(buf, MAXLINE, stdin) != NULL) {
        if (buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = 0; // replace newline with null
        
        // check if an empty command (space) was entered
        if ((strcmp(buf, " ") == 0) || (buf[0] == ' ') || (strlen(buf) == 0)) {
            printShellDirectory(directory_path);//printf("%s $ ", directory_path);
            continue;
        }
        // Build-in commands: quit
        if (strcmp(buf, "quit") == 0) {
            exit(EXIT_SUCCESS);
        }

        // parse the line with command and arguments, store to "params[]" array with argc elements
        argc = 0;
        // Initialize argv[] with NULL values
        for(int i =0; i < MAX_PARAMS; i++){
            argv[i] = NULL;
        }
        // find command
        char *arg = strtok(buf, " "); // Split input by spaces
        while (arg && argc < MAX_PARAMS) {
            //printf("Argument %d: %s\n", argc, arg);
            argv[argc] = strdup(arg); // Allocate memory and copy the word
            // find arguments
            arg = strtok(NULL, " ");
            argc++;
        }
        
        // Build-in commands: cd
        if(strcmp(argv[0], "cd") == 0) {
            if(argc >1)
                chdir(argv[1]);//if there is argument, pass it to chdir
            else
                chdir(".."); //if no argument, go to parent folder
        }
        else 
        // Build-in commands: path
        if(strcmp(argv[0], "path") == 0) {            
            if(argc >1)
            {                
                //path + /abc/def: appends directory "/abc/def" to the end of the pathname environment.
                //path - /abc/def: removes pathname "/abc/def" from the pathname environment if it exists. Otherwise, do nothing.
                if(strcmp(argv[1],"+") == 0){
                    addPathnameEnv(pathnameEnvs, &pathnameEnvCount, argv[2]);                    
                }
                else if(strcmp(argv[1],"-") == 0){
                    removePathnameEnv(pathnameEnvs, &pathnameEnvCount, argv[2]);
                }
            }                
            else
            {
                // path (without argument): display the current pathname environment
                printPathnameEnvs(pathnameEnvs, pathnameEnvCount);
            }                
        }
        // Execute other command
        else{            
            if ((pid = fork()) < 0) {
                printf("Error: fork error");
            } else if (pid == 0) { // child
                // if(execv(combineStrings(cmdPath, argv[0]), argv) == -1){
                //     printf("Error: couldn't execute: %s\n", buf);
                //     exit(127);
                // }
                int i = 0;
                char* cmdPath = combineStrings3(pathnameEnvs[i], "/",argv[0]);
                while (execv(cmdPath, argv) == -1)
                {
                    //printf("Error: couldn't execute: %s\n", cmdPath);
                    i++;
                    cmdPath = combineStrings3(pathnameEnvs[i], "/",argv[0]);
                }
            }
            else {
                // parent
                if ((pid = waitpid(pid, &status, 0)) < 0)
                    printf("Error: waitpid error");
            }            
            
        }

        //get current directory again
        getCurrentFolder(directory_path);
        printShellDirectory(directory_path);//printf("%s $ ", directory_path);
    }
    exit(EXIT_SUCCESS);
}