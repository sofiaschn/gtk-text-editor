#include "fileChooser.h"
#include "save.h"
#include <gtk/gtk.h>

static void openAction(gpointer *action, GtkWidget *window) {
    GtkFileChooserNative *fileChooser =
        newFileChooser(window, GTK_FILE_CHOOSER_ACTION_OPEN);

    gtk_native_dialog_show(GTK_NATIVE_DIALOG(fileChooser));
}

void buildTitleBar(GtkWidget *window) {
    GtkWidget *headerBar = gtk_header_bar_new();

    GtkWidget *saveButton = newSaveButton(window);
    GtkWidget *saveButtonAndMenu = attachSaveMenu(window, saveButton);

    GtkWidget *openButton = gtk_button_new_with_mnemonic("_Open");
    g_signal_connect(openButton, "clicked", G_CALLBACK(openAction), window);

    gtk_header_bar_pack_start(GTK_HEADER_BAR(headerBar), saveButtonAndMenu);
    gtk_header_bar_pack_start(GTK_HEADER_BAR(headerBar), openButton);

    gtk_window_set_titlebar(GTK_WINDOW(window), headerBar);
}
