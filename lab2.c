#include <stdio.h>
#include <io.h>
#include "system.h"
#include "definition.h"


/*
 * Stany
1- czerwony
2- czerwony i pomarańczowy
3- zielony
4- pomarańczowy
5- pomarańczowy migający
6- tryb przejazdu pojazdu uprzywilejowanego
7 - STAN ZABRONIONY
 */

// stan na 1-2
#define STATE_RED 1
#define STATE_RED_ORANGE 2
#define STATE_GREEN 3
#define STATE_ORANGE 4
#define STATE_BLINKING_ORANGE 5
#define STATE_EMERGENCY_VEHICLE 6
#define STATE_FORBIDDEN 7


/* EXAMPLE
HEX0= SEGB | SEGC;     //1
	HEX1= SEGA | SEGB |SEGD | SEGE | SEGG;   //2
	HEX2= SEGA | SEGB |SEGC | SEGD | SEGG;   //3
	HEX3= SEGB |SEGC | SEGF | SEGG;   //4
	IOWR(HEX_BASE,0, (HEX3 << 24) | (HEX2 <<16) | (HEX1 <<8) |HEX0);
*/

/*
 * Stany
1- czerwony
2- czerwony i pomarańczowy
3- zielony
4- pomarańczowy
5- pomarańczowy migający
6- tryb przejazdu pojazdu uprzywilejowanego
7 - STAN ZABRONIONY
 */

// stan na 1-2


main()
{

	/* inicjalizacja */
	int state = 1;
	int swstate = 0;
	int pstate = 4;
	IOWR(LEDS_BASE, 0, 0); /* Disable all leds */
	IOWR(HEX_BASE, 0, 0);  /* Disable all segments on hex display*/

	do
	{

		swstate = IORD(SW_SLIDERS_BASE, 0); /* odczyt danych z portów wejściowych */

		/*
		 * led0 - czerowny
		 * led1 - pom
		 * led2 - ziel
		 * led3 - migajacy
		 *
		 * led4 - czerowny
		 * led5 - pom
		 * led6 - ziel
		 * led7 - migajacy
		 */

		/*
		 * sw3 - reset
		 * sw4 - emergency mode
		 * sw5 - blinking mode
		 *
		 * sw9 1->2
		 * sw8 2->3
		 * sw7 3->4
		 * sw6 4->1
		 */

		if (__builtin_popcount(swstate) && (1 == 2))
		{
			state = 7;
		}

		if (state == 5 || state == 6 || state == 7)
		{ // reset z trybów awaryjnych
			if (swstate & SW3)
			{
				state = 1;
				pstate = 4;
			}
			break;
		}

		if (swstate & SW5)
		{ // blinking orange mode
			state = 5;
		}
		if (swstate & SW4)
		{ // emergency mode
			state = 6;
		}

		switch (state)
		{				// stany na skrzyzowaniu 1-2
		case STATE_RED: // ok  1
			IOWR(HEX_BASE, 0, ONE);
			IOWR(LEDS_BASE, 0, LED0 | LED6); // czerw1 ziel2
			IOWR(HEX_BASE, 2, THREE);
			if ((swstate & SW9) && (pstate == 4))
			{
				state = 2;
				pstate = 1;
			}
			break;

		case STATE_RED_ORANGE: // ok  2
			IOWR(HEX_BASE, 0, TWO);
			IOWR(LEDS_BASE, 0, LED0 | LED1 | LED5); // czerw1 pom1 pom2
			IOWR(HEX_BASE, 2, FOUR);
			if ((swstate & SW8) && (pstate == 1))
			{
				state = 3;
				pstate = 2;
			}
			break;

		case STATE_GREEN: // ok  3
			IOWR(HEX_BASE, 0, THREE);
			IOWR(LEDS_BASE, 0, LED2 | LED4); // ziel1 czer2
			IOWR(HEX_BASE, 2, ONE);

			if ((swstate & SW7) && (pstate == 2))
			{
				state = 4;
				pstate = 3;
			}
			break;

		case STATE_ORANGE: // ok  4
			IOWR(HEX_BASE, 0, FOUR);
			IOWR(LEDS_BASE, 0, LED1 | LED4 | LED5); // pom1 pom2 czerw2
			IOWR(HEX_BASE, 2, TWO);
			if ((swstate & SW6) && (pstate == 3))
			{
				state = 1;
				pstate = 4;
			}
			break;

		case STATE_BLINKING_ORANGE: // 5
			IOWR(HEX_BASE, 0, FIVE);
			IOWR(LEDS_BASE, 0, LED3 | LED7);
			IOWR(HEX_BASE, 2, FIVE);
			break;

		case STATE_EMERGENCY_VEHICLE: // 6
			IOWR(HEX_BASE, 0, SIX);
			IOWR(HEX_BASE, 2, SIX);
			IOWR(LEDS_BASE, 0, LED0 | LED6); // czerw1 ziel2
			break;

		case STATE_FORBIDDEN: // 7
			IOWR(HEX_BASE, 0, SEVEN);
			IOWR(HEX_BASE, 2, SEVEN);
			IOWR(LEDS_BASE, 0, 0);
			break;

		default:
			break;
		}
	} while (1);

	return 0;
}

//////////////////

/*
 * definition.h
 *
 *  Created on: 19 paź 2016
 *      Author: darek
 */

#ifndef DEFINITION_H_
#define DEFINITION_H_

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

#endif /* DEFINITION_H_ */
