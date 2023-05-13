#include "mysh.h"



int main() {
    char command[COMMAND_LENGTH];
    char commandHistory[MAX_COMMANDS][COMMAND_LENGTH];
    int historyCount = 0;
    int historyIndex = 0;

    enableRawMode();

    while (1) {
        printf("Enter a command (type 'history' to view command history): ");

        int index = 0;
        char c;

        while ((c = getchar()) != '\n') {
            if (c == 127 || c == 8) {  // Handle backspace/delete
                if (index > 0) {
                    putchar('\b');
                    putchar(' ');
                    putchar('\b');
                    index--;
                }
            } else {
                command[index++] = c;
                putchar(c);
            }
        }
        command[index] = '\0';

        if (strcmp(command, "history") == 0) {
            printCommandHistory(commandHistory, historyCount);
            continue;
        }

        printf("\n");

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

    disableRawMode();

    return 0;
}