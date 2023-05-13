#include "mysh.h"

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
