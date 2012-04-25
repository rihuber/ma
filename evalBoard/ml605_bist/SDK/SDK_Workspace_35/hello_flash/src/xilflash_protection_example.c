/* $Id: xilflash_protection_example.c,v 1.1.2.1 2008/04/11 09:52:19 svemula Exp $ */
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
*       (c) Copyright 2010 Xilinx Inc.
*       All rights reserved.
*
******************************************************************************/
/*****************************************************************************/
/**
* @file xilflash_protection_example.c
*
*
* This file contains a design example using the Generic Flash Library.
* This example tests the Lock and UnLock features of the Flash Device.
*
* A block is Locked and an attempt is made to erase a Page in that block. This
* should return an error. The block is then unlocked and an attempt is made to
* erase a page in that block which should succeed. A page of data is written,
* read back and compared with the data written for correctness.
*
* @note		This example has been tested with an Intel CFI compliant
*		Flash device.
*		This example has not been tested with an AMD CFI compliant Flash
*		device. The AMD CFI compliant Flash devices require the user to
*		apply a 12V DC voltage on the RP pin while performing the Lock
*		and Unlock operations.
*
*<pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- ---------------------------------------------------------
* 1.01a mta  10/09/07 First release
* 1.01a sdm  02/24/09 Updated the notes with information about using the example
*		      with AMD CFI compliant Flash devices
* 2.00a ktn  12/04/09 Updated to use the HAL processor APIs/macros
* 3.00a sdm  03/03/11 Updated to pass BaseAddress and Flash Width to _Initialize
*		      API, as required by the new version of the library
*</pre>
******************************************************************************/

/***************************** Include Files *********************************/

#include "xilflash.h"
#include "xbasic_types.h"
#include <stdio.h>

/************************** Constant Definitions *****************************/

/*
 * The following constants define the baseaddress and width the flash memory.
 * These constants map to the XPAR parameters created in the xparameters.h file.
 * They are defined here such that a user can easily change all the needed
 * parameters in one place.
 */
#define FLASH_BASE_ADDRESS	XPAR_EMC_0_MEM0_BASEADDR

/*
 * The following constant defines the total byte width of the flash memory. The
 * user needs to update this width based on the flash width in the design/board.
 * The total flash width on some of the Xilinx boards is listed below.
 * -------------------------------
 * Board		Width
 * -------------------------------
 * ML403		4 (32 bit)
 * ML5xx		2 (16 bit)
 * Spartan3S 1600E	2 (16 bit)
 * Spartan-3A DSP	2 (16 bit)
 * Spartan-3A		2 (16 bit)
 * Spartan-3AN		2 (16 bit)
 * ML605		2 (16 bit)
 * SP605		2 (16 bit)
 * SP601		1 (8 bit)
 */
#define FLASH_MEM_WIDTH		2

#define FLASH_TEST_SIZE		256
#define START_ADDRESS		0x01FE0000
#define BLOCK_SIZE 			0x20000
#define NumBlocks				256

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/
int FlashProtectionExample(void);

/************************** Variable Definitions *****************************/

XFlash FlashInstance; /* XFlash Instance. */

/*
 * Buffers used during read and write transactions.
 */
u8 ReadBuffer[FLASH_TEST_SIZE]; /* Buffer used to store the data read. */
u8 WriteBuffer[FLASH_TEST_SIZE]; /* Write buffer. */

/************************** Function Definitions ******************************/

/*****************************************************************************/
/**
*
* Main function to execute the Flash protection example.
*
* @param	None
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
/*int main(void)
{
	int Status;

	Status = FlashProtectionExample();
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
xil_printf("-- Exiting Main --\r\n");
	return XST_SUCCESS;
}


*/


/*****************************************************************************/
/**
*
* This function verifies the locking and unlocking features of the Flash device.
*
* @param	None
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
int FlashProtectionExample(void)
{
	int Status;
	u32 Index;

	/*
	 * Initialize the Flash Library.
	 */
	Status = XFlash_Initialize(&FlashInstance, FLASH_BASE_ADDRESS,
				   FLASH_MEM_WIDTH, 0);
	if(Status != XST_SUCCESS) {
	xil_printf("-- Fail at Initialize --\r\n");
		return XST_FAILURE;
	}
	xil_printf("-- Initialized the Flash library successfully --\r\n");
	/*
	 * Reset the Flash Device. This clears the Status registers and puts
	 * the device in Read mode.
	 */
	Status = XFlash_Reset(&FlashInstance);
	if(Status != XST_SUCCESS) {
	xil_printf("-- Fail at reset --\r\n");
		return XST_FAILURE;
	}

	/*
	 * Unlock all the Blocks.
	 */
	for (Index = 0; Index < NumBlocks; Index++)
	{
		Status = XFlash_Unlock(&FlashInstance, (Index * BLOCK_SIZE), 0);
		if(Status != XST_SUCCESS) {
		xil_printf("-- Unlock error --\r\n");
		return XST_FAILURE;
		}
	}
    	xil_printf("-- Unlocked all the blocks successfully --\r\n");

	/*
	 * Perform the Erase operation. This should succeed as the block is
	 * unlocked.
	 */
	Status = XFlash_Erase(&FlashInstance, START_ADDRESS, FLASH_TEST_SIZE);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
		xil_printf("-- Erase failed --\r\n");
	}
   	xil_printf("-- Erased the Flash memory contents at offset 0x%07x successfully --\r\n",
	    START_ADDRESS);

	/*
	 * Prepare the write buffer. Fill in the data need to be written into
	 * Flash Device.
	 */
    xil_printf("-- Writing: ");
	for(Index = 0; Index < FLASH_TEST_SIZE; Index++) {
		WriteBuffer[Index] = (u8)Index;
        xil_printf("%02x", WriteBuffer[Index]);
	}
    xil_printf("\r\n");

	/*
	 * Perform the Write operation.
	 */
	Status = XFlash_Write(&FlashInstance, START_ADDRESS, FLASH_TEST_SIZE,
			      WriteBuffer);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
  	xil_printf("-- Write operation at offset 0x%07x completed successfully --\r\n", 
	    START_ADDRESS);
	/*
	 * Perform the read operation.
	 */
	Status = XFlash_Read(&FlashInstance, START_ADDRESS, FLASH_TEST_SIZE,
			     ReadBuffer);
		if(Status != XST_SUCCESS) {
			return XST_FAILURE;
	}
   	xil_printf("-- Read operation completed successfully --\r\n");
	/*
	 * Compare the data read against the data Written.
	 */
	for(Index = 0; Index < FLASH_TEST_SIZE; Index++) {
		if(ReadBuffer[Index] != (u8)Index) {
			return XST_FAILURE;
		}
	}
   	xil_printf("-- Data comparison successful --\r\n");
	return XST_SUCCESS;
}
