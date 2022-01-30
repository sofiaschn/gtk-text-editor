#include "textView.h"
#include "titleBar.h"
#include "closeDialog.h"
#include <gtk/gtk.h>

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

    gtk_window_present(GTK_WINDOW(window));

    g_signal_connect(window, "close-request", G_CALLBACK(onClose), NULL);
}

int main(int argc, char **argv) {
    GtkApplication *app =
        gtk_application_new("com.gtk-text-editor", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(onActive), NULL);

    return g_application_run(G_APPLICATION(app), argc, argv);
}
