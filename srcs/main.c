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

            executeCommand(command);
            command[0] = '\0';  // Reset command
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