#include "mysh.h"

void splitInput(const char* input, char** arguments, Node* history) {
    char* token;
    char* rest = strdup(input);
    int argIndex = 0;

    while ((token = strtok_r(rest, " \t\n", &rest))) {
        // Check for history reference commands
        if (strcmp(token, "!!") == 0) {
            // Retrieve the last command from history
            Node* lastNode = history;
            while (lastNode != NULL && lastNode->next != NULL) {
                lastNode = lastNode->next;
            }
            if (lastNode != NULL) {
                // Split the last command and add its arguments to the current arguments list
                char* lastCommand = strdup(lastNode->command);
                splitInput(lastCommand, arguments + argIndex, history);
                free(lastCommand);
                // Update the argument index
                while (arguments[argIndex] != NULL)
                    argIndex++;
            } else {
                fprintf(stderr, "No previous command in history\n");
            }
        } else if (token[0] == '!') {
            // Extract the history index
            int historyIndex = atoi(token + 1);
            Node* historyNode = history;
            int count = 1;
            while (historyNode != NULL && count < historyIndex) {
                historyNode = historyNode->next;
                count++;
            }
            if (historyNode != NULL) {
                // Split the command and add its arguments to the current arguments list
                char* command = strdup(historyNode->command);
                splitInput(command, arguments + argIndex, history);
                free(command);
                // Update the argument index
                while (arguments[argIndex] != NULL)
                    argIndex++;
            } else {
                fprintf(stderr, "Invalid history reference\n");
            }
        }
        // Check for redirection operators
        else if (strcmp(token, ">") == 0 || strcmp(token, "<") == 0) {
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
