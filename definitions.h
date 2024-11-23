//        sw_sliders
#define SW0 0x00000001
#define SW1 0x00000002
#define SW2 0x00000004
#define SW3 0x00000008
#define SW4 0x00000010
#define SW5 0x00000020
#define SW6 0x00000040
#define SW7 0x00000080
#define SW8 0x00000100
#define SW9 0x00000200
#define SW10 0x00000400
#define SW11 0x00000800
#define SW12 0x00001000
#define SW13 0x00002000
#define SW14 0x00004000
#define SW15 0x00008000
#define SW16 0x00010000
#define SW17 0x00020000

//      pushbuttons
#define KEY1 0x00000002
#define KEY2 0x00000004
#define KEY3 0x00000008
#define KEY4 0x00000010

//		leds
#define LED0 0x00000001
#define LED1 0x00000002
#define LED2 0x00000004
#define LED3 0x00000008
#define LED4 0x00000010
#define LED5 0x00000020
#define LED6 0x00000040
#define LED7 0x00000080
#define LED8 0x00000100
#define LED9 0x00000200
#define LED10 0x00000400
#define LED11 0x00000800
#define LED12 0x00001000
#define LED13 0x00002000
#define LED14 0x00004000
#define LED15 0x00008000
#define LED16 0x00010000
#define LED17 0x00020000

//      hex
#define SEGA 0x00001
#define SEGB 0x00002
#define SEGC 0x00004
#define SEGD 0x00008
#define SEGE 0x00010
#define SEGF 0x00020
#define SEGG 0x00040

//     hex - numbers
#define ZERO SEGA | SEGB | SEGC | SEGD | SEGE | SEGF
#define ONE SEGB | SEGC
#define TWO SEGA | SEGB | SEGG | SEGE | SEGD
#define THREE SEGA | SEGB | SEGC | SEGD | SEGG
#define FOUR SEGB | SEGC | SEGF | SEGG
#define FIVE SEGA | SEGF | SEGG | SEGC | SEGD
#define SIX SEGA | SEGF | SEGE | SEGD | SEGC | SEGG
#define SEVEN SEGA | SEGB | SEGC