#include <gtk/gtk.h>

GtkTextBuffer *textBuffer;
bool EDITED = FALSE;

const char *UTF_16_BE_BOM = "\xFE\xFF";
const char *UTF_16_LE_BOM = "\xFF\xFE";
const char *UTF_32_BE_BOM = "\x00\x00\xFE\xFF";
const char *UTF_32_LE_BOM = "\xFF\xFE\x00\x00";

//  Tries to use BOM to detect encoding. If no match is found, defaults to
//  ISO-8859-1
char *getEncoding(const char *data, size_t size) {
    if (size >= 4) {
        if (memcmp(data, UTF_32_LE_BOM, 4) == 0)
            return "UTF-32-LE";
        if (memcmp(data, UTF_32_BE_BOM, 4) == 0)
            return "UTF-32-BE";
    }
    if (size >= 2) {
        if (memcmp(data, UTF_16_LE_BOM, 2) == 0)
            return "UTF-16-LE";
        if (memcmp(data, UTF_16_BE_BOM, 2) == 0)
            return "UTF-16-BE";
    }
    return "ISO-8859-1";
}

static void onTextChange(GtkWidget *textBuffer, GtkWidget *window) {
    if (EDITED) {
        return;
    }

    EDITED = TRUE;

    const char *title = gtk_window_get_title(GTK_WINDOW(window));
    size_t titleLength = strlen(title);

    char newTitle[titleLength + 2];
    sprintf(newTitle, "* %s", title);

    gtk_window_set_title(GTK_WINDOW(window), newTitle);
}

void buildTextView(GtkWidget *window) {
    GtkWidget *scrolledWindow = gtk_scrolled_window_new();
    GtkWidget *textView = gtk_text_view_new();
    textBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));

    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(textView), 2);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(textView), 2);
    gtk_text_view_set_top_margin(GTK_TEXT_VIEW(textView), 2);
    gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(textView), 2);

    g_signal_connect(textBuffer, "changed", G_CALLBACK(onTextChange), window);

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolledWindow),
                                  textView);

    gtk_window_set_child(GTK_WINDOW(window), scrolledWindow);
}

void textBufferToFile(GFile *file) {
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(textBuffer, &start, &end);

    char *text = gtk_text_buffer_get_text(textBuffer, &start, &end, true);

    g_file_replace_contents(file, text, strlen(text), NULL, FALSE,
                            G_FILE_CREATE_NONE, NULL, NULL, NULL);
}

void fileToTextBuffer(GFile *file) {
    char *text;
    gsize len;

    g_file_load_contents(file, NULL, &text, &len, NULL, NULL);

    if (!g_utf8_validate(text, len, NULL)) {
        char *encoding = getEncoding(text, len);

        text = g_convert(text, len, "UTF-8", encoding, NULL, &len, NULL);
    }

    gtk_text_buffer_set_text(textBuffer, text, len);

    g_free(text);
}
