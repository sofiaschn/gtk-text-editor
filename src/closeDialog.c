#include "save.h"
#include <gtk/gtk.h>

static void onDialogReponse(GtkWidget *dialog, int response,
                            GtkWidget *window) {
    if (response == GTK_RESPONSE_CLOSE) {
        gtk_window_destroy(GTK_WINDOW(window));
    } else if (response == GTK_RESPONSE_OK) {
        saveAction((gpointer *)dialog, window, TRUE);
    }

    gtk_window_destroy(GTK_WINDOW(dialog));
}

GtkWidget *newCloseDialog(GtkWidget *window) {
    GtkWidget *dialog = gtk_message_dialog_new(
        GTK_WINDOW(window), GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING,
        GTK_BUTTONS_NONE, "You have changes that have not been saved.");

    GtkWidget *closeButton = gtk_dialog_add_button(
        GTK_DIALOG(dialog), "Close without saving", GTK_RESPONSE_CLOSE);

    gtk_widget_add_css_class(closeButton, "destructive-action");

    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog),
                                             "Do you want to save them?");

    gtk_dialog_add_buttons(GTK_DIALOG(dialog), "Cancel", GTK_RESPONSE_CANCEL,
                           "Save As...", GTK_RESPONSE_OK, NULL);

    g_signal_connect(dialog, "response", G_CALLBACK(onDialogReponse), window);

    return dialog;
}