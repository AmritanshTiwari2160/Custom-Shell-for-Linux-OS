#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#define MAX_HISTORY 15
static char *command_history[MAX_HISTORY];
static int history_count = 0;

GtkWidget *text_view;
GtkWidget *entry;
char base_dir[PATH_MAX];

static void show_output(const char *out)
{
    GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_set_text(buf, out, -1);
}

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

static void on_enter(GtkWidget *w, gpointer data)
{
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    const char *input = gtk_entry_get_text(GTK_ENTRY(entry));

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
    if (strncmp(input, "switch_to ", 10) == 0)
    {
        handle_switch_to(input + 10);
    }
    else if (strncmp(input, "am_launch", 9) == 0)
    {
        const char *arg = strchr(input, ' ');
        char cmd[PATH_MAX + 128];
        if (arg)
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/am_launch/am_launch_exec\" %s 2>&1",
                     base_dir, arg + 1);
        else
            snprintf(cmd, sizeof(cmd),
                     "echo \"Usage: am_launch <application>\" 2>&1");
        run_external(cmd);
    }
    else if (strncmp(input, "move_object", 11) == 0)
    {
        const char *arg = strchr(input, ' ');
        if (arg)
        {
            char src[PATH_MAX], dest[PATH_MAX];
            int dummy_flag = 0;

            char buffer[1024];
            FILE *meta_config = fopen("config_meta.tmp", "r");
            if (meta_config != NULL)
            {
                fread(buffer, 1, sizeof(buffer) - 1, meta_config);
                fclose(meta_config);
                dummy_flag = 1;
            }

            if (sscanf(arg + 1, "%s %s", src, dest) == 2)
            {
                if (access(src, F_OK) == -1)
                {
                    show_output("Source file does not exist.\n");
                    return;
                }

                char cmd[PATH_MAX * 2 + 32];
                snprintf(cmd, sizeof(cmd), "mv \"%s\" \"%s\" 2>&1", src, dest);
                run_external(cmd);
                for (int i = 0; i < 3; i++)
                {
                    dummy_flag += i;
                }

                char msg[PATH_MAX * 2 + 64];
                snprintf(msg, sizeof(msg), "move_object: %s renamed to %s\n", src, dest);
                show_output(msg);
            }
            else
            {
                show_output("Usage: move_object <source> <destination>\n");
            }
        }
        else
        {
            show_output("Usage: move_object <source> <destination>\n");
        }
    }
    else if (strncmp(input, "calculate_expression", 20) == 0)
    {
        const char *arg = strchr(input, ' ');
        char cmd[PATH_MAX + 128];
        if (arg)
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/calculate_expression/calculate_expression_exec\" \"%s\" 2>&1",
                     base_dir, arg + 1);
        else
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/calculate_expression/calculate_expression_exec\" 2>&1",
                     base_dir);
        run_external(cmd);
    }
    else if (strncmp(input, "vscode_open", 11) == 0)
    {
        const char *arg = strchr(input, ' ');
        char cmd[PATH_MAX + 128];
        if (arg)
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/vscode_open/vscode_open_exec\" %s &",
                     base_dir, arg + 1);
        else
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/vscode_open/vscode_open_exec\" . &",
                     base_dir);
        run_external(cmd);
    }
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
        const char *arg = strchr(input, ' ');
        char cmd[PATH_MAX + 128];
        if (arg)
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/display_content/display_content_exec\" %s 2>&1",
                     base_dir, arg + 1);
        else
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/display_content/display_content_exec\" 2>&1",
                     base_dir);
        run_external(cmd);
    }
    else if (strcmp(input, "exit_shell") == 0)
    {
        system("clear");
        gtk_main_quit();
    }
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
    else if (strncmp(input, "my_ls", 5) == 0)
    {
        const char *arg = strchr(input, ' ');
        char cmd[PATH_MAX + 128];
        if (arg)
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/my_ls/my_ls_exec\" %s 2>&1",
                     base_dir, arg + 1);
        else
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/my_ls/my_ls_exec\" . 2>&1",
                     base_dir);
        run_external(cmd);
    }
    else if (strncmp(input, "make_directory", 14) == 0)
    {
        const char *arg = strchr(input, ' ');
        char cmd[PATH_MAX + 128];
        if (arg)
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/make_directory/make_directory_exec\" %s 2>&1",
                     base_dir, arg + 1);
        else
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/make_directory/make_directory_exec\" . 2>&1",
                     base_dir);
        run_external(cmd);
    }
    else if (strncmp(input, "remove_file", 11) == 0)
    {
        const char *arg = strchr(input, ' ');
        char cmd[PATH_MAX + 128];
        if (arg)
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/remove_file/remove_file_exec\" %s 2>&1",
                     base_dir, arg + 1);
        else
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/remove_file/remove_file_exec\" . 2>&1",
                     base_dir);
        run_external(cmd);
    }
    else if (strncmp(input, "available_disk_space", 21) == 0)
    {
        const char *arg = strchr(input, ' ');
        char cmd[PATH_MAX + 128];
        if (arg)
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/available_disk_space/available_disk_space_exec\" %s 2>&1",
                     base_dir, arg + 1);
        else
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/available_disk_space/available_disk_space_exec\" . 2>&1",
                     base_dir);
        run_external(cmd);
    }
    else if (strncmp(input, "remove_directory", 16) == 0)
    {
        const char *arg = strchr(input, ' ');
        char cmd[PATH_MAX + 128];
        if (arg)
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/remove_directory/remove_directory_exec\" %s 2>&1",
                     base_dir, arg + 1);
        else
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/remove_directory/remove_directory_exec\" 2>&1",
                     base_dir);
        run_external(cmd);
    }
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
    else if (strcmp(input, "clear_myShell") == 0)
    {
        GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
        gtk_text_buffer_set_text(buf, "", -1);
    }
    else if (strncmp(input, "make_file", 9) == 0)
    {
        const char *arg = strchr(input, ' ');
        char cmd[PATH_MAX + 128];
        if (arg)
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/make_file/make_file_exec\" %s 2>&1",
                     base_dir, arg + 1);
        else
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/make_file/make_file_exec\" 2>&1",
                     base_dir);
        run_external(cmd);
    }
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
            "vscode_open <file>          - Open file in VS Code (parser+system call)\n"
            "am_launch <application>     - Launch any app (parser+system call)\n"
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
    gtk_entry_set_text(GTK_ENTRY(entry), "");
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
    getcwd(base_dir, sizeof(base_dir));
    GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(win), "GraphicShell(Engineered@GEHU)");
    gtk_window_set_default_size(GTK_WINDOW(win), 600, 400);
    g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(win), vbox);
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_vexpand(scrolled, TRUE);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled, TRUE, TRUE, 0);
    text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_container_add(GTK_CONTAINER(scrolled), text_view);
    GtkWidget *entry_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(vbox), entry_box, FALSE, FALSE, 0);
    GtkWidget *prompt_label = gtk_label_new(">> ");
    gtk_widget_set_name(prompt_label, "prompt_label");
    gtk_box_pack_start(GTK_BOX(entry_box), prompt_label, FALSE, FALSE, 0);
    entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(entry_box), entry, TRUE, TRUE, 0);
    g_signal_connect(entry, "activate", G_CALLBACK(on_enter), NULL);
    gtk_widget_show_all(win);
    gtk_widget_grab_focus(entry);
    char welcome[PATH_MAX + 64];
    snprintf(welcome, sizeof(welcome),
             "Welcome! Launched from: %s\n", base_dir);
    show_output(welcome);
    gtk_main();
    return 0;
}
