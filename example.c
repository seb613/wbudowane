#include <stdio.h>
#include <system.h>
#include "includes.h"

typedef enum POZIOM_TEMP
{
    POZIOM_0 = 0,
    POZIOM_1 = 2,
    POZIOM_2 = 4,
    POZIOM_3 = 8,
    POZIOM_4 = 16,
    POZIOM_5 = 32,
    POZIOM_6 = 64,
    POZIOM_7 = 128,
    POZIOM_8 = 256,
    POZIOM_9 = 512,
    POZIOM_10 = 1024,

} TEMP;

#define LED0 0x00000001
#define LED1 0x00000002
#define LED2 0x00000004
#define LED3 0x00000008

/* Definition of Task Stacks */
#define TASK_STACKSIZE 2048
OS_STK task1_stk[TASK_STACKSIZE];
OS_STK task2_stk[TASK_STACKSIZE];

OS_EVENT *SWBox1;

/* Definition of Task Priorities */

#define TASK1_PRIORITY 1
#define TASK2_PRIORITY 2

// alt_up_parallel_port_dev const *uchwytLEDR; = alt_up_parallel_port_open_dev("/dev/parallel_port_2");

alt_up_parallel_port_dev *uchwytLEDR;

/* Prints "Hello World" and sleeps for three seconds */
void task1(void *pdata)
{
    while (1)
    {
        printf("Hello from task1\n");
        int sw;
        int *msg;
        sw = IORD(0x80010, 0);

        printf("Slider = %d\n", sw);
        OSMboxPostOpt(SWBox1, &sw, OS_POST_OPT_BROADCAST);

        OSTimeDlyHMSM(0, 0, 1, 0); // sleep for 1 second
    }
}
/* Prints "Hello World" and sleeps for three seconds */
void task2(void *pdata)
{
    while (1)
    {
        INT8U err;
        printf("Hello from task2\n");
        int *num;

        num = OSMboxPend(SWBox1, 0, &err);

        printf("T=%d\n", *num);

        IOWR(0x80030, 0, *num);
        OSTimeDlyHMSM(0, 0, 1, 0); // sleep for 1 second
    }
}

/* The main function creates two task and starts multi-tasking */
int main(void)
{

    SWBox1 = OSMboxCreate((void *)0);

    OSTaskCreateExt(task1,
                    NULL,
                    (void *)&task1_stk[TASK_STACKSIZE - 1],
                    TASK1_PRIORITY,
                    TASK1_PRIORITY,
                    task1_stk,
                    TASK_STACKSIZE,
                    NULL,
                    0);

    OSTaskCreateExt(task2,
                    NULL,
                    (void *)&task2_stk[TASK_STACKSIZE - 1],
                    TASK2_PRIORITY,
                    TASK2_PRIORITY,
                    task2_stk,
                    TASK_STACKSIZE,
                    NULL,
                    0);

    OSStart();
    return 0;
}