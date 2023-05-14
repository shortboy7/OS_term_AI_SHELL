#include "srcs/mysh.h"

void splitInput(const char* input, char** arguments) {
    char* token;
    char* rest = strdup(input);
    int argIndex = 0;

    while ((token = strtok_r(rest, " \t\n", &rest))) {
        // Check for redirection operators
        if (strcmp(token, ">") == 0 || strcmp(token, "<") == 0) {
            arguments[argIndex] = strdup(token);
            argIndex++;
        }
        // Check for pipe operator
        else if (strcmp(token, "|") == 0) {
            arguments[argIndex] = strdup(token);
            argIndex++;
        }
        // Check for environment variables
        else if (token[0] == '$') {
            // Extract the environment variable value
            char* envVar;
            if (token[1] == '\0') {
                // Special case: '$' alone is treated as a regular argument
                arguments[argIndex] = strdup(token);
                argIndex++;
            } else {
                if (token[1] == '{' && token[strlen(token) - 1] == '}') {
                    // Handle "${ENV_VAR}" syntax
                    char envName[strlen(token) - 2];
                    strncpy(envName, token + 2, strlen(token) - 3);
                    envName[strlen(token) - 3] = '\0';
                    envVar = getenv(envName);
                } else {
                    // Handle "$ENV_VAR" syntax
                    envVar = getenv(token + 1);
                }

                if (envVar != NULL) {
                    arguments[argIndex] = strdup(envVar);
                    argIndex++;
                }
            }
        }
        // Treat as a regular argument
        else {
            arguments[argIndex] = strdup(token);
            argIndex++;
        }

        // Check if the argument limit has been reached
        if (argIndex >= MAX_ARGUMENTS - 1)
            break;
    }

    arguments[argIndex] = NULL;  // Null-terminate the argument list

    free(rest);
}

int main() {
    const char* input = "ls -l>output.txt | grep foo $HOME";

    char* arguments[MAX_ARGUMENTS];
    splitInput(input, arguments);

    int i;
    for (i = 0; arguments[i] != NULL; i++) {
        printf("Argument %d: %s\n", i, arguments[i]);
        free(arguments[i]);
    }

    return 0;
}
