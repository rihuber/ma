/* $Id: xiic_eeprom_example.c,v 1.1.4.2 2009/03/24 05:47:25 svemula Exp $
*/
/******************************************************************************
*
* (c) Copyright 2006-2009 Xilinx, Inc. All rights reserved.
*
* This file contains confidential and proprietary information of Xilinx, Inc.
* and is protected under U.S. and international copyright and other
* intellectual property laws.
*
* DISCLAIMER
* This disclaimer is not a license and does not grant any rights to the
* materials distributed herewith. Except as otherwise provided in a valid
* license issued to you by Xilinx, and to the maximum extent permitted by
* applicable law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL
* FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS,
* IMPLIED, OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
* MERCHANTABILITY, NON-INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE;
* and (2) Xilinx shall not be liable (whether in contract or tort, including
* negligence, or under any other theory of liability) for any loss or damage
* of any kind or nature related to, arising under or in connection with these
* materials, including for any direct, or any indirect, special, incidental,
* or consequential loss or damage (including loss of data, profits, goodwill,
* or any type of loss or damage suffered as a result of any action brought by
* a third party) even if such damage or loss was reasonably foreseeable or
* Xilinx had been advised of the possibility of the same.
*
* CRITICAL APPLICATIONS
* Xilinx products are not designed or intended to be fail-safe, or for use in
* any application requiring fail-safe performance, such as life-support or
* safety devices or systems, Class III medical devices, nuclear facilities,
* applications related to the deployment of airbags, or any other applications
* that could lead to death, personal injury, or severe property or
* environmental damage (individually and collectively, "Critical
* Applications"). Customer assumes the sole risk and liability of any use of
* Xilinx products in Critical Applications, subject only to applicable laws
* and regulations governing limitations on product liability.
*
* THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE
* AT ALL TIMES.
*
******************************************************************************/
/*****************************************************************************/
/**
* @file xiic_eeprom_example.c
*
* This file consists of a Interrupt mode design example which uses the Xilinx
* IIC device and XIic driver to exercise the EEPROM. The XIic driver uses the
* complete FIFO functionality to transmit/receive data.
*
* The XIic_MasterSend() API is used to transmit the data and
* XIic_MasterRecv() API is used to receive the data.
*
* The example is tested on ML300/ML310/ML403/ML501 Xilinx boards.
*
* The ML310 board has an IIC EEPROM on the board which is a 8Kbyte serial
* EEPROM(Microchip 24LC64A). The WP pin of the EEPROM-Microchip 24LC64A
* is hardwired to ground on this board.
*
* The ML300 board has an IIC EEPROM on the board which is a 4Kbyte serial
* EEPROM(Microchip 24LC32A). The WP pin of the EEPROM-Microchip 24LC32A has to
* be connected to ground for this example. The WP is connected to pin Y3 of
* the FPGA.
*
* The ML403/ML501 board has an IIC EEPROM on the board which is a 1Kbyte serial
* EEPROM(Microchip 24LC04A). The WP pin of the EEPROM-Microchip 24LC04A
* is hardwired to ground on this board.
*
* The AddressType for ML300/ML310 boards should be u16 as the address
* pointer in the on board EEPROM is 2 bytes.
* The AddressType for ML403/ML501 boards should be u8 as the address pointer in
* the on board EEPROM is 1 bytes.
*
* This code assumes that no Operating System is being used.
*
* @note
*
* None.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- ---------------------------------------------------------
* 1.00a mta  02/24/06 Created.
* 1.00a mta  04/05/07 Added support for microblaze.
* </pre>
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "xparameters.h"
#include "xiic.h"
#include "xintc.h"
#include "xuartns550_l.h"
#include "xbasic_types.h"


#if !SIM
#include "stdio.h"
#endif


#ifdef __MICROBLAZE__
#include "mb_interface.h"
#endif

#ifdef __PPC__
#include "xexception_l.h"
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
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define IIC_DEVICE_ID               XPAR_IIC_EEPROM_DEVICE_ID
#define INTC_DEVICE_ID              XPAR_INTERRUPT_CNTLR_DEVICE_ID
#define IIC_INTR_ID                 XPAR_INTC_0_IIC_0_VEC_ID

/*
 * The following constant defines the address of the IIC Slave device on the
 * IIC bus. Note that since the address is only 7 bits, this constant is the
 * address divided by 2.
 */
#define EEPROM_ADDRESS 0x54	/* 0xA0 as an 8 bit number. */

/*
 * The page size determines how much data should be written at a time.
 * The ML310/ML300 board supports a page size of 32 and 16.
 * The write function should be called with this as a maximum byte count.
 */
#define PAGE_SIZE   16

/*
 * The Starting address in the IIC EEPROM on which this test is performed.
 */
#define EEPROM_TEST_START_ADDRESS   128

/**************************** Type Definitions *******************************/

/*
 * The AddressType for ML300/ML310 boards should be u16 as the address
 * pointer in the on board EEPROM is 2 bytes.
 * The AddressType for ML403 boards should be u8 as the address pointer in the
 * on board EEPROM is 1 bytes.
 */
typedef u8 AddressType;

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

#if SIM
static void printf(char *s)
{
  while (*s) {
    XUartNs550_SendByte(UART_BASEADDR, *s);
    s++;
  }
}
#endif


int IicEepromExample();

int EepromWriteData(u16 ByteCount);

int EepromReadData(u8 *BufferPtr, u16 ByteCount);

static int SetupInterruptSystem(XIic * IicInstPtr);

static void SendHandler(XIic * InstancePtr);

static void ReceiveHandler(XIic * InstancePtr);

static void StatusHandler(XIic * InstancePtr, int Event);

/************************** Variable Definitions *****************************/

XIic IicInstance;		/* The instance of the IIC device. */
XIntc InterruptController;	/* The instance of the Interrupt Controller. */

/*
 * Write buffer for writing a page.
 */
u8 WriteBuffer[sizeof(AddressType) + PAGE_SIZE];

u8 ReadBuffer[PAGE_SIZE];	/* Read buffer for reading a page. */

volatile u8 TransmitComplete;	/* Flag to check completion of Transmission */
volatile u8 ReceiveComplete;	/* Flag to check completion of Reception */

/************************** Function Definitions *****************************/

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
* Main function to call the High level EEPROM example.
*
* @param	None.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
int main(void)
{
    int Status;

    XUartNs550_SetBaud(UART_BASEADDR, UART_CLOCK, UART_BAUDRATE);
    XUartNs550_SetLineControlReg(UART_BASEADDR, XUN_LCR_8_DATA_BITS);

    xil_printf("\n\r********************************************************");
    xil_printf("\n\r********************************************************");
    xil_printf("\n\r**     ML605 - IIC EEPROM Test                        **");
    xil_printf("\n\r********************************************************");
    xil_printf("\n\r********************************************************\r\n");
    xil_printf("IIC EEPROM Test:\r\n");

    /*
     * Run the High Level EEPROM example.
     */
	Status = IicEepromExample();
	if (Status != XST_SUCCESS) {
		xil_printf("Failed\r\n");
		return XST_FAILURE;
	}
	xil_printf("Passed!\r\n");

	return_to_loader();

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
* This function writes, reads, and verifies the data to the IIC EEPROM. It
* does the write as a single page write, performs a buffered read.
*
* @param	None.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
int IicEepromExample()
{
	u32 Index;
	int Status;
	AddressType Address = EEPROM_TEST_START_ADDRESS;

	/*
	 * Initialize the IIC device Instance.
	 */
	Status = XIic_Initialize(&IicInstance, IIC_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Setup the Interrupt System.
	 */
	Status = SetupInterruptSystem(&IicInstance);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Set the Handlers for transmit and reception.
	 */
	XIic_SetSendHandler(&IicInstance, &IicInstance,
			    (XIic_Handler) SendHandler);
	XIic_SetRecvHandler(&IicInstance, &IicInstance,
			    (XIic_Handler) ReceiveHandler);
	XIic_SetStatusHandler(&IicInstance, &IicInstance,
			      (XIic_StatusHandler) StatusHandler);

	/*
	 * Set the Slave address.
	 */
	Status = XIic_SetAddress(&IicInstance, XII_ADDR_TO_SEND_TYPE,
				 EEPROM_ADDRESS);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Initialize the data to write and the read buffer.
	 */
	if (sizeof(Address) == 1) {
		WriteBuffer[0] = (u8) (Address);
	}
	else {
		WriteBuffer[0] = (u8) (Address >> 8);
		WriteBuffer[1] = (u8) (Address);
		ReadBuffer[Index] = 0;
	}

    xil_printf("Writing data to eeprom at 0x%02x: ", Address);
	for (Index = 0; Index < PAGE_SIZE; Index++) {
		WriteBuffer[sizeof(Address) + Index] = Index;
		ReadBuffer[Index] = 0;
        xil_printf("%02x", WriteBuffer[sizeof(Address) + Index]);
	}
    xil_printf("\r\n");

	/*
	 * Write to the EEPROM.
	 */
	Status = EepromWriteData(sizeof(Address) + PAGE_SIZE);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Read from the EEPROM.
	 */
    xil_printf("Reading data from EEPROM\r\n");
	Status = EepromReadData(ReadBuffer, PAGE_SIZE);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Verify the data read against the data written.
	 */
    xil_printf("Comparing read data\r\n");
	for (Index = 0; Index < PAGE_SIZE; Index++) {
		if (ReadBuffer[Index] != WriteBuffer[Index + sizeof(Address)]) {
			return XST_FAILURE;
		}

		ReadBuffer[Index] = 0;
	}

	/*
	 * Initialize the data to write and the read buffer.
	 */
	if (sizeof(Address) == 1) {
		WriteBuffer[0] = (u8) (Address);
	}
	else {
		WriteBuffer[0] = (u8) (Address >> 8);
		WriteBuffer[1] = (u8) (Address);
		ReadBuffer[Index] = 0;
	}

	for (Index = 0; Index < PAGE_SIZE; Index++) {
		WriteBuffer[sizeof(Address) + Index] = Index;
		ReadBuffer[Index] = 0;
	}

	/*
	 * Write to the EEPROM.
	 */
	Status = EepromWriteData(sizeof(Address) + PAGE_SIZE);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Read from the EEPROM.
	 */
	Status = EepromReadData(ReadBuffer, PAGE_SIZE);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Verify the data read against the data written.
	 */
	for (Index = 0; Index < PAGE_SIZE; Index++) {
		if (ReadBuffer[Index] != WriteBuffer[Index + sizeof(Address)]) {
			return XST_FAILURE;
		}

		ReadBuffer[Index] = 0;
	}

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
* This function writes a buffer of data to the IIC serial EEPROM.
*
* @param	ByteCount contains the number of bytes in the buffer to be
*		written.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		The Byte count should not exceed the page size of the EEPROM as
*		noted by the constant PAGE_SIZE.
*
******************************************************************************/
int EepromWriteData(u16 ByteCount)
{
	int Status;

	/*
	 * Set the defaults.
	 */
	TransmitComplete = 1;
	IicInstance.Stats.TxErrors = 0;

	/*
	 * Start the IIC device.
	 */
	Status = XIic_Start(&IicInstance);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Send the Data.
	 */
	Status = XIic_MasterSend(&IicInstance, WriteBuffer, ByteCount);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Wait till the transmission is completed.
	 */
	while ((TransmitComplete) || (XIic_IsIicBusy(&IicInstance) == TRUE)) {
		/*
		 * This condition is required to be checked in the case where we
		 * are writing two consecutive buffers of data to the EEPROM.
		 * The EEPROM takes about 2 milliseconds time to update the data
		 * internally after a STOP has been sent on the bus.
		 * A NACK will be generated in the case of a second write before
		 * the EEPROM updates the data internally resulting in a
		 * Transmission Error.
		 */
		if (IicInstance.Stats.TxErrors != 0) {


			/*
			 * Enable the IIC device.
			 */
			Status = XIic_Start(&IicInstance);
			if (Status != XST_SUCCESS) {
				return XST_FAILURE;
			}


			if (!XIic_IsIicBusy(&IicInstance)) {
				/*
				 * Send the Data.
				 */
				Status = XIic_MasterSend(&IicInstance,
							 WriteBuffer,
							 ByteCount);
				if (Status == XST_SUCCESS) {
					IicInstance.Stats.TxErrors = 0;
				}
				else {
				}
			}
		}
	}

	/*
	 * Stop the IIC device.
	 */
	Status = XIic_Stop(&IicInstance);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
* This function reads data from the IIC serial EEPROM into a specified buffer.
*
* @param	BufferPtr contains the address of the data buffer to be filled.
* @param	ByteCount contains the number of bytes in the buffer to be read.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
int EepromReadData(u8 *BufferPtr, u16 ByteCount)
{
	int Status;
	AddressType Address = EEPROM_TEST_START_ADDRESS;

	/*
	 * Set the Defaults.
	 */
	ReceiveComplete = 1;

	/*
	 * Position the Pointer in EEPROM.
	 */
	if (sizeof(Address) == 1) {
		WriteBuffer[0] = (u8) (Address);
	}
	else {
		WriteBuffer[0] = (u8) (Address >> 8);
		WriteBuffer[1] = (u8) (Address);
	}

	Status = EepromWriteData(sizeof(Address));
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Start the IIC device.
	 */
	Status = XIic_Start(&IicInstance);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Receive the Data.
	 */
	Status = XIic_MasterRecv(&IicInstance, BufferPtr, ByteCount);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Wait till all the data is received.
	 */
	while ((ReceiveComplete) || (XIic_IsIicBusy(&IicInstance) == TRUE)) {

	}

	/*
	 * Stop the IIC device.
	 */
	Status = XIic_Stop(&IicInstance);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
* This function setups the interrupt system so interrupts can occur for the
* IIC device. The function is application-specific since the actual system may
* or may not have an interrupt controller. The IIC device could be directly
* connected to a processor without an interrupt controller. The user should
* modify this function to fit the application.
*
* @param	IicInstPtr contains a pointer to the instance of the IIC device
*		which is going to be connected to the interrupt controller.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
static int SetupInterruptSystem(XIic * IicInstPtr)
{
	int Status;

	if (InterruptController.IsStarted == XCOMPONENT_IS_STARTED) {
		return XST_SUCCESS;
	}

	/*
	 * Initialize the interrupt controller driver so that it's ready to use.
	 */
	Status = XIntc_Initialize(&InterruptController, INTC_DEVICE_ID);

	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Connect the device driver handler that will be called when an
	 * interrupt for the device occurs, the handler defined above performs
	 * the specific interrupt processing for the device.
	 */
	Status = XIntc_Connect(&InterruptController, IIC_INTR_ID,
			       (XInterruptHandler) XIic_InterruptHandler,
			       IicInstPtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Start the interrupt controller so interrupts are enabled for all
	 * devices that cause interrupts.
	 */
	Status = XIntc_Start(&InterruptController, XIN_REAL_MODE);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Enable the interrupts for the IIC device.
	 */
	XIntc_Enable(&InterruptController, IIC_INTR_ID);

#ifdef __PPC__
	/*
	 * Initialize the PPC exception table.
	 */
	XExc_Init();

	/*
	 * Register the interrupt controller handler with the exception table.
	 */
	XExc_RegisterHandler(XEXC_ID_NON_CRITICAL_INT,
			     (XExceptionHandler) XIntc_InterruptHandler,
			     &InterruptController);

	/*
	 * Enable non-critical exceptions.
	 */
	XExc_mEnableExceptions(XEXC_NON_CRITICAL);
#endif

#ifdef __MICROBLAZE__
	/*
	 * Enable the Microblaze Interrupts.
	 */
	microblaze_enable_interrupts();
#endif

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
* This Send handler is called asynchronously from an interrupt
* context and indicates that data in the specified buffer has been sent.
*
* @param	InstancePtr is not used, but contains a pointer to the IIC
*		device driver instance which the handler is being called for.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
static void SendHandler(XIic * InstancePtr)
{
	TransmitComplete = 0;
}

/*****************************************************************************/
/**
* This Receive handler is called asynchronously from an interrupt
* context and indicates that data in the specified buffer has been Received.
*
* @param	InstancePtr is not used, but contains a pointer to the IIC
*		device driver instance which the handler is being called for.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
static void ReceiveHandler(XIic * InstancePtr)
{
	ReceiveComplete = 0;
}

/*****************************************************************************/
/**
* This Status handler is called asynchronously from an interrupt
* context and indicates the events that have occurred.
*
* @param	InstancePtr is a pointer to the IIC driver instance for which
*		the handler is being called for.
* @param	Event indicates the condition that has occurred.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
static void StatusHandler(XIic * InstancePtr, int Event)
{

}
