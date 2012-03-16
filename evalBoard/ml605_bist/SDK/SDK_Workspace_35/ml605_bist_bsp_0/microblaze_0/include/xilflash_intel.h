/* $Id: xilflash_intel.h,v 1.1.2.2 2011/03/25 09:06:57 sadanan Exp $ */
/******************************************************************************
*
* (c) Copyright 2007-2011 Xilinx, Inc. All rights reserved.
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
* @file xilflash_intel.h
*
* This file consists definitions, Macros and structures specific to the Intel
* flash devices.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00a rmm  10/25/07 First release
* 1.00a mta  10/25/07 Updated to flash library
* 1.01a ksu  04/10/08 Added support for AMD CFI Interface
* 1.02a ksu  06/16/09 Added support for multiple banks in Intel flash.
*                     Added Reset Bank function.
*                     Added support for 0xF0 reset command.
*                     Added XFL_DEVCTL_SET_CONFIG_REG IOCTL to write to the
*                     Configuration Register of the Xilinx Platform Flash XL
*                     which can be used to set the Flash in Sync/Async mode.
*                     The Xilinx Platform Flash XL is set to Async mode during
*                     the initialization of the library.
*                     Added bank(s) reset function at the top of the read
*                     function.
*                     Updated Lock and Unlock operations for multiple blocks.
*
* </pre>
*
******************************************************************************/

#ifndef XFLASH_INTEL_H		/* prevent circular inclusions */
#define XFLASH_INTEL_H		/* by using protection macros */

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/

#ifdef XPAR_XFL_DEVICE_FAMILY_INTEL
#include "xilflash.h"

/************************** Constant Definitions *****************************/

/**
 * Supported manufacturer IDs. Note, that not all parts from these listed
 * vendors are supported.
 */
#define XFL_MANUFACTURER_ID_INTEL		0x89

/**
 * RYBY Options
 * These options control the RYBY signal. They can be accessed with the
 * XFL_INTEL_DEVCTL_SET_RYBY command.
 */
#define XFL_INTEL_RYBY_PULSE_OFF		0	/* Do not pulse */
#define XFL_INTEL_RYBY_PULSE_ON_ERASE		1	/* Pulse on erase
							 * complete only */
#define XFL_INTEL_RYBY_PULSE_ON_PROG		2	/* Pulse on program
							 * complete only */
#define XFL_INTEL_RYBY_PULSE_ON_ERASE_PROG	3	/* Pulse on erase &
							 * program complete */

/*
 * Status register bit definitions. Use these bitmaps to decipher the
 * return value of the XFL_INTEL_DEVCTL_GET_LAST_ERROR command.
 */
#define XFL_INTEL_SR_WSM_READY			0x80
#define XFL_INTEL_SR_ERASE_SUSPENDED		0x40
#define XFL_INTEL_SR_ERASE_OR_UNLOCK_ERROR	0x20
#define XFL_INTEL_SR_PROG_OR_LOCK_ERROR		0x10
#define XFL_INTEL_SR_VOLTAGE_ERROR		0x08
#define XFL_INTEL_SR_WRITE_SUSPENDED		0x04
#define XFL_INTEL_SR_BLOCK_LOCKED_ERROR		0x02

/*****************************************************************************
*
* Extended capabilities list
* These bits can be read with the XFL_INTEL_DEVCTL_GET_SUPPORT1 command.
*
* XFL_INTEL_SUPPORT1_CHIP_ERASE - The part supports the chip erase command.
*
* XFL_INTEL_SUPPORT1_SUSPEND_ERASE - The part supports suspension and
*   resumption of an erase operation.
*
* XFL_INTEL_SUPPORT1_SUSPEND_PROG - The part supports suspension and
*   resumption of a programming operation.
*
* XFL_INTEL_SUPPORT1_LEGACY_LOCK - The part supports legacy lock/unlock.
*
* XFL_INTEL_SUPPORT1_QUEUED_ERASE - The part supports the queuing up of
*   erase blocks.
*
* XFL_INTEL_SUPPORT1_INSTANT_LOCK - The part supports instant individual
*   block locking.
*
* XFL_INTEL_SUPPORT1_PROTECTION_BIT - The part supports protection bits.
*
* XFL_INTEL_SUPPORT1_PAGE_MODE_READ - The part supports page-mode reads.
*
* XFL_INTEL_SUPPORT1_SYNC_READ - The part supports synchronous reads.
*
*****************************************************************************/
#define XFL_INTEL_SUPPORT1_CHIP_ERASE		0x80000000
#define XFL_INTEL_SUPPORT1_SUSPEND_ERASE	0x40000000
#define XFL_INTEL_SUPPORT1_SUSPEND_PROG		0x20000000
#define XFL_INTEL_SUPPORT1_LEGACY_LOCK		0x10000000
#define XFL_INTEL_SUPPORT1_QUEUED_ERASE		0x08000000
#define XFL_INTEL_SUPPORT1_INSTANT_LOCK		0x04000000
#define XFL_INTEL_SUPPORT1_PROTECTION_BIT	0x02000000
#define XFL_INTEL_SUPPORT1_PAGE_MODE_READ	0x01000000
#define XFL_INTEL_SUPPORT1_SYNC_READ		0x00800000

/*
 * Suspension capabilities list.
 *
 * XFL_INTEL_PROG_AFTER_ERASE_SUSPEND - The part supports programming
 * after suspending an erase operation.
 */
#define XFL_INTEL_SUSPEND_SUPPORT_PROG_AFTER_ERASE	0x00000001

/*
 * Commands written to the devices are defined by the CMD_* constants below.
 * Each Command contains 8-bits of significant data. For x16 or greater devices,
 * the command data should appear on the LSB. Other bytes may be written as
 * don't cares. To eliminate the need to know the bus layout, the width of
 * individual parts, or potential byte-swapping requirements, the CMD_*
 * constants are defined with the command data in every nibble.
 */

/*
 * BCS/SCS command codes.
 */
#define XFL_INTEL_CMD_READ_ARRAY		0xFFFFFFFF
#define XFL_INTEL_CMD_READ_ID_CODES		0x90909090
#define XFL_INTEL_CMD_READ_STATUS_REG		0x70707070
#define XFL_INTEL_CMD_CLEAR_STATUS_REG		0x50505050
#define XFL_INTEL_CMD_WRITE_BUFFER		0xE8E8E8E8
#define XFL_INTEL_CMD_PROGRAM			0x40404040
#define XFL_INTEL_CMD_BLOCK_ERASE		0x20202020
#define XFL_INTEL_CMD_CONFIRM			0xD0D0D0D0
#define XFL_INTEL_CMD_SUSPEND			0xB0B0B0B0
#define XFL_INTEL_CMD_RESUME			0xD0D0D0D0
#define XFL_INTEL_CMD_RESET_0xF0		0xF0F0F0F0

#define XFL_INTEL_STATUS_READY			0x00800080

/*
 * SCS command codes.
 */
#define XFL_INTEL_CMD_READ_QUERY		0x98989898
#define XFL_INTEL_CMD_CONFIG			0xB8B8B8B8
#define XFL_INTEL_CMD_LOCK_BLOCK_SET		0x60606060
#define XFL_INTEL_CMD_LOCK_BLOCK_SET_CONFIRM	0x01010101
#define XFL_INTEL_CMD_LOCK_BLOCK_CLEAR		0x60606060
#define XFL_INTEL_CMD_LOCK_BLOCK_CLEAR_CONFIRM	0xD0D0D0D0
#define XFL_INTEL_CMD_CONFIG_REG_SETUP		0x60606060
#define XFL_INTEL_CMD_CONFIG_REG_CONFIRM	0x03030303

/*
 * Other command codes.
 */
#define XFL_INTEL_CMD_PROTECTION		0xC0C0C0C0

/*
 * Configuration command codes.
 */
#define XFL_INTEL_CONFIG_RYBY_LEVEL		0x00000000
#define XFL_INTEL_CONFIG_RYBY_PULSE_ERASE	0x01010101
#define XFL_INTEL_CONFIG_RYBY_PULSE_WRITE	0x02020202
#define XFL_INTEL_CONFIG_RYBY_PULSE_ALL		0x03030303

/*
 * Configuration register value/address for setting sync and async mode of the
 * platform flash.
 * To put the memory in Asynchronous Read Mode, the CR15 bit of Configuration
 * Register is set to 1. Rest of the Configuration Register Bits are set to
 * default values. The Synchronous Read Mode is set by default with CR15 = 0.
 * The commands to set Configuration register are "Set Configuration Register
 * SetUP" (BASE_ADDR + CONFIG_REG_ASYNC_ADDR -> 0x0060) and "Set Configuration
 * Register Confirm" (BASE_ADDR + CONFIG_REG_ASYNC_ADDR -> 0x0003).
 * The Platform flash is 16 bit flash so the flash controller drop off the least
 * significant bit of the address bus. So the Configuration register value seen
 * at the flash device should be left shifted by 1.
 */
#define XFL_INTEL_CMD_CONFIG_REG_ASYNC_ADDR	0x17BBE
#define XFL_INTEL_CMD_CONFIG_REG_SYNC_ADDR	0x07BBE

/*
 * The Platform flash Manufacture ID and Device ID.
 */
#define XFL_INTEL_PLATFORM_MANUFACTURE_ID	0x0049
#define XFL_INTEL_PLATFORM_DEVICE_ID		0x506B

/**************************** Type Definitions *******************************/

typedef struct {
	XFlashGeometry Geometry;	/* Part geometry */
	XFlashProperties Properties;	/* Part timing, programming and
					 * identification properties */
	XFlashVendorData VendorData;	/* Part specific data */
} XFlashIntel_Device;

/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/

int XFlashIntel_Initialize(XFlash * InstancePtr);
int XFlashIntel_Reset(XFlash * InstancePtr);

int XFlashIntel_DeviceControl(XFlash * InstancePtr, u32 Command,
			      DeviceCtrlParam *Parameters);

int XFlashIntel_Read(XFlash * InstancePtr, u32 Offset, u32 Bytes,
		     void *DestPtr);

int XFlashIntel_Write(XFlash * InstancePtr, u32 Offset, u32 Bytes,
		      void *SrcPtr);

int XFlashIntel_Erase(XFlash * InstancePtr, u32 Offset, u32 Bytes);

int XFlashIntel_Lock(XFlash * InstancePtr, u32 Offset, u32 Bytes);
int XFlashIntel_Unlock(XFlash * InstancePtr, u32 Offset, u32 Bytes);
int XFlashIntel_GetStatus(XFlash * InstancePtr, u32 Offset);

#endif /* XPAR_XFL_DEVICE_FAMILY_INTEL */

#ifdef __cplusplus
}
#endif

#endif /* end of protection macro */
