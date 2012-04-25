/* $Id: xsysace_sinit.c,v 1.1.2.1 2009/12/11 05:51:39 svemula Exp $ */
/******************************************************************************
*
* (c) Copyright 2005-2009 Xilinx, Inc. All rights reserved.
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
*
* @file xsysace_sinit.c

* The implementation of the XSysAce component's static initialzation
* functionality.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.01a jvb  10/13/05 First release
* 1.11a wgr  03/03/07 converted to new coding style.
* 2.00a ktn  10/22/09 Updated to use the HAL Processor APIs/macros.
* </pre>
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "xstatus.h"
#include "xparameters.h"
#include "xsysace.h"

/************************** Constant Definitions *****************************/


/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/


/************************** Variable Definitions *****************************/

/*****************************************************************************/
/**
*
* Looks up the device configuration based on the unique device ID. The table
* XSysAce_ConfigTable contains the configuration info for each device in the
* system.
*
* @param	DeviceId is the unique device ID to look for.
*
* @return	A pointer to the configuration data for the device,
*		or NULL if no match is found.
*
* @note		None.
*
******************************************************************************/
XSysAce_Config *XSysAce_LookupConfig(u16 DeviceId)
{
	extern XSysAce_Config XSysAce_ConfigTable[];
	XSysAce_Config *CfgPtr = NULL;
	int Index;

	for (Index = 0; Index < XPAR_XSYSACE_NUM_INSTANCES; Index++) {
		if (XSysAce_ConfigTable[Index].DeviceId == DeviceId) {
			CfgPtr = &XSysAce_ConfigTable[Index];
			break;
		}
	}

	return CfgPtr;
}

/*****************************************************************************/
/**
*
* Initialize a specific XSysAce instance. The configuration information for
* the given device ID is found and the driver instance data is initialized
* appropriately.
*
* @param	InstancePtr is a pointer to the XSysAce instance.
* @param	DeviceId is the unique id of the device controlled by this
*		XSysAce instance.
*
* @return
*		- XST_SUCCESS if successful,
*		-  XST_DEVICE_NOT_FOUND if the device was not found in the
*		   configuration table in xsysace_g.c.
*
* @note
*
* We do not want to reset the configuration controller here since this could
* cause a reconfiguration of the JTAG target chain, depending on how the
* CFGMODEPIN of the device is wired.
*
******************************************************************************/
int XSysAce_Initialize(XSysAce *InstancePtr, u16 DeviceId)
{
	XSysAce_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	InstancePtr->IsReady = 0;

	/*
	 * Lookup configuration data in the device configuration table.
	 * Use this configuration info down below when initializing this component.
	 */
	ConfigPtr = XSysAce_LookupConfig(DeviceId);

	if (ConfigPtr == (XSysAce_Config *) NULL) {
		return XST_DEVICE_NOT_FOUND;
	}

	return XSysAce_CfgInitialize(InstancePtr, ConfigPtr,
				     ConfigPtr->BaseAddress);
}
