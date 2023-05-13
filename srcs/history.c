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
     initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
}

void disableRawMode() {
   endwin();
}