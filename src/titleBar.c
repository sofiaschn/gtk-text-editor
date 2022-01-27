#include <gtk/gtk.h>
#include "textView.h"

static void onFileChooserResponse(GtkNativeDialog *fileChooser,
                                      int response, gpointer data) {
    GtkFileChooserAction action =
        gtk_file_chooser_get_action(GTK_FILE_CHOOSER(fileChooser));

    if (response == GTK_RESPONSE_ACCEPT) {
        GFile *chosenFile =
            gtk_file_chooser_get_file(GTK_FILE_CHOOSER(fileChooser));

        if (action == GTK_FILE_CHOOSER_ACTION_SAVE) {
            g_file_replace_contents(chosenFile, data, strlen(data), NULL, FALSE,
                                    G_FILE_CREATE_NONE, NULL, NULL, NULL);
        } else if (action == GTK_FILE_CHOOSER_ACTION_OPEN) {
            GtkTextBuffer *textBuffer =
                gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));

            char *content;
            gsize contentLength;

            g_file_load_contents(chosenFile, NULL, &content, &contentLength,
                                 NULL, NULL);

            gtk_text_buffer_set_text(textBuffer, content, contentLength);

            g_free(content);
        }

        g_object_unref(chosenFile);
    }

    g_object_unref(fileChooser);
}

static void saveButtonClicked(GtkWidget *saveButton, GtkWidget *window) {
    GtkTextBuffer *textBuffer =
        gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));

    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(textBuffer, &start, &end);

    char *text = gtk_text_buffer_get_text(textBuffer, &start, &end, true);

    GtkFileChooserNative *fileChooser = gtk_file_chooser_native_new(
        "Save File", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_SAVE, "Save",
        "Cancel");

    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(fileChooser),
                                      "Untitled Document");

    g_signal_connect(fileChooser, "response",
                     G_CALLBACK(onFileChooserResponse), text);

    gtk_native_dialog_show(GTK_NATIVE_DIALOG(fileChooser));
}

static void openButtonClicked(GtkWidget *saveButton, GtkWidget *window) {
    GtkFileChooserNative *fileChooser = gtk_file_chooser_native_new(
        "Open File", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_OPEN, "Open",
        "Cancel");

    g_signal_connect(fileChooser, "response",
                     G_CALLBACK(onFileChooserResponse), window);

    gtk_native_dialog_show(GTK_NATIVE_DIALOG(fileChooser));
}

void buildTitleBar(GtkWidget *window) {
    GtkWidget *headerBar = gtk_header_bar_new();
    GtkWidget *saveButton = gtk_button_new_with_label("Save");
    g_signal_connect(saveButton, "clicked", G_CALLBACK(saveButtonClicked),
                     window);

    gtk_header_bar_pack_start(GTK_HEADER_BAR(headerBar), saveButton);

    GtkWidget *openButton = gtk_button_new_with_label("Open");
    g_signal_connect(openButton, "clicked", G_CALLBACK(openButtonClicked),
                     window);

    gtk_header_bar_pack_start(GTK_HEADER_BAR(headerBar), openButton);

    gtk_window_set_titlebar(GTK_WINDOW(window), headerBar);
}
