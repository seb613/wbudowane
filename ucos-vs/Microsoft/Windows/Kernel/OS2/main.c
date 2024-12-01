#include <windows.h>
#include <commctrl.h> // Add this include for TBM_GETPOS
#include <stdio.h>
#include <cpu.h>
#include <lib_mem.h>
#include <os.h>
#include "app_cfg.h"

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

void UpdateHexDisplay()
{
    for (int i = 0; i < 42; i++)
    {
        int bit = (sw >> i) & 1;
        SetWindowText(hHexDisplay[i], bit ? "1" : "0");
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_COMMAND:
        if (HIWORD(wParam) == BN_CLICKED)
        {
            int id = LOWORD(wParam);
            int bit = id - 100; // Checkbox IDs start from 100
            if (bit >= 0 && bit < 10)
            {
                if (SendMessage(GetDlgItem(hwnd, id), BM_GETCHECK, 0, 0) == BST_CHECKED)
                {
                    sw |= (1 << bit); // Set bit
                }
                else
                {
                    sw &= ~(1 << bit); // Clear bit
                }
                UpdateHexDisplay(); // Update the hex display
            }
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

DWORD WINAPI GuiThread(LPVOID lpParam)
{
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "CheckboxWindowClass";
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        "CheckboxWindowClass",
        "Bit Manipulation",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 500,
        NULL, NULL, hInstance, NULL);

    char buttonLabel[16];

    for (int i = 0; i < 10; i++)
    {
        sprintf_s(buttonLabel, sizeof(buttonLabel), "SW%d", i);

        CreateWindowEx(
            0, "BUTTON", buttonLabel,
            WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
            10, 10 + i * 30, 260, 30,
            hwnd, (HMENU)(100 + i), hInstance, NULL);
    }

    // Create the LED display segments
    for (int i = 0; i < 42; i++)
    {
        hHexDisplay[i] = CreateWindowEx(
            0, "STATIC", "0",
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            10 + (i % 7) * 20, 350 + (i / 7) * 20, 20, 20,
            hwnd, NULL, hInstance, NULL);
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

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

    // Create a thread for the GUI
    CreateThread(NULL, 0, GuiThread, NULL, 0, NULL);

    OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II)   */

    while (DEF_ON) {                                            /* Should Never Get Here.                               */
        ;
    }

    return 0;
}