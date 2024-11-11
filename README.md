# Minishell

## Overview
Minishell is our custom implementation of a Unix shell, inspired by bash. This project required to mimic the intricate behavior of bash, including handling command execution, signal processing, and user interaction within a shell environment. It provides a deeper understanding of system interfacing, command parsing, and environment management, closely mimicking the core functionalities of bash.

### Key Functionalities
- **Interactive Command Prompt**: Presents a user prompt to input commands, simulating the familiar shell environment.
- **Command History**: Maintains a history of previously executed commands, allowing the user to revisit them easily.
- **Executable Search**: Executes commands based on the PATH environment variable or absolute path specification.
- **Quoting and Escaping**: Supports the use of single quotes ('), double quotes ("), and escape (\) characters, allowing users to input complex commands with arguments that contain special characters.
- **Redirections and Pipes**:
  - Standard input (`<`) and output (`>`) redirections.
  - Appending output (`>>`) and heredoc (`<<`) input redirections.
  - Piping (`|`) connects the output of one command to the input of another, facilitating complex workflows.
- **Environment and Special Variables**: Handles expansion of variables (e.g., $VAR) and manages special variables such as $? to track the exit status of the last executed command.
- **Signal Handling**: Emulates bash behavior for signals like `CTRL-C`, `CTRL-D`, and `CTRL-\`, ensuring the shell remains responsive and controlled under various conditions.
- **Builtin Commands**: Core commands like echo, cd, pwd, export, unset, env, and exit are implemented directly within Minishell for a complete shell experience.

## Built-in Commands
- **`echo`**: Prints arguments to the standard output. Supports the `-n` option to omit the trailing newline.
- **`cd`**: Changes the current working directory, crucial for navigating within the shell.
- **`pwd`**: Outputs the current directory path, providing users with contextual awareness.
- **`export`**: Sets environment variables, adding or modifying their values. Essential for configuring the shell session.
- **`unset`**: Unsets environment variables, removing them from the environment to clean up the session.
- **`env`**: Displays all current environment variables, aiding in debugging and configuration.
- **`exit`**: Allows exiting the shell with an optional status code, which is useful for scripting and command-line operations.

## Technical Architecture
Minishell's architecture is designed to be modular, ensuring a robust and maintainable codebase. The key components include:
- **Lexer**: Converts the raw user input into a series of tokens, which can be understood by the parser.
- **Parser**: Takes the tokens and forms structured commands, identifying the relationships between commands, arguments, and operators.
- **Executor**: Executes parsed commands by calling system functions or running built-in implementations.
- **Signal Handling**: Uses a dedicated global variable to ensure consistent and conflict-free handling of signals, allowing the shell to remain stable during unexpected interrupts.
- **Memory Management**: Ensures proper allocation and deallocation to avoid memory leaks, except for some acceptable leaks related to third-party libraries such as readline.

### Detailed Functionality
- **Global Variable Handling**: Utilizes a single global variable to manage signal states to prevent conflicts during signal handling.
- **Memory Management**: Careful management ensures there are no leaks, with the exception of unavoidable leaks from third-party libraries like `readline`.

## Installation
Follow these steps to install and run Minishell:
```bash
git clone https://github.com/olympiaobr/Minishell.git
cd Minishell
make
./minishell
```

## Authors
Minishell was developed collaboratively by Olimpiya Obreshkova and Jasmin Nguyen.
