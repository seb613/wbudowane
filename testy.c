#include <stdio.h>
#include <GTK/gtk.h>
#include "definitions.h"

int sliders = 0;
int leds = 256;
int hex = 512;

#define SLIDERS_BASE &sliders
#define LEDS_BASE &leds
#define HEX_BASE &hex

void on_slider_value_changed(GtkRange *range, gpointer user_data) {
    sliders = (int)gtk_range_get_value(range);
    printf("Sliders: %d\n", sliders);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Slider Example");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 100);

    GtkWidget *slider = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 1024, 1);
    g_signal_connect(slider, "value-changed", G_CALLBACK(on_slider_value_changed), NULL);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(box), slider, TRUE, TRUE, 0);

    gtk_container_add(GTK_CONTAINER(window), box);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
