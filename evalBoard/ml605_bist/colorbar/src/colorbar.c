/*$Id: xtft_example.c,v 1.1.2.2 2008/06/09 10:54:36 suneelg Exp $ */
/*****************************************************************************
*
*	XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"
*	AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND
*	SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,
*	OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,
*	APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION
*	THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,
*	AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE
*	FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY
*	WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE
*	IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR
*	REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF
*	INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*	FOR A PARTICULAR PURPOSE.
*
*	(c) Copyright 2008 - 2009 Xilinx Inc.
*	All rights reserved.
*
*****************************************************************************/
/****************************************************************************/
/**
*
* @file colorbar.c
*
* TGenerates a colorbar pattern
*
******************************************************************************/

/***************************** Include Files ********************************/

#include <stdio.h>
#include <xio.h>
#include "xbasic_types.h"
#include "xstatus.h"
#include "xtmrctr_udelay.h"
#include "xuartns550_l.h"
#include "xparameters.h"
#include "xtft.h"

#ifdef PPC440CACHE
#include "xcache_l.h"
#endif

/************************** Constant Definitions ****************************/
#define DISPLAY_COLUMNS  640
#define DISPLAY_ROWS     480

#ifndef UART_BASEADDR
#define UART_BASEADDR XPAR_UARTNS550_0_BASEADDR
#endif
#define UART_CLOCK    XPAR_UARTNS550_0_CLOCK_FREQ_HZ

#if !SIM
#define UART_BAUDRATE 9600                      /* real hardware */
#else
#define UART_BAUDRATE (UART_CLOCK / 16 / 3)     /* simulation */
#endif


/**************************** Type Definitions ******************************/

/***************** Macros (Inline Functions) Definitions ********************/

/************************** Function Prototypes *****************************/

static int XTft_DrawSolidBox(XTft *Tft, u32 Left, u32 Top, u32 Right,
			u32 Bottom, u32 PixelVal);
int TftExample(u32 TftDeviceId);

/************************** Variable Definitions ****************************/

static XTft TftInstance;

/************************** Function Definitions ****************************/

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

/*****************************************************************************/
/**
*
* Main function that invokes the Tft example.
*
* @param	None.
*
* @return
*		- XST_SUCCESS if successful.
*		- XST_FAILURE if unsuccessful.
*
* @note		None.
*
******************************************************************************/
int main()
{
	int Status;

	Status = TftExample(XPAR_TFT_0_DEVICE_ID);
	if ( Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return_to_loader();

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* This is the example function which performs the following operations on
* the TFT device -
* - Write numeric characters (0-9) one after another
* - Writes a Color Bar Pattern
* - fills the framebuffer with three colors
*
* @param	TftDeviceId is the unique Id of the device.
*
* @return
*		- XST_SUCCESS if successful.
*		- XST_FAILURE if unsuccessful.
*
* @note		None.
*
******************************************************************************/
int TftExample(u32 TftDeviceId)
{
	int Status;
	u8 VarChar;
	u32 Color;
	u32 Address;
	XTft_Config *TftConfigPtr;
	unsigned int *framebuf_ptr;
	unsigned int i;

    XUartNs550_SetBaud(UART_BASEADDR, UART_CLOCK, UART_BAUDRATE);
    XUartNs550_SetLineControlReg(UART_BASEADDR, XUN_LCR_8_DATA_BITS);

  xil_printf("\n\r********************************************************");
  xil_printf("\n\r********************************************************");
  xil_printf("\n\r**     ML605 - TFT Test                               **");
  xil_printf("\n\r********************************************************");
  xil_printf("\n\r********************************************************\r\n");


    /* Init Timer */
	TmrCtrInit();
	/*
	 * Get address of the XTft_Config structure for the given device id.
	 */

	TftConfigPtr = XTft_LookupConfig(TftDeviceId);
	if (TftConfigPtr == (XTft_Config *)NULL) {
		return XST_FAILURE;
	}

	/*
	 * Initialize all the TftInstance members and fills the screen with
	 * default background color.
	 */
	Status = XTft_CfgInitialize(&TftInstance, TftConfigPtr,
				 	TftConfigPtr->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}


	xil_printf("\r\n");
	xil_printf("  Display color\r\n");
	XTft_SetColor(&TftInstance, 0, 0);
	XTft_ClearScreen(&TftInstance);
	udelay(1000000);
	XTft_SetColor(&TftInstance, 0x00FF0000, 0x0);
	XTft_Write(&TftInstance, 0x30);
	XTft_Write(&TftInstance, 0x31);
	XTft_Write(&TftInstance, 0x32);
	XTft_Write(&TftInstance, 0x33);
	XTft_Write(&TftInstance, 0x34);
	XTft_Write(&TftInstance, 0x35);
	XTft_Write(&TftInstance, 0x36);
	XTft_Write(&TftInstance, 0x37);
	XTft_Write(&TftInstance, 0x38);
	XTft_Write(&TftInstance, 0x39);
	udelay(1500000);

	framebuf_ptr = (int*)(XPAR_DDR3_SDRAM_MPMC_BASEADDR + 0x01000000);
	xil_printf("  Painting Screen RED\r\n");
	for( i = 0; i < 512*1024; i++)
	{
	   *framebuf_ptr++ = 0x00FF0000;
	}
	udelay(1000000);

	xil_printf("  Painting Screen GREEN\r\n");
	framebuf_ptr = (int*)(XPAR_DDR3_SDRAM_MPMC_BASEADDR + 0x01000000);
	for( i = 0; i < 512*1024; i++)
	{
	   *framebuf_ptr++ = 0x0000FF00;
	}
	udelay(1000000);

	xil_printf("  Painting Screen BLUE\r\n");
	framebuf_ptr = (int*)(XPAR_DDR3_SDRAM_MPMC_BASEADDR + 0x01000000);
	for( i = 0; i < 512*1024; i++)
	{
	   *framebuf_ptr++ = 0x000000FF;
	}
	udelay(1500000);
	xil_printf("  Writing Color Bar Pattern\r\n");

	XTft_DrawSolidBox(&TftInstance,   0, 0, 79,479,0x00ffffff); // white
	XTft_DrawSolidBox(&TftInstance,  80, 0,159,479,0x00ff0000); // red
	XTft_DrawSolidBox(&TftInstance, 160, 0,239,479,0x0000ff00); // green
	XTft_DrawSolidBox(&TftInstance, 240, 0,319,479,0x000000ff); // blue
	XTft_DrawSolidBox(&TftInstance, 320, 0,399,479,0x00ffffff); // white
	XTft_DrawSolidBox(&TftInstance, 400, 0,479,479,0x00AAAAAA); // grey
	XTft_DrawSolidBox(&TftInstance, 480, 0,559,479,0x00777777); // not-so-grey
	XTft_DrawSolidBox(&TftInstance, 560, 0,639,479,0x00333333); // lite grey

	xil_printf("  TFT test completed!\r\n");
	xil_printf("  You should see vertical color and grayscale bars\r\n");
	xil_printf("  across your VGA Output Monitor\r\n\r\n");


	return XST_SUCCESS;
}

/*****************************************************************************/
/**
* Draws a solid box with the specified color between two points.
*
* @param	InstancePtr is a pointer to the XTft instance.
* @param	Left, Top, Right, Bottom are the edges of the box
* @param	PixelVal is the Color Value to be put at pixel.
*
* @return
*		- XST_SUCCESS if successful.
*		- XST_FAILURE if unsuccessful.
*
* @note		None.
*
******************************************************************************/
static int XTft_DrawSolidBox(XTft *InstancePtr, u32 Left, u32 Top, u32 Right,
			u32 Bottom, u32 PixelVal)
{
   u32 xmin,xmax,ymin,ymax,i,j;

   if (Left >= 0 &&
		Left <= DISPLAY_COLUMNS-1 &&
		Right >= 0 &&
		Right <= DISPLAY_COLUMNS-1 &&
		Top >= 0 &&
		Top <= DISPLAY_ROWS-1 &&
		Bottom >= 0 &&
		Bottom <= DISPLAY_ROWS-1) {
		if (Right < Left) {
			xmin = Right;
			xmax = Left;
		}
		else {
			xmin = Left;
			xmax = Right;
		}
		if (Bottom < Top) {
			ymin = Bottom;
			ymax = Top;
		}
		else {
			ymin = Top;
			ymax = Bottom;
		}

		for (i=xmin; i<=xmax; i++) {
			for (j=ymin; j<=ymax; j++) {
				XTft_SetPixel(InstancePtr, i, j, PixelVal);
			}
		}
		return XST_SUCCESS;
	}
	return XST_FAILURE;
}

