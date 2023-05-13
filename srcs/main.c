#include "mysh.h"

int main() {
    const char* input = "ls -l > output.txt | grep foo $HOME";

    char* arguments[MAX_ARGUMENTS];
    splitInput(input, arguments);

    int i;
    for (i = 0; arguments[i] != NULL; i++) {
        printf("Argument %d: %s\n", i, arguments[i]);
        free(arguments[i]);
    }

    return 0;
}

