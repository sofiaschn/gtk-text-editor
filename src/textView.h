#include <gtk/gtk.h>

extern bool EDITED;

void buildTextView(GtkWidget *window);

void textBufferToFile(GFile *file);

void fileToTextBuffer(GFile *file);