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


void enableRawMode() {
    struct termios raw;
    tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disableRawMode() {
    struct termios raw;
    tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag |= (ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

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