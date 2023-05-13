#include "mysh.h"

int main() {
    char command[COMMAND_LENGTH];
    char commandHistory[MAX_COMMANDS][COMMAND_LENGTH];
    int historyCount = 0;
    int historyIndex = 0;
    int c;
    enableRawMode();
    int commandLength = 0;
    while ((c = getch()) != ERR) {
        if (c == KEY_UP || c == KEY_DOWN) {
            if (c == KEY_UP && historyIndex > 0)
                historyIndex--;
            else if (c == KEY_DOWN && historyIndex < historyCount)
                historyIndex++;

            clear();
            printw("Enter a command (type 'history' to view command history): %s", commandHistory[historyIndex % MAX_COMMANDS]);
            refresh();
        } else if (c == '\n') {
            printf("\n");
            strcpy(command, commandHistory[historyIndex % MAX_COMMANDS]);

            if (strcmp(command, "history") == 0) {
                printCommandHistory(commandHistory, historyCount);
                continue;
            }

            addCommandToHistory(commandHistory, command, &historyCount);
            historyIndex = historyCount;

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

            commandLength = 0;
        } else if (c == 127 || c == 8) {  // Handle backspace/delete
            if (commandLength > 0) {
                commandLength--;
                command[commandLength] = '\0';
                printw("\b \b");  // Erase the character from the screen
                refresh();
            }
        } else {
            if (commandLength < COMMAND_LENGTH - 1) {
                command[commandLength] = c;
                command[commandLength + 1] = '\0';
                commandLength++;
                printw("%c", c);  // Display the character on the screen
                refresh();
            }
        }
    }
    disableRawMode();

    return 0;
}