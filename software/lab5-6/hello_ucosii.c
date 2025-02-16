#include <stdio.h>
#include <system.h>
#include "includes.h"
#include <io.h>
#include <unistd.h>


//ODKOMENTOWA� DO SYMULACJI
#define HEX_BASE HEX3_HEX0_BASE
#define SW_SLIDERS_BASE SLIDER_SWITCHES_BASE
///

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

/*
 * Stany
1- czerwony
2- czerwony i pomara�czowy
3- zielony
4- pomara�czowy
5- pomara�czowy migaj�cy
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

/* Definition of Task Stacks */
#define TASK_STACKSIZE 2048
OS_STK task1_stk[TASK_STACKSIZE];
OS_STK task2_stk[TASK_STACKSIZE];
OS_STK task3_stk[TASK_STACKSIZE];
OS_STK task4_stk[TASK_STACKSIZE];
OS_STK task5_stk[TASK_STACKSIZE];
OS_STK task6_stk[TASK_STACKSIZE];
OS_STK task7_stk[TASK_STACKSIZE];

/* Definition of Task Priorities */

#define TASK1_PRIORITY 1
#define TASK2_PRIORITY 2
#define TASK3_PRIORITY 3
#define TASK4_PRIORITY 4
#define TASK5_PRIORITY 5
#define TASK6_PRIORITY 6
#define TASK7_PRIORITY 7

// Queue Definitions
#define QUEUE_SIZE 1
void *Queue1Storage[QUEUE_SIZE];
void *Queue2Storage[QUEUE_SIZE];
void *Queue3Storage[QUEUE_SIZE];
void *Queue4Storage[QUEUE_SIZE];
void *Queue5Storage[QUEUE_SIZE];
void *Queue6Storage[QUEUE_SIZE];

OS_EVENT *StateQueue1;
OS_EVENT *StateQueue2;
OS_EVENT *StateQueue3;
OS_EVENT *StateQueue4;
OS_EVENT *StateQueue5;
OS_EVENT *StateQueue6;

void wait(int time)
{
    usleep(time * 1000);
}

void cycle(int *state, int *pstate)
{
    if (*state >= 4)
    {
        *state = 1;
        *pstate = 4;
        return;
    }
    *pstate = *state;
    *state = *state + 1;
    return;
}


// read slider value
void task1(void *pdata)
{
    int state = STATE_RED;
    int pstate = STATE_ORANGE;
    int swstate;

    while (1)
    {
        swstate = IORD(SW_SLIDERS_BASE, 0);
        printf("state = %d\n", state);
        printf("pstate = %d\n", pstate);
        printf("swtate = %d\n", swstate);

        if (swstate & SW0) // automatic mode
        {
            cycle(&state, &pstate);
            wait(2);
        }
        else
        {
            if (__builtin_popcount(swstate >> 2) > 1)
            {
                state = 7;
            };

            if (state == 5 || state == 6 || state == 7)
            { // reset z tryb�w awaryjnych
                if (swstate & SW3)
                {
                    state = 1;
                    pstate = 4;
                }
            };

            if (swstate & SW5)
            { // blinking orange mode
                state = 5;
            };
            if (swstate & SW4)
            { // emergency mode
                state = 6;
            };

            if (state == STATE_RED_ORANGE)
            {
                if ((swstate & SW8) && (pstate == 1))
                {
                    state = 3;
                    pstate = 2;
                }
            };

            if (state == STATE_GREEN)
            {
                if ((swstate & SW7) && (pstate == 2))
                {
                    state = 4;
                    pstate = 3;
                }
            };

            if (state == STATE_ORANGE)
            {
                if ((swstate & SW6) && (pstate == 3))
                {
                    state = 1;
                    pstate = 4;
                }
            };
        };

        if (state == STATE_RED)
        {
            IOWR(HEX_BASE, 0, ONE);
            IOWR(LEDS_BASE, 0, LED0 | LED6); // czerw1 ziel2
            IOWR(HEX_BASE, 2, THREE);
            if ((swstate & SW9) && (pstate == 4))
            {
                state = 2;
                pstate = 1;
            }
        };

        if (state == 2)
        {
            INT8U err = OSQPost(StateQueue1, &state);
            if (err != OS_ERR_NONE) {
                printf("Error  StateQueue1: %d\n", err);
            }
        }
        if (state == 3)
        {
            INT8U err = OSQPost(StateQueue2, &state);
            if (err != OS_ERR_NONE) {
                printf("Error  StateQueue2: %d\n", err);
            }
        }
        if (state == 4)
        {
            INT8U err = OSQPost(StateQueue3, &state);
            if (err != OS_ERR_NONE) {
                printf("Error  StateQueue3: %d\n", err);
            }
        }
        if (state == 5)
        {
            INT8U err = OSQPost(StateQueue4, &state);
            if (err != OS_ERR_NONE) {
                printf("Error  StateQueue4: %d\n", err);
            }
        }
        if (state == 6)
        {
            INT8U err = OSQPost(StateQueue5, &state);
            if (err != OS_ERR_NONE) {
                printf("Error  StateQueue5: %d\n", err);
            }
        }
        if (state == 7)
        {
            INT8U err = OSQPost(StateQueue6, &state);
            if (err != OS_ERR_NONE) {
                printf("Error  StateQueue6: %d\n", err);
            }
        }

        OSTimeDlyHMSM(0, 0, 2, 0);
    }
}

// control led
void task2(void *pdata) // read orange
{
    while (1)
    {
        INT8U err;
        int *state;
        state = OSQPend(StateQueue1, 0, &err);
        if (err != OS_ERR_NONE) {
            printf("Error  StateQueue1: %d\n", err);
            continue;
        }
        if (*state == 2)
        {
            IOWR(HEX_BASE, 0, TWO);
            IOWR(LEDS_BASE, 0, LED0 | LED1 | LED5); // czerw1 pom1 pom2
            IOWR(HEX_BASE, 2, FOUR);
        };

        OSTimeDlyHMSM(0, 0, 0, 500);
    }
}

void task3(void *pdata)
{
    while (1)
    {
        INT8U err;
        int *state;
        state = OSQPend(StateQueue2, 0, &err);
        if (err != OS_ERR_NONE) {
            printf("Error  StateQueue2: %d\n", err);
            continue;
        }
        if (*state == 3)
        {
            IOWR(HEX_BASE, 0, THREE);
            IOWR(LEDS_BASE, 0, LED2 | LED4); // ziel1 czer2
            IOWR(HEX_BASE, 2, ONE);
        };
        OSTimeDlyHMSM(0, 0, 0, 500);
    }
}

// control led
void task4(void *pdata)
{
    while (1)
    {
        INT8U err;
        int *state;
        state = OSQPend(StateQueue3, 0, &err);
        if (err != OS_ERR_NONE) {
            printf("Error  StateQueue3: %d\n", err);
            continue;
        }
        if (*state == 4)
        {
            IOWR(HEX_BASE, 0, FOUR);
            IOWR(LEDS_BASE, 0, LED1 | LED4 | LED5); // pom1 pom2 czerw2
            IOWR(HEX_BASE, 2, TWO);
        };
        OSTimeDlyHMSM(0, 0, 0, 500);
    }
}

// stany specjalne
void task5(void *pdata)
{
    while (1)
    {
        INT8U err;
        int *state;
        state = OSQPend(StateQueue4, 0, &err);
        if (err != OS_ERR_NONE) {
            printf("Error  StateQueue4: %d\n", err);
            continue;
        }
        if (*state == 5)
        {
            IOWR(HEX_BASE, 0, FIVE);
            IOWR(LEDS_BASE, 0, LED3 | LED7);
            IOWR(HEX_BASE, 2, FIVE);
        };
        OSTimeDlyHMSM(0, 0, 0, 500);
    }
}

void task6(void *pdata)
{
    while (1)
    {
        INT8U err;
        int *state;
        state = OSQPend(StateQueue5, 0, &err);
        if (err != OS_ERR_NONE) {
            printf("Error  StateQueue5: %d\n", err);
            continue;
        }
        if (*state == 6)
        {
            IOWR(HEX_BASE, 0, SIX);
            IOWR(HEX_BASE, 2, SIX);
            IOWR(LEDS_BASE, 0, LED0 | LED6); // czerw1 ziel2
        };
        OSTimeDlyHMSM(0, 0, 0, 500);
    }
}

void task7(void *pdata)
{
    while (1)
    {
        INT8U err;
        int *state;
        state = OSQPend(StateQueue6, 0, &err);
        if (err != OS_ERR_NONE) {
            printf("Error  StateQueue6: %d\n", err);
            continue;
        }
        if (*state == 7)
        {
            IOWR(HEX_BASE, 0, SEVEN);
            IOWR(HEX_BASE, 2, SEVEN);
            IOWR(LEDS_BASE, 0, 0);
        };
        OSTimeDlyHMSM(0, 0, 0, 500);
    }
}

/* The main function creates two task and starts multi-tasking */
int main(void)
{

    StateQueue1 = OSQCreate(Queue1Storage, QUEUE_SIZE);
    StateQueue2 = OSQCreate(Queue2Storage, QUEUE_SIZE);
    StateQueue3 = OSQCreate(Queue3Storage, QUEUE_SIZE);
    StateQueue4 = OSQCreate(Queue4Storage, QUEUE_SIZE);
    StateQueue5 = OSQCreate(Queue5Storage, QUEUE_SIZE);
    StateQueue6 = OSQCreate(Queue6Storage, QUEUE_SIZE);

    INT8U err;

    err = OSTaskCreateExt(task1,
                          NULL,
                          (void *)&task1_stk[TASK_STACKSIZE - 1],
                          TASK1_PRIORITY,
                          TASK1_PRIORITY,
                          task1_stk,
                          TASK_STACKSIZE,
                          NULL,
                          0);
    if (err != OS_ERR_NONE) {
        printf("Error creating task1: %d\n", err);
    }

    err = OSTaskCreateExt(task2,
                          NULL,
                          (void *)&task2_stk[TASK_STACKSIZE - 1],
                          TASK2_PRIORITY,
                          TASK2_PRIORITY,
                          task2_stk,
                          TASK_STACKSIZE,
                          NULL,
                          0);
    if (err != OS_ERR_NONE) {
        printf("Error creating task2: %d\n", err);
    }

    err = OSTaskCreateExt(task3,
                          NULL,
                          (void *)&task3_stk[TASK_STACKSIZE - 1],
                          TASK3_PRIORITY,
                          TASK3_PRIORITY,
                          task3_stk,
                          TASK_STACKSIZE,
                          NULL,
                          0);
    if (err != OS_ERR_NONE) {
        printf("Error creating task3: %d\n", err);
    }

    err = OSTaskCreateExt(task4,
                          NULL,
                          (void *)&task4_stk[TASK_STACKSIZE - 1],
                          TASK4_PRIORITY,
                          TASK4_PRIORITY,
                          task4_stk,
                          TASK_STACKSIZE,
                          NULL,
                          0);
    if (err != OS_ERR_NONE) {
        printf("Error creating task4: %d\n", err);
    }

    err = OSTaskCreateExt(task5,
                          NULL,
                          (void *)&task5_stk[TASK_STACKSIZE - 1],
                          TASK5_PRIORITY,
                          TASK5_PRIORITY,
                          task5_stk,
                          TASK_STACKSIZE,
                          NULL,
                          0);
    if (err != OS_ERR_NONE) {
        printf("Error creating task5: %d\n", err);
    }

    err = OSTaskCreateExt(task6,
                          NULL,
                          (void *)&task6_stk[TASK_STACKSIZE - 1],
                          TASK6_PRIORITY,
                          TASK6_PRIORITY,
                          task6_stk,
                          TASK_STACKSIZE,
                          NULL,
                          0);
    if (err != OS_ERR_NONE) {
        printf("Error creating task6: %d\n", err);
    }

    err = OSTaskCreateExt(task7,
                          NULL,
                          (void *)&task7_stk[TASK_STACKSIZE - 1],
                          TASK7_PRIORITY,
                          TASK7_PRIORITY,
                          task7_stk,
                          TASK_STACKSIZE,
                          NULL,
                          0);
    if (err != OS_ERR_NONE) {
        printf("Error creating task7: %d\n", err);
    }

    OSStart();
    return 0;
}
