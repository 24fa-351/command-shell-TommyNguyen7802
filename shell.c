#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARG_SIZE 100

void execute_command(char *cmd);
void change_directory(char *path);
void print_working_directory();
void set_env_var(char *name, char *value);
void unset_env_var(char *name);
char *get_env_var(char *name);
void parse_and_execute(char *input);
void handle_echo(char **cmd);

int main() {
    char input[MAX_INPUT_SIZE];

    while (1) {
        printf("xsh# ");
        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
            break;
        }

        // Remove trailing newline character
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "quit") == 0 || strcmp(input, "exit") == 0) {
            break;
        }

        parse_and_execute(input);
    }

    return 0;
}

void parse_and_execute(char *input) {
    char *args[MAX_ARG_SIZE];
    char *token = strtok(input, " ");
    int arg_count = 0;

    while (token != NULL) {
        args[arg_count++] = token;
        token = strtok(NULL, " ");
    }
    args[arg_count] = NULL;

    if (arg_count == 0) {
        return;
    }

    if (strcmp(args[0], "cd") == 0) {
        if (arg_count > 1) {
            change_directory(args[1]);
        } else {
            fprintf(stderr, "xsh: cd: missing argument\n");
        }
    } else if (strcmp(args[0], "pwd") == 0) {
        print_working_directory();
    } else if (strcmp(args[0], "set") == 0) {
        if (arg_count == 3) {
            set_env_var(args[1], args[2]);
        } else {
            fprintf(stderr, "xsh: set: invalid arguments\n");
        }
    } else if (strcmp(args[0], "unset") == 0) {
        if (arg_count == 2) {
            unset_env_var(args[1]);
        } else {
            fprintf(stderr, "xsh: unset: invalid arguments\n");
        }
    } else if (strcmp(args[0], "echo") == 0) {
        handle_echo(args);
    } else {
        execute_command(input);
    }
}

void change_directory(char *path) {
    if (chdir(path) != 0) {
        perror("xsh: cd");
    }
}

void print_working_directory() {
    char cwd[MAX_INPUT_SIZE];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("xsh: pwd");
    }
}

void set_env_var(char *name, char *value) {
    if (setenv(name, value, 1) != 0) {
        perror("xsh: set");
    }
}

void unset_env_var(char *name) {
    if (unsetenv(name) != 0) {
        perror("xsh: unset");
    }
}

char *get_env_var(char *name) {
    return getenv(name);
}

void handle_echo(char *args[]) {
    for (int i = 1; args[i] != NULL; i++) {
        if (args[i][0] == '$') {
            char *env_var = get_env_var(args[i] + 1);
            if (env_var != NULL) {
                printf("%s ", env_var);
            } else {
                printf(" ");
            }
        } else {
            printf("%s ", args[i]);
        }
    }
    printf("\n");
}

void execute_command(char *cmd) {
    char *args[MAX_ARG_SIZE];
    char *token = strtok(cmd, " ");
    int arg_count = 0;

    while (token != NULL) {
        args[arg_count++] = token;
        token = strtok(NULL, " ");
    }
    args[arg_count] = NULL;

    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        execvp(args[0], args);
        perror("xsh");
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Fork failed
        perror("xsh");
    } else {
        // Parent process
        wait(NULL);
    }
}
