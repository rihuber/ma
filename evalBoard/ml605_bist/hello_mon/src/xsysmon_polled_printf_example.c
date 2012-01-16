/* $Id: xsysmon_polled_printf_example.c,v 1.2 2008/07/30 15:31:13 svemula Exp $ */
/*****************************************************************************
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
*       (c) Copyright 2007-2009 Xilinx Inc.
*       All rights reserved.
*
*****************************************************************************/
/****************************************************************************/
/**
*
* @file xsysmon_polled_printf_example.c
*
* This file contains a design example using the driver functions
* of the System Monitor driver. The example here shows the
* driver/device in polled mode to check the on-chip temperature and voltages.
*
* @note
*
* This examples also assumes that there is a STDIO device in the system.
*
* <pre>
*
* MODIFICATION HISTORY:
*
* Ver   Who    Date     Changes
* ----- -----  -------- -----------------------------------------------------
* 1.00a xd/sv  05/22/07 First release
* 2.00a sv     06/22/08 Added printfs and used conversion macros
*
* </pre>
*
*****************************************************************************/

/***************************** Include Files ********************************/

#include "xsysmon.h"
#include "xuartns550_l.h"
#include "xparameters.h"
#include "xstatus.h"
#include "stdio.h"

/************************** Constant Definitions ****************************/

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define SYSMON_DEVICE_ID  XPAR_SYSTEM_MONITOR_DEVICE_ID

#ifndef UART_BASEADDR
#define UART_BASEADDR XPAR_UARTNS550_0_BASEADDR
#endif
#define UART_CLOCK    XPAR_UARTNS550_0_CLOCK_FREQ_HZ

#if !SIM
#define UART_BAUDRATE 9600                    /* real hardware */
#else
#define UART_BAUDRATE (UART_CLOCK / 16 / 3)     /* simulation */
#endif

// System Monitor ML605 Board Sensor Definitions
#define I_VCCINT_FACTOR   200.0f	    				// IS 1/Rshunt (5mOhm) for vccint
#define I_VCCINT_CHAN     XSM_CH_VPVN  			// This is the name of the sysmon channel the shunt is connected to
#define V_BOARD_CHAN      (XSM_CH_AUX_MIN+12)	// This is the external channel (aux12) that is connected to the board supply sensor
#define V_BOARD_FS        24.0f        			// This is the full-scale voltage on the board supply sensor
#define I_BOARD_CHAN      (XSM_CH_AUX_MIN+13)  	// This is the external channel (aux13) for the board current sensor
#define I_BOARD_RSHUNT    2e-3						// This is the value of the Shunt Resistor in mOhms
#define I_BOARD_GAIN      50.0f						// The Board has a gain of 50x at the output of the shunt resistor


/**************************** Type Definitions ******************************/


/***************** Macros (Inline Functions) Definitions ********************/

#define printf xil_printf /* Small foot-print printf function */

/************************** Function Prototypes *****************************/

static int SysMonPolledPrintfExample(u16 SysMonDeviceId);
static int SysMonFractionToInt(float FloatNum);

/************************** Variable Definitions ****************************/

static XSysMon SysMonInst;      /* System Monitor driver instance */

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

/****************************************************************************/
/**
*
* Main function that invokes the polled example in this file.
*
* @param	None.
*
* @return
*		- XST_SUCCESS if the example has completed successfully.
*		- XST_FAILURE if the example has failed.
*
* @note		None.
*
*****************************************************************************/
int main(void)
{
	int Status;

	XUartNs550_SetBaud(UART_BASEADDR, UART_CLOCK, UART_BAUDRATE);
	XUartNs550_SetLineControlReg(UART_BASEADDR, XUN_LCR_8_DATA_BITS);

  xil_printf("\n\r********************************************************");
  xil_printf("\n\r********************************************************");
  xil_printf("\n\r**     ML605 - Sysmon Test                            **");
  xil_printf("\n\r********************************************************");
  xil_printf("\n\r********************************************************\r\n");

	/*
	 * Run the SysMonitor polled example, specify the Device ID that is
	 * generated in xparameters.h.
	 */
	Status = SysMonPolledPrintfExample(SYSMON_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return_to_loader();

	return(0);
}


/****************************************************************************/
/**
*
* This function runs a test on the System Monitor/ADC device using the
* driver APIs.
* This function does the following tasks:
*	- Initiate the System Monitor device driver instance
*	- Run self-test on the device
*	- Setup the sequence registers to continuously monitor on-chip
*	temperature, VCCINT and VCCAUX
*	- Setup configuration registers to start the sequence
*	- Read the latest on-chip temperature, VCCINT and VCCAUX
*
* @param	SysMonDeviceId is the XPAR_<SYSMON_ADC_instance>_DEVICE_ID value
*		from xparameters.h.
*
* @return
*		- XST_SUCCESS if the example has completed successfully.
*		- XST_FAILURE if the example has failed.
*
* @note   	None
*
****************************************************************************/
int SysMonPolledPrintfExample(u16 SysMonDeviceId)
{
	int Status;
	XSysMon_Config *ConfigPtr;
	u32 TempRawData, VccAuxRawData, VccIntRawData;
	u32 vint_shuntRaw, vboardRaw, vboard_shuntRaw;
	u32 channels;
	float TempData, VccAuxData, VccIntData, MaxData, MinData;
	float vint_shuntmv, iccintData, pccintData, iboardData, vboardData;
	XSysMon *SysMonInstPtr = &SysMonInst;

	printf("\r\nEntering the SysMon Polled Example. \r\n");

	/*
	 * Initialize the SysMon driver.
	 */
	ConfigPtr = XSysMon_LookupConfig(SysMonDeviceId);
	if (ConfigPtr == NULL) {
		return XST_FAILURE;
	}
	XSysMon_CfgInitialize(SysMonInstPtr, ConfigPtr,
				ConfigPtr->BaseAddress);

	/*
	 * Disable the Channel Sequencer before configuring the Sequence
	 * registers.
	 */
	XSysMon_SetSequencerMode(SysMonInstPtr, XSM_SEQ_MODE_SINGCHAN);

	/*
	 * Disable all the alarms in the Configuration Register 1.
	 */
	XSysMon_SetAlarmEnables(SysMonInstPtr, 0x0);

	/*
	 * Setup the Averaging to be done for the channels in the
	 * Configuration 0 register as 16 samples:
	 */
	XSysMon_SetAvg(SysMonInstPtr, XSM_AVG_16_SAMPLES);

	/*
	 * Setup the Sequence register for Vp/Vn channel
	 * Setting is: Bipolar Mode
	 *
	 */
	Status = XSysMon_SetSeqInputMode(SysMonInstPtr, XSM_SEQ_CH_VPVN);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}



    /*
	 * Enable the following channels in the Sequencer registers:
	 * 	- On-chip Temperature, VCCINT/VCCAUX supply sensors
	 * 	- vp/vn 12th/13th Auxiliary Channel (board vccint current, 12v suppy, 12v current)
	 *		- Calibration Channel
	 */

	channels =  XSM_SEQ_CH_CALIB  | XSM_SEQ_CH_TEMP | XSM_SEQ_CH_VCCAUX |
					XSM_SEQ_CH_VCCINT | XSM_SEQ_CH_VPVN | XSM_SEQ_CH_AUX12 | XSM_SEQ_CH_AUX13;

	Status =  XSysMon_SetSeqChEnables(SysMonInstPtr, channels);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Enable the averaging on all the channels in the Sequencer
	 */
	Status =  XSysMon_SetSeqAvgEnables(SysMonInstPtr, channels);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Set the ADCCLK frequency equal to about 5MHz for the System
	 * Monitor/ADC in the Configuration Register 2. (1-5MHz is valid operating range)
	 * 32 is used, as for system frequecy of 75-150MHz gives 2.3-4.6MHz ADC clock)
	 */
	XSysMon_SetAdcClkDivisor(SysMonInstPtr, 32);


	/*
	 * Set the Calibration enables.
	 */
	XSysMon_SetCalibEnables(SysMonInstPtr,
				XSM_CFR1_CAL_PS_GAIN_OFFSET_MASK |
				XSM_CFR1_CAL_ADC_GAIN_OFFSET_MASK);

	/*
	 * Enable the Channel Sequencer in continuous sequencer cycling mode.
	 * to begin the conversion
	 */
	XSysMon_SetSequencerMode(SysMonInstPtr, XSM_SEQ_MODE_CONTINPASS);

	/*
	 * Wait till the End of Sequence occurs
	 */
	XSysMon_GetStatus(SysMonInstPtr); /* Clear the old status */
	while ((XSysMon_GetStatus(SysMonInstPtr) & XSM_SR_EOS_MASK) !=
			XSM_SR_EOS_MASK);

	/*
	 * Read the on-chip Temperature Data (Current/Maximum/Minimum)
	 * from the ADC data registers.
	 */
	TempRawData = XSysMon_GetAdcData(SysMonInstPtr, XSM_CH_TEMP);
	TempData = XSysMon_RawToTemperature(TempRawData);
	printf("\r\nThe Present Temperature is %0d.%03d Centigrades.\r\n",
				(int)(TempData), SysMonFractionToInt(TempData));


	TempRawData = XSysMon_GetMinMaxMeasurement(SysMonInstPtr, XSM_MAX_TEMP);
	MaxData = XSysMon_RawToTemperature(TempRawData);
	printf("The Maximum Temperature is %0d.%03d Centigrades. \r\n",
				(int)(MaxData), SysMonFractionToInt(MaxData));

	TempRawData = XSysMon_GetMinMaxMeasurement(SysMonInstPtr, XSM_MIN_TEMP);
	MinData = XSysMon_RawToTemperature(TempRawData);
	printf("The Minimum Temperature is %0d.%03d Centigrades. \r\n",
				(int)(MinData), SysMonFractionToInt(MinData));

	/*
	 * Read the VccInt Votage Data (Current/Maximum/Minimum) from the
	 * ADC data registers.
	 */
	VccIntRawData = XSysMon_GetAdcData(SysMonInstPtr, XSM_CH_VCCINT);
	VccIntData = XSysMon_RawToVoltage(VccIntRawData);
	printf("\r\nThe Present VCCINT is %0d.%03d Volts. \r\n",
			(int)(VccIntData), SysMonFractionToInt(VccIntData));

	VccIntRawData = XSysMon_GetMinMaxMeasurement(SysMonInstPtr,
							XSM_MAX_VCCINT);
	MaxData = XSysMon_RawToVoltage(VccIntRawData);
	printf("The Maximum VCCINT is %0d.%03d Volts. \r\n",
			(int)(MaxData), SysMonFractionToInt(MaxData));

	VccIntRawData = XSysMon_GetMinMaxMeasurement(SysMonInstPtr,
							XSM_MIN_VCCINT);
	MinData = XSysMon_RawToVoltage(VccIntRawData);
	printf("The Minimum VCCINT is %0d.%03d Volts. \r\n",
			(int)(MinData), SysMonFractionToInt(MinData));

	/*
	 * Read the VccAux Votage Data (Current/Maximum/Minimum) from the
	 * ADC data registers.
	 */
	VccAuxRawData = XSysMon_GetAdcData(SysMonInstPtr, XSM_CH_VCCAUX);
	VccAuxData = XSysMon_RawToVoltage(VccAuxRawData);
	printf("\r\nThe Present VCCAUX is %0d.%03d Volts. \r\n",
			(int)(VccAuxData), SysMonFractionToInt(VccAuxData));

	VccAuxRawData = XSysMon_GetMinMaxMeasurement(SysMonInstPtr,
							XSM_MAX_VCCAUX);
	MaxData = XSysMon_RawToVoltage(VccAuxRawData);
	printf("The Maximum VCCAUX is %0d.%03d Volts. \r\n",
				(int)(MaxData), SysMonFractionToInt(MaxData));


	VccAuxRawData = XSysMon_GetMinMaxMeasurement(SysMonInstPtr,
							XSM_MIN_VCCAUX);
	MinData = XSysMon_RawToVoltage(VccAuxRawData);
	printf("The Minimum VCCAUX is %0d.%03d Volts. \r\n\r\n",
				(int)(MinData), SysMonFractionToInt(MinData));


	/*
	 * Read the ml605 board sensors - vccint current, 12v supply, 12v current
	 */
	vint_shuntRaw   = XSysMon_GetAdcData(SysMonInstPtr, I_VCCINT_CHAN);
	vboardRaw       = XSysMon_GetAdcData(SysMonInstPtr, V_BOARD_CHAN);
	vboard_shuntRaw = XSysMon_GetAdcData(SysMonInstPtr, I_BOARD_CHAN);

	/*
	 * Convert Board sensor data to readable format, and print out
	 */
	vint_shuntmv = ((float)vint_shuntRaw*1000.0f)/65536.0f;
	iccintData   = (vint_shuntmv*I_VCCINT_FACTOR)/1000.0f;			// I = V / R
	pccintData   = iccintData * VccIntData;

	iboardData = ((float)vboard_shuntRaw/(I_BOARD_GAIN*I_BOARD_RSHUNT))/65536.0f;	// I = (v/gain/) / R
	vboardData = ((float)vboardRaw*V_BOARD_FS)/65536.0f;

	printf("The VCCINT Current is %0d.%03d Amps. \r\n", (int)(iccintData), SysMonFractionToInt(iccintData));
	printf("The VCCINT Power   is %0d.%03d Watts. \r\n", (int)(pccintData), SysMonFractionToInt(pccintData));
	printf("The ML605 12v Voltage is %0d.%03d Volts. \r\n", (int)(vboardData), SysMonFractionToInt(vboardData));
	printf("The ML605 12v Current is %0d.%03d Amps. \r\n", (int)(iboardData), SysMonFractionToInt(iboardData));

	printf("\r\nExiting the SysMon Polled Example. \r\n");

	return XST_SUCCESS;
}


/****************************************************************************/
/*
*
* This function converts the fraction part of the given floating point number
* (after the decimal point)to an integer.
*
* @param	FloatNum is the floating point number.
*
* @return	Integer number to a precision of 3 digits.
*
* @note
* This function is used in the printing of floating point data to a STDIO device
* using the xil_printf function. The xil_printf is a very small foot-print
* printf function and does not support the printing of floating point numbers.
*
*****************************************************************************/
int SysMonFractionToInt(float FloatNum)
{
	float Temp;

	Temp = FloatNum;
	if (FloatNum < 0) {
		Temp = -(FloatNum);
	}

	return( ((int)((Temp -(float)((int)Temp)) * (1000.0f))));
}

