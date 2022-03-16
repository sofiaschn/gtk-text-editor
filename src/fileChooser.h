#include <gtk/gtk.h>

extern char *OPEN_FILE_PATH;

void setActiveFile(GtkWidget *window, GFile *file);

GtkFileChooserNative *newFileChooser(GtkWidget *window,
                                     GtkFileChooserAction action);
