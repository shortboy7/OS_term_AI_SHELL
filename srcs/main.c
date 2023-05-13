#include "mysh.h"

int main() {
    char input[MAX_INPUT_LENGTH];

    while (1) {
        printf("Enter a command (type 'exit' to quit): ");
        fgets(input, sizeof(input), stdin);

        // Remove trailing newline character
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) {
            break;
        }

        executeCommand(input);
    }

    return 0;
}
