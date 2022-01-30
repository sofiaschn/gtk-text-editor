#include "fileChooser.h"
#include "textView.h"
#include <gtk/gtk.h>

void saveAction(gpointer *action, GtkWidget *window, bool saveAs) {
    char *name = "Untitled Document";
    if (OPEN_FILE_PATH[0]) {
        GFile *file = g_file_new_for_path(OPEN_FILE_PATH);
        name = g_file_get_basename(file);
        if (!saveAs) {
            textBufferToFile(file);

            EDITED = false;
            gtk_window_set_title(GTK_WINDOW(window), name);

            g_object_unref(file);
            return;
        }
    }

    GtkFileChooserNative *fileChooser =
        newFileChooser(window, GTK_FILE_CHOOSER_ACTION_SAVE);
    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(fileChooser), name);

    gtk_native_dialog_show(GTK_NATIVE_DIALOG(fileChooser));
}

//  This function is needed since I couldn't work out how to pass the boolean
//  through the g_signal_connect data parameter.
static void saveAs(gpointer *action, GtkWidget *window) {
    saveAction(action, window, TRUE);
}

GtkWidget *attachSaveMenu(GtkWidget *window, GtkWidget *saveButton) {
    GtkWidget *menuButton = gtk_menu_button_new();

    GMenu *saveMenu = g_menu_new();

    g_menu_append(saveMenu, "Save As", "win.saveAs");
    GSimpleAction *action = g_simple_action_new("saveAs", NULL);
    g_signal_connect(action, "activate", G_CALLBACK(saveAs), window);
    g_action_map_add_action(G_ACTION_MAP(window), (GAction *)action);

    GtkWidget *menuPopover =
        gtk_popover_menu_new_from_model((GMenuModel *)saveMenu);
    gtk_menu_button_set_popover(GTK_MENU_BUTTON(menuButton), menuPopover);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_prepend(GTK_BOX(box), saveButton);
    gtk_box_append(GTK_BOX(box), menuButton);

    gtk_widget_add_css_class(box, "linked");

    return box;
}

GtkWidget *newSaveButton(GtkWidget *window) {
    GtkWidget *saveButton = gtk_button_new_with_mnemonic("_Save");

    g_signal_connect(saveButton, "clicked", G_CALLBACK(saveAction), window);

    return saveButton;
}
