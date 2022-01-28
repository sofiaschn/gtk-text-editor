#include <gtk/gtk.h>

GtkTextBuffer *textBuffer;

void buildTextView(GtkWidget *window) {
    GtkWidget *scrolledWindow = gtk_scrolled_window_new();
    GtkWidget *textView = gtk_text_view_new();
    textBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolledWindow),
                                  textView);

    gtk_window_set_child(GTK_WINDOW(window), scrolledWindow);
}

void textBufferToFile(GFile *file) {
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(textBuffer, &start, &end);

    char *text = gtk_text_buffer_get_text(textBuffer, &start, &end, true);

    g_file_replace_contents(file, text, strlen(text), NULL, FALSE,
                            G_FILE_CREATE_NONE, NULL, NULL, NULL);
}

void fileToTextBuffer(GFile *file) {
    char *content;
    gsize contentLength;

    g_file_load_contents(file, NULL, &content, &contentLength, NULL, NULL);

    gtk_text_buffer_set_text(textBuffer, content, contentLength);

    g_free(content);
}