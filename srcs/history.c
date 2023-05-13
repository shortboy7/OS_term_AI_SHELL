#include "mysh.h"

Node *addToHistory(Node *head, char *command) {
    Node *newNode = malloc(sizeof(Node));
    newNode->command = strdup(command);
    newNode->next = NULL;

    if (head == NULL) {
        head = newNode;
    } else {
        Node *current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }

    return head;
}

void printHistory(Node *head) {
    int i = 1;
    Node *current = head;

    while (current != NULL) {
        printf("%d %s\n", i++, current->command);
        current = current->next;
    }
}

void freeHistory(Node *head) {
    Node *current = head;
    Node *next;

    while (current != NULL) {
        next = current->next;
        free(current->command);
        free(current);
        current = next;
    }
}