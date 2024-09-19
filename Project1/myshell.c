#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h> // waitpid()

#define MAX_LINE 1024
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
// Function to combine 3 strings
char* combine3Strings(const char *str1, const char *str2, const char *str3) {
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

// Function to append a pathname
void addPathName(char *currPath, const char *newPath) {
    if (strlen(currPath) + strlen(newPath) + 1 < MAX_LINE) {
        if (strlen(currPath) > 0) {
            strcat(currPath, ":");
        }
        strcat(currPath, newPath);        
    } else {
        printf("Error: String length exceeds maximum limit.\n");
    }
}

// Function to delete a pathname
void removePathname(char *currPath, const char *newPath) {
    char *pos = strstr(currPath, newPath);
    if (pos != NULL) {
        size_t len = strlen(newPath);
        if ((pos == currPath || *(pos - 1) == ':') && (*(pos + len) == ':' || *(pos + len) == '\0')) {
            if (*(pos + len) == ':') {
                len++;
            }
            memmove(pos, pos + len, strlen(pos + len) + 1);
            if (pos > currPath && *(pos - 1) == ':') {
                memmove(pos - 1, pos, strlen(pos) + 1);
            }
        }
    } else {
        printf("Error: Substring not found.\n");
    }
}

void executeCommand(char *cmd, char *pathnames) {
    //for arguments
    char *argv[MAX_PARAMS]; // Command arguments
    int argc = 0;// number of arguments    
    //add, remove path
    char aPath[MAX_LINE];
    //for redirection
    char *inputFile = NULL;
    char *outputFile = NULL;
    
    /*
    Parse the command and arguments
    */
    char *token = strtok(cmd, " ");    
    while (token != NULL) {
        //check redirection from input arguments
        if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " ");
            if (token != NULL) {
                inputFile = token;
            }
        } else if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " ");
            if (token != NULL) {
                outputFile = token;
            }
        } else {
            // command arguments
            //printf("arg %d: %s\n", argc, token);
            argv[argc] = token;
            argc++;
        }
        token = strtok(NULL, " ");
    }
    argv[argc] = NULL;

    /*
    Build-in commands: cd
    */
    if(strcmp(argv[0], "cd") == 0) {
        if(argc >1)
            chdir(argv[1]);//if there is argument, pass it to chdir
        else
            chdir(".."); //if no argument, go to parent folder
    }
    /*
    Build-in commands: path
    */
    else if(strcmp(argv[0], "path") == 0) {            
        if(argc >1)
        {                
            //path + /bin: add pathname "/bin"
            //path - /bin: removes pathname "/bin" from the pathname environment if it exists
            if(strcmp(argv[1],"+") == 0){
                // Append a path to pathnames
                strcpy(aPath, argv[2]);
                addPathName(pathnames, aPath);
            }
            else if(strcmp(argv[1],"-") == 0){
                // remove a path from the pathnames
                strcpy(aPath, argv[2]);
                removePathname(pathnames, aPath);
            }
        }                
        else
        {
            // path (without argument): display the current pathname environment
            printf("%s\n", pathnames);
        }                
    }
    /*
    Execute other commands
    */
    else{
        pid_t pid = fork();
        if (pid == 0) { // Child process
            // check input redirection "<", output direction ">"
            if (inputFile != NULL) {
                int fd = open(inputFile, O_RDONLY);
                if (fd < 0) {
                    perror("Failed to open input file");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            if (outputFile != NULL) {
                int fd = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0) {
                    perror("Failed to open output file");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }

            // check the full path of the command before executing
            // full path = (pathname environment + "/" + command) e.g. "/bin/ls"            
            int isValidPath = -1;
            char* cmdPath = NULL;
            char *path = strtok(pathnames, ":"); // find path in pathnames
            while (isValidPath == -1 && path != NULL) 
            {
                cmdPath = combine3Strings(path, "/",argv[0]);
                //access(): mode X_OK: test whether the file exists and grants read, write, and execute permissions
                // return -1: the file does not exist
                isValidPath = access(cmdPath, X_OK);
                path = strtok(NULL, ":");
            }
            if(isValidPath == -1)
                printf("This command is not supported. Please try again.\n");
            else{
                execv(cmdPath, argv);
                perror("Failed to execute command");
            }
            exit(EXIT_FAILURE); // exit child process
        } 
        else if (pid > 0) { // Parent process
            wait(NULL);
        } else {
            perror("Failed to fork");
        }
    }
}

int main() {
    char cmd[MAX_LINE]; // input string
    char directoryPath[PATH_MAX];//current directory
    char pathnames[MAX_LINE] = ""; // pathname environments
    
    while (1) {
        // get current folder    
        getCurrentFolder(directoryPath);
        printf("%s$ ", directoryPath);

        if (fgets(cmd, sizeof(cmd), stdin) == NULL) {
            break;
        }
        cmd[strcspn(cmd, "\n")] = '\0'; // Remove newline character

        // check if an empty command (space) was entered
        if ((strcmp(cmd, " ") == 0) || (cmd[0] == ' ') || (strlen(cmd) == 0)) {
            continue;
        }

        if (strcmp(cmd, "quit") == 0) {
            exit(0);
        }

        executeCommand(cmd, pathnames);
    }

    return 0;
}
