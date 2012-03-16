/******************************************************************************
*
*       XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"
*       AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND
*       SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,
*       OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,
*       APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION
*       THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,
*       AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE
*       FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY
*       WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE
*       IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR
*       REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF
*       INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*       FOR A PARTICULAR PURPOSE.
*
*       (c) Copyright 2009 Xilinx Inc.
*       All rights reserved.
*
******************************************************************************/


#include "xparameters.h"
#include "stdio.h"
#include "xgpio.h"
#include "xbasic_types.h"
#include "xuartns550_l.h"
#include "xil_sprintf.h"
#include "lcd.h"


#ifndef UART_BASEADDR
#define UART_BASEADDR XPAR_UARTNS550_0_BASEADDR
#endif
#define UART_CLOCK    XPAR_UARTNS550_0_CLOCK_FREQ_HZ

#if !SIM
#define UART_BAUDRATE 9600                      /* real hardware */
#else
#define UART_BAUDRATE (UART_CLOCK / 16 / 3)     /* simulation */
#endif


/*
 * Return to the bootloader via it's well-known vector at 0x40
 */
static void
return_to_loader()
{
    void (*loader_start1)();

    loader_start1 = (void*) (*(unsigned*)0x40);
    if (loader_start1) {
        (*loader_start1)();
    }
}

// Main Loop
int main(void)
{

    //char alm_str[20];
    //int i;

    XUartNs550_SetBaud(UART_BASEADDR, UART_CLOCK, UART_BAUDRATE);
    XUartNs550_SetLineControlReg(UART_BASEADDR, XUN_LCR_8_DATA_BITS);

    xil_printf("\n\r********************************************************");
    xil_printf("\n\r********************************************************");
    xil_printf("\n\r**     ML605 - LCD Test                               **");
    xil_printf("\n\r********************************************************");
    xil_printf("\n\r********************************************************\r\n");
    xil_printf("Watch the LCD screen for text:\r\n");

    /* INIT LCD Screen */

    // init the lcd gpio and make them outputs
    XGpio_WriteReg(XPAR_LCD_GPIO_BASEADDR, 0x4, 0x00000000);

    LCDInit();
    LCDOn();

    // Print to the LCD
    LCDPrintString (" Welcome To The ","   ML605 Demo!  ");

    return_to_loader();

    return 0;

}
