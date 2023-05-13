#include "mysh.h"


void addCommandToHistory(char commandHistory[MAX_COMMANDS][COMMAND_LENGTH], const char* command, int* historyCount) {
    strcpy(commandHistory[*historyCount % MAX_COMMANDS], command);
    (*historyCount)++;
}

void printCommandHistory(const char commandHistory[MAX_COMMANDS][COMMAND_LENGTH], int historyCount) {
    int start = (historyCount <= MAX_COMMANDS) ? 0 : (historyCount - MAX_COMMANDS);
    for (int i = start; i < historyCount; i++) {
        printf("%d: %s\n", i + 1, commandHistory[i % MAX_COMMANDS]);
    }
}

int main() {
    char command[COMMAND_LENGTH];
    char commandHistory[MAX_COMMANDS][COMMAND_LENGTH];
    int historyCount = 0;

    while (1) {
        printf("Enter a command (type 'history' to view command history): ");
        fgets(command, sizeof(command), stdin);

        // Remove the newline character at the end of the command
        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "history") == 0) {
            printCommandHistory(commandHistory, historyCount);
            continue;
        }

        addCommandToHistory(commandHistory, command, &historyCount);

        // Fork a child process
        pid_t pid = fork();

        if (pid < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            execlp(command, command, NULL);

            // execlp returns only if an error occurred
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

    return 0;
}