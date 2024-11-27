#include "textView.h"
#include <gtk/gtk.h>

char *OPEN_FILE_PATH = "";

void setActiveFile(GtkWidget *window, GFile *file) {
    EDITED = FALSE;
    gtk_window_set_title(GTK_WINDOW(window), g_file_get_basename(file));

    OPEN_FILE_PATH = g_file_get_path(file);

    g_object_unref(file);
}

static void onFileChooserResponse(GtkNativeDialog *fileChooser, int response,
                                  GtkWidget *window) {
    GtkFileChooserAction action =
        gtk_file_chooser_get_action(GTK_FILE_CHOOSER(fileChooser));

    if (response == GTK_RESPONSE_ACCEPT) {
        GFile *file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(fileChooser));

        if (action == GTK_FILE_CHOOSER_ACTION_SAVE) {
            textBufferToFile(file);
        } else if (action == GTK_FILE_CHOOSER_ACTION_OPEN) {
            fileToTextBuffer(file);
        }

        setActiveFile(window, file);
    }

    g_object_unref(fileChooser);
}

GtkFileChooserNative *newFileChooser(GtkWidget *window,
                                     GtkFileChooserAction action) {
    char *actionName = action == GTK_FILE_CHOOSER_ACTION_SAVE ? "Save" : "Open";
    char title[10];
    sprintf(title, "%s File", actionName);

    GtkFileChooserNative *fileChooser = gtk_file_chooser_native_new(
        title, GTK_WINDOW(window), action, actionName, "Cancel");

    GtkFileFilter *fileFilter = gtk_file_filter_new();
    gtk_file_filter_add_mime_type(fileFilter, "text/*");
    gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(fileChooser), fileFilter);

    g_signal_connect(fileChooser, "response", G_CALLBACK(onFileChooserResponse),
                     window);

    return fileChooser;
}
