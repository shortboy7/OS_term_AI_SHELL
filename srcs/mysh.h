#ifndef MYSH_H
#define MYSH_H

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>


#define MAX_INPUT_LENGTH 10000
#define MAX_ARGUMENTS 1000

void splitInput(const char* input, char** arguments);
void executeCommand(char** arguments);
#endif