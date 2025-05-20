# Root Makefile

# List of command directories
COMMANDS = my_ls   # add others like touch here

# Compiler and GTK flags
CC      = gcc
CFLAGS  = `pkg-config --cflags gtk+-3.0`
LIBS    = `pkg-config --libs gtk+-3.0`

# Default: build all commands + GUI
all: custom_shell_gui

# Ensure commands are built before the GUI
custom_shell_gui: $(COMMANDS)
	$(CC) -o custom_shell_gui custom_shell_gui.c $(CFLAGS) $(LIBS)

# When you build the GUI, first build each command
$(COMMANDS):
	@$(MAKE) -C commands/$@

# Clean everything
clean:
	@for dir in $(COMMANDS); do \
		$(MAKE) -C commands/$$dir clean; \
	done
	rm -f custom_shell_gui