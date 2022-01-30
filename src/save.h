#include <gtk/gtk.h>

void saveAction(gpointer *action, GtkWidget *window, bool saveAs);

GtkWidget *attachSaveMenu(GtkWidget *window, GtkWidget *saveButton);

GtkWidget *newSaveButton(GtkWidget *window);
