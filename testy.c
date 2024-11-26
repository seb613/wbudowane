#include <stdio.h>
#include "definitions.h"
#include <gtk-3.0/gtk/gtk.h>
#include <glib-2.0/glib.h>


int sliders = 0;
int leds = 256;
int hex = 512;

#define SLIDERS_BASE &sliders
#define LEDS_BASE &leds
#define HEX_BASE &hex


int main(int argc, char *argv[]) {
    printf("sliders: %d\n", SLIDERS_BASE);
    printf("leds: %d\n", LEDS_BASE);
    printf("hex: %d\n", HEX_BASE);
    return 0;
}
