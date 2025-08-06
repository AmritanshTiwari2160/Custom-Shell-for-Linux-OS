# Custom Shell for Linux OS

This is a custom-built Linux shell developed using **C**, **Lex**, and **Yacc**, with modular support for various commands and a **Graphical User Interface (GUI)**. The shell mimics basic Unix shell behavior and includes user-defined commands like `my_ls`, `make_file`, `current_directory`, `display_content`, and more.

## Features

-  **Modular command structure** (each command handled in separate folders)
-  **Lex & Yacc based tokenization and parsing**
-  Custom implementation of shell built-ins (e.g., `ls`, `touch`, `cd`, `rm`, etc.)
-  **GUI interface** for interacting with the shell visually
-  **Easily extendable architecture for adding new commands**
-  **Can be integrated with the built-in Linux shell**

## Project Structure

```bash
.
├── commands/              # Contains folders for individual commands (each with its .l and .y files)
├── custom_shell           # Executable shell binary
├── custom_shell_gui       # Executable GUI binary
├── main.c                 # Entry point of shell
├── shell.c / shell.h      # Core shell implementation
├── custom_shell_gui.c     # GUI logic
└── Makefile               # Build instructions
