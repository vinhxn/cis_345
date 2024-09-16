#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

// Function to print the array of characters
void print_array(char *array[], int size) {
    printf("Array of characters:\n");
    for (int i = 0; i < size; i++) {
        printf("%s\n", array[i]);
    }
}

// Function to add a character to the array
void add_character(char *array[], int *size, const char *new_char) {
    if (*size < MAX_SIZE) {
        array[*size] = strdup(new_char);
        if (array[*size] == NULL) {
            perror("Failed to duplicate string");
            exit(EXIT_FAILURE);
        }
        (*size)++;
    } else {
        printf("Array is full. Cannot add more characters.\n");
    }
}

// Function to remove a character from the array
void remove_character(char *array[], int *size, const char *char_to_remove) {
    for (int i = 0; i < *size; i++) {
        if (strcmp(array[i], char_to_remove) == 0) {
            free(array[i]);
            for (int j = i; j < *size - 1; j++) {
                array[j] = array[j + 1];
            }
            (*size)--;
            return;
        }
    }
    printf("Character \"%s\" not found in the array.\n", char_to_remove);
}

int main() {
    char *array[MAX_SIZE];
    int size = 0;

    // Adding characters to the array
    add_character(array, &size, "H");
    add_character(array, &size, "e");
    add_character(array, &size, "l");
    add_character(array, &size, "l");
    add_character(array, &size, "o");

    // Printing the array
    print_array(array, size);

    // Removing a character from the array
    remove_character(array, &size, "l");

    // Printing the array after removal
    print_array(array, size);

    // Freeing allocated memory
    for (int i = 0; i < size; i++) {
        free(array[i]);
    }

    return 0;
}
