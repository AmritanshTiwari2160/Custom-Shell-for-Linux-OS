# Root Makefile for custom_shell

# List all the external command folders here
COMMANDS = my_ls make_directory calculate_expression make_file remove_file available_disk_space am_launch remove_directory current_directory vscode_open switch_to display_content my_editor


# GTK compiler flags
CC     = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0`
LIBS   = `pkg-config --libs gtk+-3.0`

# Default target: build GUI (which will build each command first)
all: custom_shell_gui

# Build the GUI binary (after building all commands)
custom_shell_gui: $(COMMANDS)
	$(CC) custom_shell_gui.c -o custom_shell_gui $(CFLAGS) $(LIBS)

# Build each command’s folder (runs its own Makefile)
$(COMMANDS):
	@$(MAKE) -C commands/$@

# Clean everything (commands + GUI)
clean:
	@for dir in $(COMMANDS); do \
	  $(MAKE) -C commands/$$dir clean; \
	done
	rm -f custom_shell_gui