#include "mysh.h"

char* combineArguments(char** arguments) {
    int length = 0;
    int numArgs = 0;

    // Calculate the total length of the combined string
    for (numArgs = 0; arguments[numArgs] != NULL; numArgs++) {
        length += strlen(arguments[numArgs]) + 1;  // Add 1 for the space
    }

    char* combinedString = malloc(length + 1);  // Add 1 for the null terminator
    combinedString[0] = '\0';  // Ensure the string is initially empty

    // Concatenate the arguments with spaces
    for (int i = 0; i < numArgs; i++) {
        strcat(combinedString, arguments[i]);
        strcat(combinedString, " ");
    }

    combinedString[length] = '\0';  // Null-terminate the string
    return combinedString;
}


int main() {
    Node *history = NULL;
    while (1) {
        // Read the input command
        char input[1000];
        char* arguments[MAX_ARGUMENTS];
        printf("mysh$ ");
        fgets(input, sizeof(input), stdin);

        if (input[0] == '\n') {
            continue;
        }
        // Remove newline character from input
        input[strcspn(input, "\n")] = '\0';
        splitInput(input, arguments, history);
        char *combined = combineArguments(arguments);
        history = addToHistory(history, combined);

        // Execute builtin
        if (strcmp(arguments[0], "cd") == 0 || strcmp(arguments[0], "history") == 0
            || strcmp(arguments[0], "exit") ==0 ) {
            if (strcmp(arguments[0], "cd") == 0)
                changeDirectory(arguments);
            else if (strcmp(arguments[0], "history") == 0)
                printHistory(history);
            else if (strcmp(arguments[0], "exit") == 0) {
                freeHistory(history);
                exit(0);
            }
            continue;
        }else {
            // Execute the command
            executeCommand(arguments);
        }
        int i;
        for (i = 0; arguments[i] != NULL; i++) {
            free(arguments[i]);
        }
    }
    freeHistory(history);
    return 0;
}

