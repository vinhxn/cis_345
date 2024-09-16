#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // Get the PATH environment variable
    char *path_env = getenv("PATH");
    if (path_env == NULL) {
        perror("Failed to get PATH environment variable");
        return EXIT_FAILURE;
    }

    // Duplicate the PATH string to avoid modifying the original
    char *path_copy = strdup(path_env);
    if (path_copy == NULL) {
        perror("Failed to duplicate PATH string");
        return EXIT_FAILURE;
    }

    // Tokenize the PATH string using ':' as the delimiter
    char *token = strtok(path_copy, ":");
    printf("Directories in PATH:\n");
    while (token != NULL) {
        printf("%s\n", token);
        token = strtok(NULL, ":");
    }

    // Free the duplicated string
    free(path_copy);

    return EXIT_SUCCESS;
}
