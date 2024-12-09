# Command-Line Shell Program

This is a simple command-line shell program implemented in C. It provides basic functionalities similar to bash or PowerShell, including built-in commands, environment variable management, and support for piping, redirection, and background execution.

## Features

- **Prompt**: Displays a prompt (`xsh# `) for user input.
- **Built-in Commands**:
  - `cd <path>`: Change the current directory to the specified path.
  - `pwd`: Print the current working directory.
  - `set <VAR> <value>`: Set an environment variable.
  - `unset <VAR>`: Unset an environment variable.
  - `echo <text> $<VAR>`: Print the specified text, with support for environment variable substitution.
- **External Commands**: Executes external commands found in the system's `PATH`.
- **Piping (`|`)**: Pipe the output of one command to the input of another.
- **Input Redirection (`<`)**: Redirect the contents of a file to the input of a command.
- **Output Redirection (`>`)**: Redirect the output of a command to a file.
- **Background Execution (`&`)**: Run a command in the background.

## Usage

### Running the Shell

Compile the program using `gcc`:

```sh
gcc -o shell shell.c
```
