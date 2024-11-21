#include "system.h"
#include <stdio.h>

// Define traffic light states
typedef enum {
    RED,
    RED_YELLOW,
    GREEN,
    YELLOW,
    EMERGENCY
} TrafficLightState;

// Function prototypes
void setTrafficLights(TrafficLightState state);

// Main function
int main() {
    while (1) {
        // Read the current slider position
        int slider_value = IORD(SW_SLIDERS_BASE) & 0xF; // Read only the first 4 bits

        // Set the state based on slider input
        TrafficLightState current_state;
        switch (slider_value) {
            case 0:
                current_state = RED;
                break;
            case 1:
                current_state = RED_YELLOW;
                break;
            case 2:
                current_state = GREEN;
                break;
            case 3:
                current_state = YELLOW;
                break;
            case 4:
                current_state = EMERGENCY;
                break;
            default:
                current_state = RED; // Default to RED for undefined slider positions
                break;
        }

        // Set traffic lights based on the current state
        setTrafficLights(current_state);
    }

    return 0;
}

// Function to set traffic lights based on the state
void setTrafficLights(TrafficLightState state) {
    switch (state) {
        case RED:
            IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE, 0x003); // LEDs representing RED light
            break;
        case RED_YELLOW:
            IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE, 0x007); // LEDs representing RED + YELLOW
            break;
        case GREEN:
            IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE, 0x018); // LEDs representing GREEN light
            break;
        case YELLOW:
            IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE, 0x030); // LEDs representing YELLOW light
            break;
        case EMERGENCY:
            IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE, 0x3FF); // All LEDs ON for EMERGENCY
            break;
    }
}
