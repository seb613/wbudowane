#include <stdio.h>
#include <io.h>
#include "system.h"
#include "definition.h"

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
2- czerwony i pomarañczowy
3- zielony
4- pomarañczowy
5- pomarañczowy migaj¹cy
6- tryb przejazdu pojazdu uprzywilejowanego
7 - STAN ZABRONIONY
  */

//stan na 1-2
#define STATE_RED 1
#define STATE_RED_ORANGE 2
#define STATE_GREEN 3
#define STATE_ORANGE 4
#define STATE_BLINKING_ORANGE 5
#define STATE_EMERGENCY_VEHICLE 6
#define STATE_FORBIDDEN 7




main(){



	/* inicjalizacja */
	int state=1;
	int swstate=0;
	int pstate=4;
    IOWR(LEDS_BASE, 0, 0);    /* Disable all leds */
    IOWR(HEX_BASE, 0, 0);    /* Disable all segments on hex display*/


	do {

		swstate=IORD(SW_SLIDERS_BASE, 0) ;    /* odczyt danych z portów wejœciowych */

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


		if (__builtin_popcount(swstate)&&(1==2))
		{
			state=7;
		};

		if ((state==5) || (state==6) || (state==7)){ //reset z trybów awaryjnych
			if (swstate&SW3)
			{
				state =1;
				pstate =4;
			};
		};

		if (swstate&SW5){//blinking orange mode
			state = 5;
		};

		if (swstate&SW4){//emergency mode
			state = 6;
		};


		switch (state) { //stany na skrzyzowaniu 1-2
		            case STATE_RED: //ok  1
		                IOWR(HEX_BASE, 0, ONE);
		                IOWR(LEDS_BASE,0, LED0 | LED6);//czerw1 ziel2
		                IOWR(HEX_BASE, 2, THREE);
		                if ((swstate&SW9) && (pstate==4))
		                {
		                	state = 2;
		                	pstate = 1;
		                }
		                break;

		            case STATE_RED_ORANGE: //ok  2
		                IOWR(HEX_BASE, 0, TWO );
		                IOWR(LEDS_BASE,0, LED0 | LED1 | LED5);//czerw1 pom1 pom2
		                IOWR(HEX_BASE, 2, FOUR );
		                if ((swstate&SW8) && (pstate==1))
						{
							state = 3;
							pstate = 2;
						}
		                break;

		            case STATE_GREEN: //ok  3
		                IOWR(HEX_BASE, 0, THREE);
		                IOWR(LEDS_BASE,0, LED2 | LED4);//ziel1 czer2
		                IOWR(HEX_BASE, 2, ONE );

		                if ((swstate&SW7) && (pstate==2))
						{
							state = 4;
							pstate = 3;
						}
		                break;

		            case STATE_ORANGE: //ok  4
		                IOWR(HEX_BASE, 0, FOUR);
		                IOWR(LEDS_BASE,0, LED1 | LED4 | LED5);//pom1 pom2 czerw2
		                IOWR(HEX_BASE, 2, TWO);
		                if ((swstate&SW6) && (pstate==3))
						{
							state = 1;
							pstate = 4;
						}
		                break;

		            case STATE_BLINKING_ORANGE: //5
 		               IOWR(HEX_BASE, 0, FIVE);
		               IOWR(LEDS_BASE,0, LED3 | LED7);
 		               IOWR(HEX_BASE, 2, FIVE);
		                break;

		            case STATE_EMERGENCY_VEHICLE: //6
		                IOWR(HEX_BASE, 0, SIX);
		                IOWR(HEX_BASE, 2, SIX);
		                IOWR(LEDS_BASE,0, LED0 | LED6);//czerw1 ziel2
		                break;

		            case STATE_FORBIDDEN://7
		                IOWR(HEX_BASE, 0, SEVEN);
		                IOWR(HEX_BASE, 2, SEVEN);
		                IOWR(LEDS_BASE,0, 0);
		                break;

		            default:
		                break;
		        }
		    } while (1);


	return 0;
}

