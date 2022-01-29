#include "textView.h"
#include <gtk/gtk.h>

char *OPEN_FILE_PATH = "";

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
        EDITED = FALSE;
        gtk_window_set_title(GTK_WINDOW(window), g_file_get_basename(file));

        OPEN_FILE_PATH = g_file_get_path(file);

        g_object_unref(file);
    }

    g_object_unref(fileChooser);
}

void saveAction(gpointer *action, bool saveAs) {
    GListModel *topLevels = gtk_window_get_toplevels();
    GtkWidget *window = g_list_model_get_item(topLevels, 0);

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

    GtkFileChooserNative *fileChooser = gtk_file_chooser_native_new(
        "Save File", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_SAVE, "Save",
        "Cancel");
    
    GtkFileFilter *fileFilter = gtk_file_filter_new();
    gtk_file_filter_add_mime_type(fileFilter, "text/*");
    gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(fileChooser), fileFilter);

    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(fileChooser), name);

    g_signal_connect(fileChooser, "response", G_CALLBACK(onFileChooserResponse),
                     window);

    gtk_native_dialog_show(GTK_NATIVE_DIALOG(fileChooser));
}

//  This function is needed since I couldn't work out how to pass the boolean
//  through the g_signal_connect data parameter.
static void saveAs(gpointer *action) { saveAction(action, TRUE); }

void openAction(gpointer *action) {
    GListModel *topLevels = gtk_window_get_toplevels();
    GtkWidget *window = g_list_model_get_item(topLevels, 0);

    GtkFileChooserNative *fileChooser = gtk_file_chooser_native_new(
        "Open File", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_OPEN, "Open",
        "Cancel");
    
    GtkFileFilter *fileFilter = gtk_file_filter_new();
    gtk_file_filter_add_mime_type(fileFilter, "text/*");
    gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(fileChooser), fileFilter);

    g_signal_connect(fileChooser, "response", G_CALLBACK(onFileChooserResponse),
                     window);

    gtk_native_dialog_show(GTK_NATIVE_DIALOG(fileChooser));
}

void buildTitleBar(GtkWidget *window) {
    GtkWidget *headerBar = gtk_header_bar_new();
    GtkWidget *saveButton = gtk_button_new_with_mnemonic("_Save");
    g_signal_connect(saveButton, "clicked", G_CALLBACK(saveAction), NULL);

    GtkWidget *saveMenuButton = gtk_menu_button_new();

    GMenu *saveMenu = g_menu_new();
    g_menu_append(saveMenu, "Save As", "win.saveAs");
    GSimpleAction *action = g_simple_action_new("saveAs", NULL);
    g_signal_connect(action, "activate", G_CALLBACK(saveAs), NULL);
    g_action_map_add_action(G_ACTION_MAP(window), (GAction *)action);

    GtkWidget *saveMenuPopover =
        gtk_popover_menu_new_from_model((GMenuModel *)saveMenu);
    gtk_menu_button_set_popover(GTK_MENU_BUTTON(saveMenuButton),
                                saveMenuPopover);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_prepend(GTK_BOX(box), saveButton);
    gtk_box_append(GTK_BOX(box), saveMenuButton);

    gtk_widget_add_css_class(box, "linked");

    gtk_header_bar_pack_start(GTK_HEADER_BAR(headerBar), box);

    GtkWidget *openButton = gtk_button_new_with_mnemonic("_Open");
    g_signal_connect(openButton, "clicked", G_CALLBACK(openAction), NULL);

    gtk_header_bar_pack_start(GTK_HEADER_BAR(headerBar), openButton);

    gtk_window_set_titlebar(GTK_WINDOW(window), headerBar);
}
