#include <gtk/gtk.h>

static void saveButtonClicked(GtkWidget *saveButton, gpointer data) {
    // TODO: cycle through all widgets and find the GtkTextView.
    GtkWidget *textView = gtk_widget_get_first_child(data);
    GtkTextBuffer *textBuffer =
        gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));

    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(textBuffer, &start, &end);

    char *text = gtk_text_buffer_get_text(textBuffer, &start, &end, true);

    FILE *file = fopen("file.txt", "w");
    fprintf(file, "%s", text);
    fclose(file);
}

static void buildTitleBar(GtkWidget *window) {
    GtkWidget *headerBar = gtk_header_bar_new();
    GtkWidget *saveButton = gtk_button_new_with_label("Save");
    g_signal_connect(saveButton, "clicked", G_CALLBACK(saveButtonClicked),
                     window);

    gtk_header_bar_pack_start(GTK_HEADER_BAR(headerBar), saveButton);

    gtk_window_set_titlebar(GTK_WINDOW(window), headerBar);
}

static void buildTextView(GtkWidget *window) {
    GtkWidget *textView = gtk_text_view_new();
    GtkTextBuffer *textBuffer =
        gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));

    gtk_window_set_child(GTK_WINDOW(window), textView);
}

static void onActive(GtkApplication *app) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Text Editor");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    buildTextView(window);
    buildTitleBar(window);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app =
        gtk_application_new("com.gtk-text-editor", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(onActive), NULL);

    return g_application_run(G_APPLICATION(app), argc, argv);
}
