#include "mysh.h"

int executeCommand(const char * command) {
	if (strcmp(command, "cd") == 0) {
        // Handle 'cd' command
        char directory[COMMAND_LENGTH];
        printf("Enter directory: ");
        fgets(directory, COMMAND_LENGTH, stdin);

        // Remove newline character from directory
        directory[strcspn(directory, "\n")] = '\0';

        if (chdir(directory) != 0) {
            perror("cd failed");
            return -1;
        }

        return 0;
    } else if (strcmp(command, "exit") == 0) {
        // Handle 'exit' command
        exit(EXIT_SUCCESS);
    }

	 // Fork a child process
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return -1;
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
            return WEXITSTATUS(status);
        } else if (WIFSIGNALED(status)) {
            printf("Command terminated by signal: %d\n", WTERMSIG(status));
            return -1;
        }
    }

	return 0;
}