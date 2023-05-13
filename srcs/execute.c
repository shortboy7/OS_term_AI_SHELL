#include "mysh.h"

void changeDirectory(char** arguments) {
    if (arguments[1] == NULL) {
        // No directory provided, go to home directory
        const char* homeDir = getenv("HOME");
        if (homeDir != NULL) {
            if (chdir(homeDir) != 0) {
                perror("chdir failed");
            }
        } else {
            fprintf(stderr, "Home directory not found\n");
        }
    } else {
        // Change to the specified directory
        if (chdir(arguments[1]) != 0) {
            perror("chdir failed");
        }
    }
}

void executeCommand(char** arguments) {
    int i;
    int numCommands = 1;

	int commandIdx[MAX_ARGUMENTS];
    char* commands[MAX_ARGUMENTS];
    commands[0] = arguments[0];
	commandIdx[0] = 0;

	// Count the number of
	int argumentIdx;
	for (argumentIdx = 1; arguments[argumentIdx] != NULL; argumentIdx++) {
    	if (strcmp(arguments[argumentIdx], "|") == 0) {
			if (arguments[argumentIdx + 1] == NULL) {
				fprintf(stderr, "Invalid command format\n");
				exit(EXIT_FAILURE);
			}
			commandIdx[numCommands] = argumentIdx + 1;
			commands[numCommands] = arguments[argumentIdx + 1];
			numCommands++;
    	}
	}
	commandIdx[numCommands] = argumentIdx;

    int pipefd[MAX_ARGUMENTS - 1][2];
    pid_t pids[MAX_ARGUMENTS];

    // Create pipes
    for (i = 0; i < numCommands - 1; i++) {
        if (pipe(pipefd[i]) == -1) {
            perror("Pipe creation failed");
            exit(EXIT_FAILURE);
        }
    }
    for (i = 0; i < numCommands; i++) {
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

			// Gather arguments for the command to be executed
            char* commandArguments[MAX_ARGUMENTS];
            int argIndex = 0;

            // 명렁어 전전 argument or 마지막 argument까지
            // if commandIdx[i + 1] == argumentIdx -> 마지막 argument까지
            // else commandIdx[i + 1] - 1 까지
            int bound = (commandIdx[i + 1] == argumentIdx) ? commandIdx[i + 1] : commandIdx[i + 1] - 1;
            int redirectSymbolIdx = -1;
            for (int j = commandIdx[i]; j < bound; j++) {
                if (strcmp(arguments[j], ">") == 0 || strcmp(arguments[j], "<") == 0) {
					redirectSymbolIdx = j;
                    break;
                } else {
                    commandArguments[argIndex] = arguments[j];
                    argIndex++;
                }
            }
            commandArguments[argIndex] = NULL;

            if (redirectSymbolIdx != - 1) {
                for (int redx = redirectSymbolIdx; redx < bound; redx++) {
                    // Handle redirection symbols
                    if (strcmp(arguments[redx], ">") == 0) {
                        // Output redirection
                        if (redx + 1 < argumentIdx) {
                            int outputFile = open(arguments[redx + 1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
                            if (outputFile == -1) {
                                perror("File open failed");
                                exit(EXIT_FAILURE);
                            }
                            dup2(outputFile, STDOUT_FILENO);
                            close(outputFile);
                            redx += 1;
                        } else {
                            fprintf(stderr, "Invalid command format\n");
                            exit(EXIT_FAILURE);
                        }
                    } else if (strcmp(arguments[redx], "<") == 0) {
                        // Input redirection
                        if (redx + 1 < argumentIdx) {
                            int inputFile = open(arguments[redx + 1], O_RDONLY);
                            if (inputFile == -1) {
                                perror("File open failed");
                                exit(EXIT_FAILURE);
                            }
                            dup2(inputFile, STDIN_FILENO);
                            close(inputFile);
                            redx += 1;
                        } else {
                            fprintf(stderr, "Invalid command format\n");
                            exit(EXIT_FAILURE);
                        }
                    }
                }
            }

            // Execute the command
            execvp(commands[i], commandArguments);

            // execvp returns only if an error occurred
            perror("Command execution failed");
            exit(EXIT_FAILURE);
        }else {
            // Parent process
            int status;
            if (i > 0) {
                close(pipefd[i - 1][0]);
                close(pipefd[i - 1][1]);
            }
            waitpid(pids[i], &status, 0);
             if (WIFEXITED(status)) {
                ("Command exited with status: %d\n", WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                printf("Command terminated");
            } else if (WIFSIGNALED(status)) {
                printf("Command terminated by signal: %d\n", WTERMSIG(status));
    	    }
        }
    }

    // Close pipe file descriptors in the parent process
    for (i = 0; i < numCommands - 1; i++) {
        close(pipefd[i][0]);
        close(pipefd[i][1]);
    }

    // // Wait for all child processes to finish
    // for (i = 0; i < numCommands; i++) {
    //     int status;
    //     waitpid(pids[i], &status, 0);

    //     if (WIFEXITED(status)) {
    //         printf("Command exited with status: %d\n", WEXITSTATUS(status));
    //     } else if (WIFSIGNALED(status)) {
    //         printf("Command terminated");
	// 	} else if (WIFSIGNALED(status)) {
    //     	printf("Command terminated by signal: %d\n", WTERMSIG(status));
    // 	}
	// }
}
