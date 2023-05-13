#include "mysh.h"

int main() {
     // Read the input command
    char input[1000];
    printf("Enter command: ");
    fgets(input, sizeof(input), stdin);

    char* arguments[MAX_ARGUMENTS];
    splitInput(input, arguments);
    executeCommand(arguments);
    int i;
    for (i = 0; arguments[i] != NULL; i++) {
        printf("%s\n", arguments[i]);
        free(arguments[i]);
    }


    return 0;
}

