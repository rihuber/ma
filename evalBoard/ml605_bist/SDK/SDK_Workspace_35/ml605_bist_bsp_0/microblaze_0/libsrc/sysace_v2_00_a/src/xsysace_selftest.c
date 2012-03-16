/* $Id: xsysace_selftest.c,v 1.1.2.1 2009/12/11 05:51:39 svemula Exp $ */
/******************************************************************************
*
* (c) Copyright 2002-2009 Xilinx, Inc. All rights reserved.
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
* @file xsysace_selftest.c
*
* Contains diagnostic functions for the System ACE device and driver. This
* includes a self-test to make sure communication to the device is possible
* and the ability to retrieve the ACE controller version.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00a rpm  06/17/02 work in progress
* 1.11a wgr  07/03/07 converted to new coding style.
* 2.00a ktn  10/22/09 Updated to use the HAL Processor APIs/macros.
*		      Updated to use the driver macros that have been renamed to
*		      remove _m from the name.
* </pre>
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "xsysace.h"
#include "xsysace_l.h"

/************************** Constant Definitions *****************************/


/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/


/************************** Variable Definitions *****************************/


/*****************************************************************************/
/**
*
* A self-test that simply proves communication to the ACE controller from the
* device driver by obtaining an MPU lock, verifying it, then releasing it.
*
* @param	InstancePtr is a pointer to the XSysAce instance to be worked on.
*
* @return
*		- XST_SUCCESS if self-test passes.
*		- XST_FAILURE if an error occurs.
*
* @note		None.
*
******************************************************************************/
int XSysAce_SelfTest(XSysAce *InstancePtr)
{
	int Result;

	Xil_AssertNonvoid(InstancePtr != NULL);
	Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

	/*
	 * Grab a lock (expect immediate success)
	 */
	Result = XSysAce_Lock(InstancePtr, TRUE);
	if (Result != XST_SUCCESS) {
		return Result;
	}

	/*
	 * Verify the lock was retrieved
	 */
	if (!XSysAce_IsMpuLocked(InstancePtr->BaseAddress)) {
		return XST_FAILURE;
	}

	/*
	 * Release the lock
	 */
	XSysAce_Unlock(InstancePtr);

	/*
	 * Verify the lock was released
	 */
	if (XSysAce_IsMpuLocked(InstancePtr->BaseAddress)) {
		return XST_FAILURE;
	}

	/*
	 * If there are currently any errors on the device, fail self-test
	 */
	if (XSysAce_GetErrorReg(InstancePtr->BaseAddress) != 0) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}


/*****************************************************************************/
/**
*
* Retrieve the version of the System ACE device.
*
* @param	InstancePtr is a pointer to the XSysAce instance.
*
* @return
*
* A 16-bit version where the 4 most significant bits are the major version
* number, the next four bits are the minor version number, and the least
* significant 8 bits are the revision or build number.
*
* @note		None.
*
******************************************************************************/
u16 XSysAce_GetVersion(XSysAce *InstancePtr)
{
	Xil_AssertNonvoid(InstancePtr != NULL);
	Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

	return XSysAce_RegRead16(InstancePtr->BaseAddress + XSA_VR_OFFSET);
}
