#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // chdir, getcwd
#include <limits.h> // PATH_MAX

// ─── ADDITION: Command history globals ─────────────────────────────────────
#define MAX_HISTORY 15
static char *command_history[MAX_HISTORY];
static int history_count = 0;
// ─────────────────────────────────────────────────────────────────────────────

GtkWidget *text_view;
GtkWidget *entry;
char base_dir[PATH_MAX]; // where we launched the GUI

// Utility to dump text into the output view
static void show_output(const char *out)
{
    GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_set_text(buf, out, -1);
}

// Runs a shell command, capturing stdout+stderr
static void run_external(const char *cmd)
{
    char line[512], *result = NULL;
    size_t cap = 0;
    FILE *fp = popen(cmd, "r");
    if (!fp)
    {
        show_output("Error running command\n");
        return;
    }
    while (fgets(line, sizeof(line), fp))
    {
        size_t len = strlen(line);
        result = realloc(result, cap + len + 1);
        memcpy(result + cap, line, len);
        cap += len;
        result[cap] = '\0';
    }
    pclose(fp);
    show_output(result ? result : "");
    free(result);
}

// Built-in switch_to (changes the GUI’s own CWD)
static void handle_switch_to(const char *arg)
{
    char msg[PATH_MAX + 64];
    if (chdir(arg) == 0)
    {
        char cwd[PATH_MAX];
        getcwd(cwd, sizeof(cwd));
        snprintf(msg, sizeof(msg), "Directory changed to: %s\n", cwd);
    }
    else
    {
        snprintf(msg, sizeof(msg),
                 "switch_to: no such file or directory: %s\n", arg);
    }
    show_output(msg);
}

// Called when Enter is pressed
static void on_enter(GtkWidget *w, gpointer data)
{
    const char *input = gtk_entry_get_text(GTK_ENTRY(entry));

    // ─── ADDITION: Store command in history ────────────────────────────────
    if (history_count < MAX_HISTORY)
    {
        command_history[history_count++] = strdup(input);
    }
    else
    {
        free(command_history[0]);
        memmove(&command_history[0], &command_history[1],
                (MAX_HISTORY - 1) * sizeof(char *));
        command_history[MAX_HISTORY - 1] = strdup(input);
    }
    // ─────────────────────────────────────────────────────────────────────────

    // ─── BUILT-IN switch_to ────────────────────────────────────────────────
    if (strncmp(input, "switch_to ", 10) == 0)
    {
        handle_switch_to(input + 10);
    }
    // ─────────────────────────────────────────────────────────────────────────

    // ─── BUILT-IN current_directory ────────────────────────────────────────
    else if (strcmp(input, "current_directory") == 0)
    {
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)))
        {
            char msg[PATH_MAX + 64];
            snprintf(msg, sizeof(msg), "%s\n", cwd);
            show_output(msg);
        }
        else
        {
            show_output("current_directory: error getting cwd\n");
        }
    }

    else if (strncmp(input, "display_content", 15) == 0)
    {
        // show file contents via your Flex/Bison display_content_exec
        const char *arg = strchr(input, ' ');
        char cmd[PATH_MAX + 128];
        if (arg)
        {
            // user provided a filename
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/display_content/display_content_exec\" %s 2>&1",
                     base_dir, arg + 1);
        }
        else
        {
            // no filename → invoke with no args (you might print a usage message)
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/display_content/display_content_exec\" 2>&1",
                     base_dir);
        }
        run_external(cmd);
    }

    else if (strcmp(input, "exit_shell") == 0)
    {
        system("clear"); // optional
        gtk_main_quit(); // exits GUI shell
    }
    // ─────────────────────────────────────────────────────────────────────────
    else if (strncmp(input, "my_editor", 9) == 0)
    {
        const char *arg = strchr(input, ' ');
        if (arg)
        {
            char cmd[PATH_MAX + 128];
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/my_editor/my_editor_exec\" %s &",
                     base_dir, arg + 1);
            system(cmd);
        }
        else
        {
            show_output("Usage: my_editor <filename>\n");
        }
    }

    else if (strncmp(input, "display_content", 15) == 0)
    {
        // show file contents via your Flex/Bison display_content_exec
        const char *arg = strchr(input, ' ');
        char cmd[PATH_MAX + 128];
        if (arg)
        {
            // user provided a filename
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/display_content/display_content_exec\" %s 2>&1",
                     base_dir, arg + 1);
        }
        else
        {
            // no filename → invoke with no args (you might print a usage message)
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/display_content/display_content_exec\" 2>&1",
                     base_dir);
        }
        run_external(cmd);
    }
    // All other commands remain as external calls
    else if (strncmp(input, "my_ls", 5) == 0)
    {
        const char *arg = strchr(input, ' ');
        char cmd[PATH_MAX + 128];
        if (arg)
        {
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/my_ls/my_ls_exec\" %s 2>&1",
                     base_dir, arg + 1);
        }
        else
        {
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/my_ls/my_ls_exec\" . 2>&1",
                     base_dir);
        }
        run_external(cmd);
    }
    else if (strncmp(input, "make_directory", 14) == 0)
    {
        const char *arg = strchr(input, ' ');
        char cmd[PATH_MAX + 128];
        if (arg)
        {
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/make_directory/make_directory_exec\" %s 2>&1",
                     base_dir, arg + 1);
        }
        else
        {
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/make_directory/make_directory_exec\" . 2>&1",
                     base_dir);
        }
        run_external(cmd);
    }
    else if (strncmp(input, "remove_file", 11) == 0)
    {
        const char *arg = strchr(input, ' ');
        char cmd[PATH_MAX + 128];
        if (arg)
        {
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/remove_file/remove_file_exec\" %s 2>&1",
                     base_dir, arg + 1);
        }
        else
        {
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/remove_file/remove_file_exec\" . 2>&1",
                     base_dir);
        }
        run_external(cmd);
    }
    else if (strncmp(input, "available_disk_space", 21) == 0)
    {
        const char *arg = strchr(input, ' ');
        char cmd[PATH_MAX + 128];
        if (arg)
        {
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/available_disk_space/available_disk_space_exec\" %s 2>&1",
                     base_dir, arg + 1);
        }
        else
        {
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/available_disk_space/available_disk_space_exec\" . 2>&1",
                     base_dir);
        }
        run_external(cmd);
    }
    else if (strncmp(input, "remove_directory", 16) == 0)
    {
        const char *arg = strchr(input, ' ');
        char cmd[PATH_MAX + 128];
        if (arg)
        {
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/remove_directory/remove_directory_exec\" %s 2>&1",
                     base_dir, arg + 1);
        }
        else
        {
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/remove_directory/remove_directory_exec\" 2>&1",
                     base_dir);
        }
        run_external(cmd);
    }

    // ─── ADDITION: command_history handler ────────────────────────────────
    else if (strncmp(input, "command_history", 15) == 0)
    {
        char history_output[4096] = "Last Commands:\n";
        for (int i = 0; i < history_count; i++)
        {
            strcat(history_output, command_history[i]);
            strcat(history_output, "\n");
        }
        show_output(history_output);
    }
    // ─────────────────────────────────────────────────────────────────────────

    // ─── ADDITION: clear_myShell handler ─────────────────────────────────
    else if (strcmp(input, "clear_myShell") == 0)
    {
        GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
        gtk_text_buffer_set_text(buf, "", -1);
    }

    else if (strncmp(input, "make_file", 9) == 0)
    {
        // create a new file via your Flex/Bison make_file_exec
        const char *arg = strchr(input, ' ');
        char cmd[PATH_MAX + 128];
        if (arg)
        {
            // user provided a filename
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/make_file/make_file_exec\" %s 2>&1",
                     base_dir, arg + 1);
        }
        else
        {
            // no filename → just invoke make_file_exec with no args
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/make_file/make_file_exec\" 2>&1",
                     base_dir);
        }
        run_external(cmd);
    }
    // ─────────────────────────────────────────────────────────────────────────

    else if (strcmp(input, "help") == 0)
    {
        const char *help_text =
            "Custom Shell Commands:\n"
            "──────────────────────────────\n"
            "my_ls [path]                 - List files/directories\n"
            "make_directory <name>       - Create a new directory\n"
            "make_file <name>            - Create a new empty file\n"
            "remove_file <name>          - Remove a file\n"
            "remove_directory <name>     - Remove a directory\n"
            "available_disk_space        - Show disk space info\n"
            "display_content <file>      - Display content of a file\n"
            "my_editor <file>            - Open file in text editor\n"
            "switch_to <path>            - Change working directory\n"
            "current_directory           - Show current working directory\n"
            "command_history             - Show last 15 commands\n"
            "clear_myShell               - Clear the output box\n"
            "help                        - Show this help message\n"
            "exit_shell                  - Exit the GUI shell\n";

        show_output(help_text);
    }

    else
    {
        // any other system command
        char *cmd;
        if (asprintf(&cmd, "%s 2>&1", input) == -1)
        {
            show_output("Error building command string\n");
        }
        else
        {
            run_external(cmd);
            free(cmd);
        }
    }

    gtk_entry_set_text(GTK_ENTRY(entry), ""); // clear input
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    GtkCssProvider *css = gtk_css_provider_new();
    gtk_css_provider_load_from_data(css,
                                    "textview, entry {"
                                    "  font-family: Monospace;"
                                    "  font-size: 14pt;"
                                    "}"
                                    "#prompt_label {"
                                    "  font-family: Monospace;"
                                    "  font-size: 18pt;"
                                    "}",
                                    -1, NULL);
    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(css),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(css);

    // Record the directory where we launched
    getcwd(base_dir, sizeof(base_dir));

    // Build GTK window
    GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(win), "Custom GUI Shell");
    gtk_window_set_default_size(GTK_WINDOW(win), 600, 400);
    g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Layout
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(win), vbox);

    // Scrollable output
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_vexpand(scrolled, TRUE);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled, TRUE, TRUE, 0);

    text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_container_add(GTK_CONTAINER(scrolled), text_view);

    // ─── REPLACE entry with prompt_label + entry ───────────────────────────
    GtkWidget *entry_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(vbox), entry_box, FALSE, FALSE, 0);

    GtkWidget *prompt_label = gtk_label_new(">> ");
    gtk_widget_set_name(prompt_label, "prompt_label");
    gtk_box_pack_start(GTK_BOX(entry_box), prompt_label, FALSE, FALSE, 0);

    entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(entry_box), entry, TRUE, TRUE, 0);
    g_signal_connect(entry, "activate", G_CALLBACK(on_enter), NULL);
    // ─────────────────────────────────────────────────────────────────────────

    gtk_widget_show_all(win);
    gtk_widget_grab_focus(entry);

    // Show starting directory
    char welcome[PATH_MAX + 64];
    snprintf(welcome, sizeof(welcome),
             "Welcome! Launched from: %s\n", base_dir);
    show_output(welcome);

    gtk_main();
    return 0;
}