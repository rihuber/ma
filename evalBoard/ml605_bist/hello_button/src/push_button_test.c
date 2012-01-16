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
*       (c) Copyright 2008 - 2009 Xilinx Inc.
*       All rights reserved.
*
******************************************************************************/

/*****************************************************************************/
/**
* @file push_button_test.c
*
*
* This file contains the application for testing the Pushbutton & LED
* functionality for the Virtex-4 ML403 Development kit.
*
* @note     None.
*
* <pre>
*
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- --------------------------------------------------------
* 1.00  sun  02/29/08  First release.
* </pre>
*
******************************************************************************/

#include "xgpio_l.h"
#include "xutil.h"
#include "xuartns550_l.h"
#include <stdlib.h>
#include <stdio.h>
#include "xgpio.h"
#include "xparameters.h"

#if !SIM
#include "stdio.h"
#endif

/************************** Constant Definitions *****************************/
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

int main()
{
	u32 Button = 0;

	XUartNs550_SetBaud(UART_BASEADDR, UART_CLOCK, UART_BAUDRATE);
	XUartNs550_SetLineControlReg(UART_BASEADDR, XUN_LCR_8_DATA_BITS);

	/*
	 * Set gpio bits to input
	 */
	XGpio_WriteReg(XPAR_PUSH_BUTTONS_5BIT_BASEADDR, 0x4, 0x00003E00);

  xil_printf("\n\r********************************************************");
  xil_printf("\n\r********************************************************");
  xil_printf("\n\r**     ML605 - Button Test                            **");
  xil_printf("\n\r********************************************************");
  xil_printf("\n\r********************************************************\r\n");

	/*
	 * Set gpio bits to all output
	 */
	XGpio_WriteReg(XPAR_LEDS_POSITIONS_BASEADDR, 0x4, 0xFFFFFFF0);


	xil_printf("Press West Button & see if LED 0 glows\r\n\r\n");
	Button =0;
	while (Button == 0){
		Button = XGpio_ReadReg(XPAR_PUSH_BUTTONS_5BIT_BASEADDR, 1) & 0x00000008;
	}
	XGpio_WriteReg(XPAR_LEDS_POSITIONS_BASEADDR, 1, Button);

	xil_printf("Press South Button & see if LED 1 glows\r\n\r\n");
	Button =0;
	while (Button == 0){
		Button = XGpio_ReadReg(XPAR_PUSH_BUTTONS_5BIT_BASEADDR, 1) & 0x00000004;
	}
	XGpio_WriteReg(XPAR_LEDS_POSITIONS_BASEADDR, 1, Button);

	xil_printf("Press East Button & see if LED 2 glows\r\n\r\n");
	Button = 0;
	while (Button == 0){
		Button = XGpio_ReadReg(XPAR_PUSH_BUTTONS_5BIT_BASEADDR, 1)  & 0x00000002;
	}
	XGpio_WriteReg(XPAR_LEDS_POSITIONS_BASEADDR, 1, Button);

	xil_printf("Press North Button & see if LED 3 glows\r\n\r\n");
	Button =0;
	while (Button == 0){
		Button = XGpio_ReadReg(XPAR_PUSH_BUTTONS_5BIT_BASEADDR, 1) & 0x00000001;
	}
	XGpio_WriteReg(XPAR_LEDS_POSITIONS_BASEADDR, 1, Button);

	xil_printf("Press Center Button & see if all the 4 LEDs  glow\r\n\r\n");
	Button =0;
	while (Button == 0){
		Button = XGpio_ReadReg(XPAR_PUSH_BUTTONS_5BIT_BASEADDR, 1)  & 0x00000010;
	}
	XGpio_WriteReg(XPAR_LEDS_POSITIONS_BASEADDR, 1, 0xF);

	return_to_loader();

	return(0);
}

