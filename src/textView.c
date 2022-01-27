#include <gtk/gtk.h>

GtkWidget *textView;

void buildTextView(GtkWidget *window) {
    GtkWidget *scrolledWindow = gtk_scrolled_window_new();
    textView = gtk_text_view_new();
    GtkTextBuffer *textBuffer =
        gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolledWindow),
                                  textView);

    gtk_window_set_child(GTK_WINDOW(window), scrolledWindow);
}
