#ifndef MYSH_H
#define MYSH_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <termios.h>
#include <ncurses.h>


#define MAX_COMMANDS 100
#define COMMAND_LENGTH 100

void addCommandToHistory(char commandHistory[MAX_COMMANDS][COMMAND_LENGTH], const char* command, int* historyCount);
void printCommandHistory(const char commandHistory[MAX_COMMANDS][COMMAND_LENGTH], int historyCount);
void enableRawMode();
void disableRawMode();


#endif