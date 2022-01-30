#include <gtk/gtk.h>

extern char *OPEN_FILE_PATH;

GtkFileChooserNative *newFileChooser(GtkWidget *window,
                                     GtkFileChooserAction action);
