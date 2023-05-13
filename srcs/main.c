#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_LENGTH 100
#define MAX_ARGUMENTS 10

void executeCommand(const char* command) {
    char* arguments[MAX_ARGUMENTS];
    int argCount = 0;

    char* token = strtok(command, " ");

    while (token != NULL) {
        arguments[argCount] = token;
        argCount++;
        token = strtok(NULL, " ");
    }

    arguments[argCount] = NULL;

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        execvp(arguments[0], arguments);

        // execvp returns only if an error occurred
        perror("Command execution failed");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("Command exited with status: %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Command terminated by signal: %d\n", WTERMSIG(status));
        }
    }
}

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
