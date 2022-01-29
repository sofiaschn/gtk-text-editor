#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

GtkTextBuffer *textBuffer;
bool EDITED = FALSE;

static void onTextChange(GtkWidget *textBuffer) {
    if (EDITED) {
        return;
    }

    EDITED = TRUE;

    GListModel *topLevels = gtk_window_get_toplevels();
    GtkWidget *window = g_list_model_get_item(topLevels, 0);

    const char *title = gtk_window_get_title(GTK_WINDOW(window));
    size_t titleLength = strlen(title);

    char newTitle[titleLength + 2];
    sprintf(newTitle, "* %s", title);

    gtk_window_set_title(GTK_WINDOW(window), newTitle);
}

void buildTextView(GtkWidget *window) {
    GtkWidget *scrolledWindow = gtk_scrolled_window_new();
    GtkWidget *textView = gtk_text_view_new();
    textBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));

    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(textView), 2);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(textView), 2);
    gtk_text_view_set_top_margin(GTK_TEXT_VIEW(textView), 2);
    gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(textView), 2);

    g_signal_connect(textBuffer, "changed", G_CALLBACK(onTextChange), NULL);

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