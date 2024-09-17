#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_LINE 1024

void execute_command(char *cmd) {
    char *args[MAX_LINE / 2 + 1]; // Command arguments
    char *input_file = NULL;
    char *output_file = NULL;
    int argc = 0;
    char *token = strtok(cmd, " ");

    // Parse the command and arguments
    while (token != NULL) {
        if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " ");
            if (token != NULL) {
                input_file = token;
            }
        } else if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " ");
            if (token != NULL) {
                output_file = token;
            }
        } else {
            args[argc] = token;
            argc++;
        }
        token = strtok(NULL, " ");
    }
    args[argc] = NULL;

    pid_t pid = fork();
    if (pid == 0) { // Child process
        if (input_file != NULL) {
            int fd = open(input_file, O_RDONLY);
            if (fd < 0) {
                perror("Failed to open input file");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        if (output_file != NULL) {
            int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("Failed to open output file");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        execvp(args[0], args);
        perror("Failed to execute command");
        exit(EXIT_FAILURE);
    } else if (pid > 0) { // Parent process
        wait(NULL);
    } else {
        perror("Failed to fork");
    }
}

int main() {
    char cmd[MAX_LINE];

    while (1) {
        printf("simple-shell> ");
        if (fgets(cmd, sizeof(cmd), stdin) == NULL) {
            break;
        }
        cmd[strcspn(cmd, "\n")] = '\0'; // Remove newline character

        if (strcmp(cmd, "exit") == 0) {
            break;
        }

        execute_command(cmd);
    }

    return 0;
}
