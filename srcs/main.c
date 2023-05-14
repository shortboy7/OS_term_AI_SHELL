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

int isWhitespace(const char* str) {
    while (*str != '\0') {
        if (!isspace((unsigned char)*str))
            return 0;
        str++;
    }
    return 1;
}

int main() {
    Node *history = NULL;
    while (1) {
        // Read the input command
        char* input = readline("mysh$ ");
        if (input == NULL) {
            printf("\n");
            break;
        }

        if (isWhitespace(input)) {
            free(input);
            continue;
        }

        char* arguments[MAX_ARGUMENTS];
        splitInput(input, arguments, history);
        char* combined = combineArguments(arguments);
        history = addToHistory(history, combined);
        free(input);

        // Execute builtin
        if (strcmp(arguments[0], "cd") == 0 || strcmp(arguments[0], "history") == 0
            || strcmp(arguments[0], "exit") == 0 || strcmp(arguments[0], "export") == 0) {
            if (strcmp(arguments[0], "cd") == 0)
                changeDirectory(arguments);
            else if (strcmp(arguments[0], "history") == 0)
                printHistory(history);
            else if (strcmp(arguments[0], "exit") == 0) {
                 for (int i = 0; arguments[i] != NULL; i++) {
                    free(arguments[i]);
                }
                break;
            }
            else if (strcmp(arguments[0], "export") == 0) {
            if (arguments[1] != NULL && arguments[2] != NULL)
                setEnvironmentVariable(arguments[1], arguments[2]);
            else
                printf("Invalid arguments for export command.\n");
            }
            continue;
        } else {
            // Execute the command
            executeCommand(arguments);
        }

        for (int i = 0; arguments[i] != NULL; i++) {
            free(arguments[i]);
        }
    }

    freeHistory(history);
    return 0;
}

// 1. built-in 함수와 shell command, 외부 프로그램을 모두 실행시킬 수 있어야 함.
// 2. 사용자가 사용했던 명령어들을 기억하고, 다시 칠 수 있는 기능을 history같은 명령어나 단단축키로 사용할 수 있어야 함.
// 3. 탭으로 파일 명이나 디렉토리, 명령어들을 자동완성 할 수 있어야 함.
// 4. 리다이렉션, 파이프처리를 수행할 수 있어야 함.
// 5. 환경 변수를 가져오거나 변경할 수 있어야 함.
// 6. 작업을 background나 foreground에서 처리할 수 있어야 함.
// 7. 파일이나 디렉터리 작업을 수행할 수 있어야 함.
