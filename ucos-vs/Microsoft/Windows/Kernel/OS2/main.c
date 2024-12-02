#include <windows.h>
#include <commctrl.h> 
#include <stdio.h>
#include <cpu.h>
#include <os.h>
#include <ucos_ii.h>
#include <ucos_ii.c>
#include "app_cfg.h"
#include <os_cpu.h>

#define ZERO SEGA | SEGB | SEGC | SEGD | SEGE | SEGF
#define ONE SEGB | SEGC
#define TWO SEGA | SEGB | SEGG | SEGE | SEGD
#define THREE SEGA | SEGB | SEGC | SEGD | SEGG
#define FOUR SEGB | SEGC | SEGF | SEGG
#define FIVE SEGA | SEGF | SEGG | SEGC | SEGD
#define SIX SEGA | SEGF | SEGE | SEGD | SEGC | SEGG
#define SEVEN SEGA | SEGB | SEGC


#define TASK_STACKSIZE 2048
OS_STK task1_stk[TASK_STACKSIZE];
OS_STK task2_stk[TASK_STACKSIZE];

OS_EVENT* SWBox1;
int sw = 0; // Global variable to store the slider value
HWND hHexDisplay[42]; // Handles to the LED display segments

/* Definition of Task Priorities */
#define TASK1_PRIORITY 1
#define TASK2_PRIORITY 2

/* Prints "Hello World" and sleeps for one second */
void task1(void* pdata)
{
    while (1)
    {
        printf("Hello from task1\n");
        printf("Slider = %d\n", sw);
        OSMboxPostOpt(SWBox1, &sw, OS_POST_OPT_BROADCAST);
        OSTimeDlyHMSM(0, 0, 1, 0); // sleep for 1 second
    }
}

/* Prints "Hello World" and sleeps for one second */
void task2(void* pdata)
{
    while (1)
    {
        INT8U err;
        printf("Hello from task2\n");
        int* num;

        num = OSMboxPend(SWBox1, 0, &err);

        printf("T=%d\n", *num);

        //IOWR(0x80030, 0, *num);
        OSTimeDlyHMSM(0, 0, 1, 0); // sleep for 1 second
    }
}

int main(void)
{
    CPU_IntInit();
    Mem_Init();                                                 /* Initialize Memory Management Module                   */
    CPU_IntDis();                                               /* Disable all Interrupts                               */
    CPU_Init();                                                 /* Initialize the uC/CPU services                       */
    OSInit();                                                   /* Initialize uC/OS-II                                  */

    SWBox1 = OSMboxCreate((void*)0);

    OSTaskCreateExt(task1,
        NULL,
        (void*)&task1_stk[TASK_STACKSIZE - 1],
        TASK1_PRIORITY,
        TASK1_PRIORITY,
        task1_stk,
        TASK_STACKSIZE,
        NULL,
        0);

    OSTaskCreateExt(task2,
        NULL,
        (void*)&task2_stk[TASK_STACKSIZE - 1],
        TASK2_PRIORITY,
        TASK2_PRIORITY,
        task2_stk,
        TASK_STACKSIZE,
        NULL,
        0);


    OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II)   */

    while (DEF_ON) {                                            /* Should Never Get Here.                               */
        ;
    }

    return 0;
}