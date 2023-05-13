#include "mysh.h"

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
            char* envVar = getenv(token + 1);
            if (envVar != NULL) {
                arguments[argIndex] = strdup(envVar);
                argIndex++;
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

}
