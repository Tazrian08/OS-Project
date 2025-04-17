# README.md

# UNIX Shell

This project implements a simple UNIX shell in C that can execute basic Linux commands, handle I/O redirection, and manage command history. 

## Features

- Command prompt display (e.g., `sh>`)
- Execution of system commands (e.g., `pwd`)
- Input and output redirection using `<`, `>`, and `>>`
- Command piping using `|`
- Support for multiple commands in-line separated by `;`
- Support for command sequences using `&&`
- Command history management
- Signal handling for interrupts (e.g., CTRL+C)

## File Structure

```
unix-shell
├── src
│   ├── main.c          # Entry point of the shell application
│   ├── shell.c         # Core functionalities of the shell
│   ├── shell.h         # Header for shell functions
│   ├── parser.c        # Command parsing logic
│   ├── parser.h        # Header for parser functions
│   ├── executor.c      # Command execution logic
│   ├── executor.h      # Header for executor functions
│   ├── history.c       # Command history management
│   ├── history.h       # Header for history functions
│   ├── signals.c       # Signal management
│   └── signals.h       # Header for signal functions
├── Makefile             # Build instructions
└── README.md            # Project documentation
```

## Building the Project

To compile the shell, navigate to the project directory and run:

```
make
```

This will generate the executable file.

## Running the Shell

After building the project, you can run the shell by executing:

```
./unix-shell
```

## Usage

Once the shell is running, you can enter commands at the prompt. Use `CTRL+C` to terminate the currently running command. 

Enjoy using your new UNIX shell!