#include <stdio.h> // Standard I/O library
#include <system.h> // System-specific definitions
#include "includes.h" // Additional includes for the RTOS

typedef enum POZIOM_TEMP // Enumeration for temperature levels
{
  POZIOM_0 = 0, // Level 0
  POZIOM_1 = 2, // Level 1
  POZIOM_2 = 4, // Level 2
  POZIOM_3 = 8, // Level 3
  POZIOM_4 = 16, // Level 4
  POZIOM_5 = 32, // Level 5
  POZIOM_6 = 64, // Level 6
  POZIOM_7 = 128, // Level 7
  POZIOM_8 = 256, // Level 8
  POZIOM_9 = 512, // Level 9
  POZIOM_10 = 1024, // Level 10
} TEMP;

#define LED0 0x00000001 // LED0 bit mask
#define LED1 0x00000002 // LED1 bit mask
#define LED2 0x00000004 // LED2 bit mask
#define LED3 0x00000008 // LED3 bit mask

/* Definition of Task Stacks */
#define TASK_STACKSIZE 2048 // Stack size for tasks
OS_STK task1_stk[TASK_STACKSIZE]; // Stack for task1
OS_STK task2_stk[TASK_STACKSIZE]; // Stack for task2

OS_EVENT *SWBox1; // Mailbox for inter-task communication

/* Definition of Task Priorities */
#define TASK1_PRIORITY 1 // Priority for task1
#define TASK2_PRIORITY 2 // Priority for task2

// alt_up_parallel_port_dev const *uchwytLEDR; = alt_up_parallel_port_open_dev("/dev/parallel_port_2");
alt_up_parallel_port_dev *uchwytLEDR; // Handle for LED parallel port

/* Prints "Hello World" and sleeps for three seconds */
void task1(void *pdata) // Task1 function
{
  while (1) // Infinite loop
  {
    printf("Hello from task1\n"); // Print message
    int sw; // Variable to store slider value
    int *msg; // Pointer for mailbox message
    sw = IORD(0x80010, 0); // Read slider value from memory address

    printf("Slider = %d\n", sw); // Print slider value
    OSMboxPostOpt(SWBox1, &sw, OS_POST_OPT_BROADCAST); // Post slider value to mailbox

    OSTimeDlyHMSM(0, 0, 1, 0); // Delay for 1 second
  }
}
/* Prints "Hello World" and sleeps for three seconds */
void task2(void *pdata) // Task2 function
{
  while (1) // Infinite loop
  {
    INT8U err; // Variable to store error code
    printf("Hello from task2\n"); // Print message
    int *num; // Pointer to store received value

    num = OSMboxPend(SWBox1, 0, &err); // Wait for message from mailbox

    printf("T=%d\n", *num); // Print received value

    IOWR(0x80030, 0, *num); // Write received value to memory address
    OSTimeDlyHMSM(0, 0, 1, 0); // Delay for 1 second
  }
}

/* The main function creates two task and starts multi-tasking */
int main(void) // Main function
{
  SWBox1 = OSMboxCreate((void *)0); // Create mailbox

  OSTaskCreateExt(task1, // Create task1
                  NULL,
                  (void *)&task1_stk[TASK_STACKSIZE - 1],
                  TASK1_PRIORITY,
                  TASK1_PRIORITY,
                  task1_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);

  OSTaskCreateExt(task2, // Create task2
                  NULL,
                  (void *)&task2_stk[TASK_STACKSIZE - 1],
                  TASK2_PRIORITY,
                  TASK2_PRIORITY,
                  task2_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);

  OSStart(); // Start the RTOS
  return 0; // Return from main
}