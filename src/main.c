#include "closeDialog.h"
#include "textView.h"
#include "titleBar.h"
#include "fileChooser.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

GFile *file;

static int onOpen(GApplication *app, GFile **files, int fileCount) {
    if (fileCount > 1) {
        fprintf(stderr, "ERROR: Pass only one file as argument!\n");
        exit(EXIT_FAILURE);
    }

    file = files[0];
    g_application_activate(app);

    return EXIT_SUCCESS;
}

static void onClose(GtkWidget *window) {
    if (!EDITED) {
        gtk_window_destroy(GTK_WINDOW(window));

        return;
    }

    GtkWidget *dialog = newCloseDialog(window);

    gtk_widget_show(dialog);
}

static void onActive(GtkApplication *app) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Untitled Document");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    buildTextView(window);
    buildTitleBar(window);

    if (file) {
        fileToTextBuffer(file);
        setActiveFile(window, file);
    }

    gtk_window_present(GTK_WINDOW(window));

    g_signal_connect(window, "close-request", G_CALLBACK(onClose), NULL);
}

int main(int argc, char **argv) {
    GtkApplication *app =
        gtk_application_new("com.gtk-text-editor", G_APPLICATION_HANDLES_OPEN);
    g_signal_connect(app, "open", G_CALLBACK(onOpen), NULL);
    g_signal_connect(app, "activate", G_CALLBACK(onActive), NULL);

    return g_application_run(G_APPLICATION(app), argc, argv);
}
