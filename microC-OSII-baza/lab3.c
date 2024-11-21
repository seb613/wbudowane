/*
 * "Hello World" example.
 
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */
#include <stdio.h>
#include <io.h>
#include <system.h>
#include "alt_types.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_irq.h"
//#include "sys/alt_timestamp.h"
#include <unistd.h>
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
 
//stan na 1-2
#define STATE_RED 1
#define STATE_RED_ORANGE 2
#define STATE_GREEN 3
#define STATE_ORANGE 4
#define STATE_BLINKING_ORANGE 5
#define STATE_EMERGENCY_VEHICLE 6
#define STATE_FORBIDDEN 7
 
 
struct interrupt_data
{
	volatile int *leds_addr;
	volatile int *hex_addr;
	volatile int *sliders_addr;
	volatile int *wstate;
	volatile int *wpstate;
	};
 
volatile int count = 0;
 
static void handle_sliders_interrupt(struct interrupt_data * data)
{
	int sw = IORD(SW_SLIDERS_BASE, 0);
	IOWR(data->leds_addr,0,sw);
	IOWR(data->hex_addr, 0 ,sw);
 
	/* inicjalizacja */
		int state=data->wstate;
		int pstate=data->wpstate;
	    IOWR(data->leds_addr, 0, 0);    /* Disable all leds */
	    IOWR(data->hex_addr, 0, 0);    /* Disable all segments on hex display*/
 
		int swstate=IORD(SW_SLIDERS_BASE, 0);     /* odczyt danych z portów wejściowych */
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
			 * sw3 - reset -> 1
			 * sw4 - emergency mode
			 * sw5 - blinking mode
			 *
			 * sw9 1->2
			 * sw8 2->3
			 * sw7 3->4
			 * sw6 4->1
			 */
 
 
 
 
			if ((data->wstate==5) || (data->wstate==6) || (data->wstate==7)){ //reset z trybów awaryjnych
				if (swstate&SW3)
				{
					data->wstate =1;
					data->wpstate =4;
				}
			}
 
			if (swstate&SW5){//blinking orange mode
				data->wstate = 5;
			}
			if (swstate&SW4){//emergency mode
				data->wstate = 6;
			}
 
			if ((__builtin_popcount(swstate>>2)>1))//shift bo slidery zepsute
			{
				data->wstate=7;
			}
 
			switch (state) { //stany na skrzyzowaniu 1-2
			            case STATE_RED: //ok  1
			                IOWR(data->hex_addr, 0, ONE);
			                IOWR(data->leds_addr,0, LED0 | LED6);//czerw1 ziel2
			                IOWR(data->hex_addr, 2, THREE);
			                if ((swstate&SW9) && (pstate==4))
			                {
			                	data->wstate = 2;
			                	data->wpstate = 1;
			                }
			                break;
 
			            case STATE_RED_ORANGE: //ok  2
			                IOWR(data->hex_addr, 0, TWO );
			                IOWR(data->leds_addr,0, LED0 | LED1 | LED5);//czerw1 pom1 pom2
			                IOWR(data->hex_addr, 2, FOUR );
			                if ((swstate&SW8) && (pstate==1))
							{
			                	data->wstate = 3;
			                	data->wpstate = 2;
							}
			                break;
 
			            case STATE_GREEN: //ok  3
			                IOWR(data->hex_addr, 0, THREE);
			                IOWR(data->leds_addr,0, LED2 | LED4);//ziel1 czer2
			                IOWR(data->hex_addr, 2, ONE );
 
			                if ((swstate&SW7) && (pstate==2))
							{
			                	data->wstate = 4;
			                	data->wpstate = 3;
							}
			                break;
 
			            case STATE_ORANGE: //ok  4
			                IOWR(data->hex_addr, 0, FOUR);
			                IOWR(data->leds_addr,0, LED1 | LED4 | LED5);//pom1 pom2 czerw2
			                IOWR(data->hex_addr, 2, TWO);
			                if ((swstate&SW6) && (pstate==3))
							{
			                	data->wstate = 1;
			                	data->wpstate = 4;
							}
			                break;
 
			            case STATE_BLINKING_ORANGE: //5
	 		               IOWR(data->hex_addr, 0, FIVE);
			               IOWR(data->leds_addr,0, LED3 | LED7);
	 		               IOWR(data->hex_addr, 2, FIVE);
			                break;
 
			            case STATE_EMERGENCY_VEHICLE: //6
			                IOWR(data->hex_addr, 0, SIX);
			                IOWR(data->hex_addr, 2, SIX);
			                IOWR(data->leds_addr,0, LED0 | LED6);//czerw1 ziel2
			                break;
 
			            case STATE_FORBIDDEN://7
			                IOWR(data->hex_addr, 0, SEVEN);
			                IOWR(data->hex_addr, 2, SEVEN);
			                IOWR(data->leds_addr,0, LED9);
			                break;
 
			            default:
			                break;
			        }
 
 
 
}
 
int main()
{
	int state=1;
	int pstate=4;
 
 
	volatile int *leds = (int*) (LEDS_BASE);
	volatile int *hexs = (int*) (HEX_BASE);
	volatile int *sliders = (int*) (SW_SLIDERS_BASE);
	volatile int *wstate=(int*)state;
	volatile int *wpstate=(int*)pstate;
 
	  struct interrupt_data data;
 
	  data.leds_addr = leds;
	  data.hex_addr = hexs;
	  data.sliders_addr = sliders;
	  data.wstate = wstate;
	  data.wpstate = wpstate;
 
	  IOWR_ALTERA_AVALON_PIO_IRQ_MASK(SW_SLIDERS_BASE, 0xff);
 
	  alt_ic_isr_register(SW_SLIDERS_IRQ_INTERRUPT_CONTROLLER_ID, SW_SLIDERS_IRQ, handle_sliders_interrupt, &data, 0x0);
	  //alt_ic_isr_register(PUSHBUTTON_IRQ_INTERRUPT_CONTROLLER_ID, PUSHBUTTON_IRQ, handle_pushbuttons_interrupt, &data, 0x0);
 
	  alt_ic_irq_enable(SW_SLIDERS_IRQ_INTERRUPT_CONTROLLER_ID, SW_SLIDERS_IRQ);
 
 
 
	  while(1);
 
  return 0;
}
 