#ifndef MYSH_H
#define MYSH_H

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <readline/readline.h>


#define MAX_INPUT_LENGTH 10000
#define MAX_ARGUMENTS 1000

typedef struct node {
    char *command;
    struct node *next;
} Node;

void splitInput(const char* input, char** arguments, Node* history);
void executeCommand(char** arguments);

void changeDirectory(char** arguments);
void setEnvironmentVariable(const char* variable, const char* value);

Node *addToHistory(Node *head, char *command);
void printHistory(Node *head);
void freeHistory(Node *head);

#endif