#include "mysh.h"

void executeCommand(const char* command) {
    char* arguments[MAX_ARGUMENTS];
    int argCount = 0;

    char* token = strtok(command, " ");

    while (token != NULL) {
        arguments[argCount] = token;
        argCount++;
        token = strtok(NULL, " ");
    }

    arguments[argCount] = NULL;

    int numCommands = 1;
    char* commands[MAX_ARGUMENTS];
    commands[0] = arguments[0];

    int i;
    for (i = 1; i < argCount - 1; i++) {
        if (strcmp(arguments[i], ">") == 0) {
            // Output redirection
            if (i + 1 < argCount - 1) {
                FILE* outputFile = fopen(arguments[i + 1], "w");
                if (outputFile == NULL) {
                    perror("File open failed");
                    exit(EXIT_FAILURE);
                }
                dup2(fileno(outputFile), STDOUT_FILENO);
                fclose(outputFile);
            } else {
                fprintf(stderr, "Invalid command format\n");
                return;
            }
        } else if (strcmp(arguments[i], "<") == 0) {
            // Input redirection
            if (i + 1 < argCount - 1) {
                FILE* inputFile = fopen(arguments[i + 1], "r");
                if (inputFile == NULL) {
                    perror("File open failed");
                    exit(EXIT_FAILURE);
                }
                dup2(fileno(inputFile), STDIN_FILENO);
                fclose(inputFile);
            } else {
                fprintf(stderr, "Invalid command format\n");
                return;
            }
        } else if (strcmp(arguments[i], "|") == 0) {
            // Pipe
            if (i + 1 < argCount - 1) {
                commands[numCommands] = arguments[i + 1];
                numCommands++;
            } else {
                fprintf(stderr, "Invalid command format\n");
                return;
            }
        }
    }

    // Execute the commands in a pipeline
    int pipefd[numCommands - 1][2];
    pid_t pids[numCommands];

    for (i = 0; i < numCommands; i++) {
        if (i < numCommands - 1) {
            if (pipe(pipefd[i]) == -1) {
                perror("Pipe creation failed");
                exit(EXIT_FAILURE);
            }
        }

        pids[i] = fork();

        if (pids[i] < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else if (pids[i] == 0) {
            // Child process

            // Redirect input
            if (i > 0) {
                dup2(pipefd[i - 1][0], STDIN_FILENO);
                close(pipefd[i - 1][0]);
                close(pipefd[i - 1][1]);
            }

            // Redirect output
            if (i < numCommands - 1) {
                dup2(pipefd[i][1], STDOUT_FILENO);
                close(pipefd[i][0]);
                close(pipefd[i][1]);
            }

            execvp(commands[i], arguments);

            // execvp returns only if an error occurred
            perror("Command execution failed");
            exit(EXIT_FAILURE);
			// execvp returns only if an error occurred
            perror("Command execution failed");
            exit(EXIT_FAILURE);
        }
    }

    // Close all pipe file descriptors in the parent process
    for (i = 0; i < numCommands - 1; i++) {
        close(pipefd[i][0]);
        close(pipefd[i][1]);
    }

    // Wait for all child processes to finish
    for (i = 0; i < numCommands; i++) {
        int status;
        waitpid(pids[i], &status, 0);

        if (WIFEXITED(status)) {
            printf("Command exited with status: %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Command terminated by signal: %d\n", WTERMSIG(status));
        }
    }
}