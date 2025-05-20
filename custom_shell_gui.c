#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // chdir, getcwd
#include <limits.h>     // PATH_MAX

GtkWidget *text_view;
GtkWidget *entry;
char base_dir[PATH_MAX];  // where we launched the GUI

// Utility to dump text into the output view
static void show_output(const char *out) {
    GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_set_text(buf, out, -1);
}

// Runs a shell command, capturing stdout+stderr
static void run_external(const char *cmd) {
    char line[512], *result = NULL;
    size_t cap = 0;
    FILE *fp = popen(cmd, "r");
    if (!fp) {
        show_output("Error running command\n");
        return;
    }
    while (fgets(line, sizeof(line), fp)) {
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

// Handle switch_to as a builtin (just like cd)
static void handle_switch_to(const char *arg) {
    char msg[PATH_MAX + 64];
    if (chdir(arg) == 0) {
        char cwd[PATH_MAX];
        getcwd(cwd, sizeof(cwd));
        snprintf(msg, sizeof(msg), "Directory changed to: %s\n", cwd);
    } else {
        snprintf(msg, sizeof(msg), 
                 "switch_to: no such file or directory: %s\n", arg);
    }
    show_output(msg);
}

// Called when Enter is pressed
static void on_enter(GtkWidget *w, gpointer data) {
    const char *input = gtk_entry_get_text(GTK_ENTRY(entry));

    if (strncmp(input, "switch_to ", 10) == 0) {
        // use switch_to instead of cd
        handle_switch_to(input + 10);
    }
    else if (strncmp(input, "my_ls", 5) == 0) {
        // extract optional argument
        const char *arg = strchr(input, ' ');
        char cmd[PATH_MAX + 128];
        if (arg) {
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/my_ls/my_ls_exec\" %s 2>&1",
                     base_dir, arg + 1);
        } else {
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/my_ls/my_ls_exec\" . 2>&1",
                     base_dir);
        }
        run_external(cmd);
    }
    else if (strncmp(input, "current_directory", 17) == 0) {
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd))) {
            char msg[PATH_MAX + 64];
            snprintf(msg, sizeof(msg), "Current directory: %s\n", cwd);
            show_output(msg);
        } else {
            show_output("Error getting current directory\n");
        }
    }
    else if (strncmp(input, "make_file", 9) == 0) {
        const char *arg = strchr(input, ' ');
        char cmd[PATH_MAX + 128];
        if (arg) {
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/make_file/make_file_exec\" %s 2>&1",
                     base_dir, arg + 1);
        } else {
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/make_file/make_file_exec\" 2>&1",
                     base_dir);
        }
        run_external(cmd);
    }
    else if (strncmp(input, "display_content", 15) == 0) {
        const char *arg = strchr(input, ' ');
        char cmd[PATH_MAX + 128];
        if (arg) {
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/display_content/display_content_exec\" %s 2>&1",
                     base_dir, arg + 1);
        } else {
            snprintf(cmd, sizeof(cmd),
                     "\"%s/commands/display_content/display_content_exec\" 2>&1",
                     base_dir);
        }
        run_external(cmd);
    }
    else if (strncmp(input, "remove_dir", 10) == 0) {
    const char *arg = strchr(input, ' ');
    char cmd[PATH_MAX + 128];
    if (arg) {
        snprintf(cmd, sizeof(cmd),
                 "\"%s/commands/remove_dir/remove_dir_exec\" %s 2>&1",
                 base_dir, arg + 1);
    } else {
        snprintf(cmd, sizeof(cmd),
                 "\"%s/commands/remove_dir/remove_dir_exec\" 2>&1",
                 base_dir);
    }
    run_external(cmd);
}
    else {
        // any other system command
        char *cmd;
        if (asprintf(&cmd, "%s 2>&1", input) == -1) {
            show_output("Error building command string\n");
        } else {
            run_external(cmd);
            free(cmd);
        }
    }

    gtk_entry_set_text(GTK_ENTRY(entry), "");  // clear input
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

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

    // Entry field
    entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);
    g_signal_connect(entry, "activate", G_CALLBACK(on_enter), NULL);

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