#include <gtk/gtk.h>

void buildTextView(GtkWidget *window) {
    GtkWidget *textView = gtk_text_view_new();
    GtkTextBuffer *textBuffer =
        gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));

    gtk_window_set_child(GTK_WINDOW(window), textView);
}
