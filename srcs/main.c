#include "mysh.h"

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
        history = addToHistory(history, input);
        splitInput(input, arguments);

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

