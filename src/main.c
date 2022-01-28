#include "textView.h"
#include "titleBar.h"
#include <gtk/gtk.h>

static void onDialogReponse(GtkWidget *dialog, int response) {
    GListModel *topLevels = gtk_window_get_toplevels();
    GtkWidget *window = g_list_model_get_item(topLevels, 0);

    if (response == GTK_RESPONSE_CLOSE) {
        gtk_window_destroy(GTK_WINDOW(window));
    } else if (response == GTK_RESPONSE_OK) {
        saveAction((gpointer *)dialog, TRUE);
    }

    gtk_window_destroy(GTK_WINDOW(dialog));
}

static void onClose(GtkWidget *window) {
    GtkWidget *dialog = gtk_message_dialog_new(
        GTK_WINDOW(window), GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING,
        GTK_BUTTONS_NONE, "You have changes that have not been saved.");

    GtkWidget *closeButton = gtk_dialog_add_button(
        GTK_DIALOG(dialog), "Close without saving", GTK_RESPONSE_CLOSE);

    GtkStyleContext *context = gtk_widget_get_style_context(closeButton);
    gtk_style_context_add_class(context, "destructive-action");

    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog),
                                             "Do you want to save them?");

    gtk_dialog_add_buttons(GTK_DIALOG(dialog), "Cancel", GTK_RESPONSE_CANCEL,
                           "Save As...", GTK_RESPONSE_OK, NULL);

    g_signal_connect(dialog, "response", G_CALLBACK(onDialogReponse), NULL);

    gtk_widget_show(dialog);
}

static void onActive(GtkApplication *app) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Text Editor");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    buildTextView(window);
    buildTitleBar(window);

    gtk_window_present(GTK_WINDOW(window));

    g_signal_connect(window, "close-request", G_CALLBACK(onClose), NULL);
}

int main(int argc, char **argv) {
    GtkApplication *app =
        gtk_application_new("com.gtk-text-editor", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(onActive), NULL);

    return g_application_run(G_APPLICATION(app), argc, argv);
}
