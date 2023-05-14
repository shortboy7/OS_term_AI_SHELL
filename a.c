#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define MAX_INPUT_LENGTH 1000
#define MAX_ARGUMENTS 10

char* autocomplete(const char* partial) {
    char* result = NULL;
    DIR* dir;
    struct dirent* entry;

    dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        return NULL;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strncmp(entry->d_name, partial, strlen(partial)) == 0) {
            result = strdup(entry->d_name);
            break;
        }
    }

    closedir(dir);
    return result;
}

void handleTabAutocomplete(char* input) {
    char* lastToken = NULL;
    char* autocompleteResult = NULL;

    // Find the last token in the input
    char* token = strtok(input, " ");
    while (token != NULL) {
        lastToken = token;
        token = strtok(NULL, " ");
    }

    if (lastToken != NULL) {
        // Perform tab autocomplete on the last token
        autocompleteResult = autocomplete(lastToken);
        if (autocompleteResult != NULL) {
            // Append the autocomplete result to the input
            size_t inputLength = strlen(input);
            size_t autocompleteLength = strlen(autocompleteResult);
            size_t newLength = inputLength + autocompleteLength + 1; // +1 for the space
            if (newLength < MAX_INPUT_LENGTH) {
                strncat(input, " ", MAX_INPUT_LENGTH - inputLength - 1);
                strncat(input, autocompleteResult, MAX_INPUT_LENGTH - inputLength - 1);
            }
        }
    }

    free(autocompleteResult);
}

int main() {
    char input[MAX_INPUT_LENGTH];

    while (1) {
        printf("mysh$ ");
        fgets(input, sizeof(input), stdin);

        if (input[0] == '\n') {
            continue;
        }

        // Remove newline character from input
        input[strcspn(input, "\n")] = '\0';

        // Handle tab autocomplete
        handleTabAutocomplete(input);

        // Print the modified input
        printf("Modified input: %s\n", input);
    }

    return 0;
}
