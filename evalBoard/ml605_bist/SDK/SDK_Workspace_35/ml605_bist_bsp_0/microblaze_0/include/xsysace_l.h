/* $Id: xsysace_l.h,v 1.1.2.1 2009/12/11 05:51:39 svemula Exp $ */
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
* @file xsysace_l.h
*
* Defines identifiers and low-level macros/functions for the XSysAce driver.
* These identifiers include register offsets and bit masks. A high-level driver
* interface is defined in xsysace.h.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00a rpm  06/14/02 work in progress
* 1.01a jvb  02/01/06 Added include of xparameters.h unless EXT_CONFIG is
*                     defined at compile time (external configuration), in
*                     which case it just defaults to 8-bit wide memory.
* 1.11a wgr  03/03/07 converted to new coding style.
* 2.00a ktn  10/22/09 Updated to use the HAL Processor APIs/macros.
*		      XSysAce_mSetCfgAddr and XSysAce_mIsCfgDone macros are
*		      removed. User should use XSysAce_SetCfgAddr and
*		      XSysAce_IsCfgDone functions to acheive the desired
*		      functionality
*		      The macros that have been renamed to remove _m from
*		      the name.
* </pre>
*
******************************************************************************/

#ifndef XSYSACE_L_H		/* prevent circular inclusions */
#define XSYSACE_L_H		/* by using protection macros */

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/

#ifdef EXT_CONFIG
#define XPAR_XSYSACE_MEM_WIDTH 8
#else
#include "xparameters.h"
#endif
#include "xil_types.h"
#include "xil_assert.h"
#include "xil_io.h"

/************************** Constant Definitions *****************************/

/*
 * Constant used to align the register offsets to the proper address. This was
 * used during development to handle both byte-addressable (alignment=1) and
 * word addressable (alignment=4) registers. The #ifndef allows the user to
 * modify this at compile-time.
 */
#ifndef XSA_ADDR_ALIGN
#define XSA_ADDR_ALIGN      1
#endif

/** @name Register Offsets
 * System ACE register offsets
 * @{
 */
#define XSA_BMR_OFFSET	(XSA_ADDR_ALIGN * 0)  /**< Bus mode (BUSMODEREG) */
#define XSA_SR_OFFSET	(XSA_ADDR_ALIGN * 4)  /**< Status (STATUSREG) */
#define XSA_ER_OFFSET	(XSA_ADDR_ALIGN * 8)  /**< Error (ERRORREG) */
#define XSA_CLR_OFFSET	(XSA_ADDR_ALIGN * 12) /**< Config LBA (CFGLBAREG) */
#define XSA_MLR_OFFSET	(XSA_ADDR_ALIGN * 16) /**< MPU LBA (MPULBAREG) */
#define XSA_SCCR_OFFSET	(XSA_ADDR_ALIGN * 20) /**< Sector cnt (SECCNTCMDREG) */
#define XSA_VR_OFFSET	(XSA_ADDR_ALIGN * 22) /**< Version (VERSIONREG) */
#define XSA_CR_OFFSET	(XSA_ADDR_ALIGN * 24) /**< Control (CONTROLREG) */
#define XSA_FSR_OFFSET	(XSA_ADDR_ALIGN * 28) /**< FAT status (FATSTATREG) */
#define XSA_DBR_OFFSET	(XSA_ADDR_ALIGN * 64) /**< Data buffer (DATABUFREG) */
/*@}*/

/*
 * Bus Mode Register masks
 */
#define XSA_BMR_16BIT_MASK      0x0101	/**< 16-bit access to ACE controller */


/** @name Status Values
 * Status Register masks
 * @{
 */
#define XSA_SR_CFGLOCK_MASK	0x00000001  /**< Config port lock status */
#define XSA_SR_MPULOCK_MASK	0x00000002  /**< MPU port lock status */
#define XSA_SR_CFGERROR_MASK	0x00000004  /**< Config port error status */
#define XSA_SR_CFCERROR_MASK	0x00000008  /**< CF error status */
#define XSA_SR_CFDETECT_MASK	0x00000010  /**< CF detect flag */
#define XSA_SR_DATABUFRDY_MASK	0x00000020  /**< Data buffer ready status */
#define XSA_SR_DATABUFMODE_MASK	0x00000040  /**< Data buffer mode status */
#define XSA_SR_CFGDONE_MASK	0x00000080  /**< Configuration done status */
#define XSA_SR_RDYFORCMD_MASK	0x00000100  /**< Ready for CF command */
#define XSA_SR_CFGMODE_MASK	0x00000200  /**< Configuration mode status */
#define XSA_SR_CFGADDR_MASK	0x0000E000  /**< Configuration address  */
#define XSA_SR_CFBSY_MASK	0x00020000  /**< CF busy (BSY bit) */
#define XSA_SR_CFRDY_MASK	0x00040000  /**< CF ready (RDY bit) */
#define XSA_SR_CFDWF_MASK	0x00080000  /**< CF data write fault (DWF bit) */
#define XSA_SR_CFDSC_MASK	0x00100000  /**< CF ready (DSC bit) */
#define XSA_SR_CFDRQ_MASK	0x00200000  /**< CF data request (DRQ) */
#define XSA_SR_CFCORR_MASK	0x00400000  /**< CF correctable error (CORR bit) */
#define XSA_SR_CFERR_MASK	0x00800000  /**< CF error (ERR bit) */
/*@}*/


/** @name Error Values
 * Error Register masks.
 * @{
 */
#define XSA_ER_CARD_RESET	0x00000001  /**< CF card failed to reset */
#define XSA_ER_CARD_READY	0x00000002  /**< CF card failed to ready */
#define XSA_ER_CARD_READ	0x00000004  /**< CF read command failed */
#define XSA_ER_CARD_WRITE	0x00000008  /**< CF write command failed */
#define XSA_ER_SECTOR_READY	0x00000010  /**< CF sector failed to ready */
#define XSA_ER_CFG_ADDR		0x00000020  /**< Cfg address is invalid */
#define XSA_ER_CFG_FAIL		0x00000040  /**< Failed to configure a device */
#define XSA_ER_CFG_READ		0x00000080  /**< Cfg read of CF failed */
#define XSA_ER_CFG_INSTR	0x00000100  /**< Invalid instruction during cfg */
#define XSA_ER_CFG_INIT		0x00000200  /**< CFGINIT pin error - did not
					      * go high within 500ms of start */
#define XSA_ER_RESERVED		0x00000400  /**< reserved */
#define XSA_ER_BAD_BLOCK	0x00000800  /**< CF bad block detected */
#define XSA_ER_UNCORRECTABLE	0x00001000  /**< CF uncorrectable error */
#define XSA_ER_SECTOR_ID	0x00002000  /**< CF sector ID not found */
#define XSA_ER_ABORT		0x00004000  /**< CF command aborted */
#define XSA_ER_GENERAL		0x00008000  /**< CF general error */
/*@}*/


/**
 * Config LBA Register - address mask
 */
#define XSA_CLR_LBA_MASK	0x0FFFFFFF  /* Logical Block Address mask */

/**
 * MPU LBA Register - address mask
 */
#define XSA_MLR_LBA_MASK    0x0FFFFFFF	/* Logical Block Address mask */


/** @name Sector Cound/Command Values
 * Sector Count Command Register masks
 * @{
 */
#define XSA_SCCR_COUNT_MASK     0x00FF	 /**< Sector count mask */
#define XSA_SCCR_RESET_MASK     0x0100	 /**< Reset CF card command */
#define XSA_SCCR_IDENTIFY_MASK  0x0200	 /**< Identify CF card command */
#define XSA_SCCR_READDATA_MASK  0x0300	 /**< Read CF card command */
#define XSA_SCCR_WRITEDATA_MASK 0x0400	 /**< Write CF card command */
#define XSA_SCCR_ABORT_MASK     0x0600	 /**< Abort CF command */
#define XSA_SCCR_CMD_MASK       0x0700	 /**< Command mask */
/*@}*/


/*
 * Version Register masks
 */
#define XSA_VR_BUILD_MASK   0x00FF	/* Revision/build number */
#define XSA_VR_MINOR_MASK   0x0F00	/* Minor version number */
#define XSA_VR_MAJOR_MASK   0xF000	/* Major version number */


/** @name Control Values
 * Control Register masks
 * @{
 */
#define XSA_CR_FORCELOCK_MASK       0x00000001	/**< Force lock request */
#define XSA_CR_LOCKREQ_MASK         0x00000002	/**< MPU lock request */
#define XSA_CR_FORCECFGADDR_MASK    0x00000004	/**< Force CFG address */
#define XSA_CR_FORCECFGMODE_MASK    0x00000008	/**< Force CFG mode */
#define XSA_CR_CFGMODE_MASK         0x00000010	/**< CFG mode */
#define XSA_CR_CFGSTART_MASK        0x00000020	/**< CFG start */
#define XSA_CR_CFGSEL_MASK          0x00000040	/**< CFG select */
#define XSA_CR_CFGRESET_MASK        0x00000080	/**< CFG reset */
#define XSA_CR_DATARDYIRQ_MASK      0x00000100	/**< Enable data ready IRQ */
#define XSA_CR_ERRORIRQ_MASK        0x00000200	/**< Enable error IRQ */
#define XSA_CR_CFGDONEIRQ_MASK      0x00000400	/**< Enable CFG done IRQ */
#define XSA_CR_RESETIRQ_MASK        0x00000800	/**< Reset IRQ line */
#define XSA_CR_CFGPROG_MASK         0x00001000	/**< Inverted CFGPROG pin */
#define XSA_CR_CFGADDR_MASK         0x0000E000	/**< Config address mask */
#define XSA_CR_CFGADDR_SHIFT                13	/**< Config address shift */
/*@}*/


/** @name FAT Status
 *
 * FAT filesystem status masks. The first valid partition of the CF
 * is a FAT partition.
 * @{
 */
#define XSA_FAT_VALID_BOOT_REC  0x0001	/**< Valid master boot record */
#define XSA_FAT_VALID_PART_REC  0x0002	/**< Valid partition boot record */
#define XSA_FAT_12_BOOT_REC     0x0004	/**< FAT12 in master boot rec */
#define XSA_FAT_12_PART_REC     0x0008	/**< FAT12 in parition boot rec */
#define XSA_FAT_16_BOOT_REC     0x0010	/**< FAT16 in master boot rec */
#define XSA_FAT_16_PART_REC     0x0020	/**< FAT16 in partition boot rec */
#define XSA_FAT_12_CALC         0x0040	/**< Calculated FAT12 from clusters */
#define XSA_FAT_16_CALC         0x0080	/**< Calculated FAT16 from clusters */
/*@}*/


#define XSA_DATA_BUFFER_SIZE    32   /**< Size of System ACE data buffer */
#define XSA_CF_SECTOR_SIZE     512   /**< Number of bytes in a CF sector */

/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/

/*****************************************************************************/
/**
*
* Get the contents of the control register.
*
* @param	BaseAddress is the base address of the device.
*
* @return	The 32-bit value of the control register.
*
* @note		C-Style signature:
* 		u32 XSysAce_GetControlReg(u32 BaseAddress);
*
******************************************************************************/
#define XSysAce_GetControlReg(BaseAddress) \
	XSysAce_RegRead32((BaseAddress) + XSA_CR_OFFSET)


/*****************************************************************************/
/**
*
* Set the contents of the control register.
*
* @param	BaseAddress is the base address of the device.
* @param	Data is the 32-bit value to write to the register.
*
* @return	None.
*
* @note		C-Style signature:
* 		void XSysAce_SetControlReg(u32 BaseAddress, u32 Data)
*
******************************************************************************/
#define XSysAce_SetControlReg(BaseAddress, Data) \
	XSysAce_RegWrite32((BaseAddress) + XSA_CR_OFFSET, (Data))


/*****************************************************************************/
/**
*
* Set the contents of the control register to the value specified OR'ed with
* its current contents.
*
* @param	BaseAddress is the base address of the device.
* @param	Data is the 32-bit value to OR with the register.
*
* @return	None.
*
* @note		C-Style signature:
*		void XSysAce_OrControlReg(u32 BaseAddress, u32 Data);
*
******************************************************************************/
#define XSysAce_OrControlReg(BaseAddress, Data) \
	XSysAce_SetControlReg((BaseAddress), \
		XSysAce_GetControlReg(BaseAddress) | (Data))


/*****************************************************************************/
/**
*
* Set the contents of the control register to the value specified AND'ed with
* its current contents.
*
* @param	BaseAddress is the base address of the device.
* @param	Data is the 32-bit value to AND with the register.
*
* @return	None.
*
* @note		C-Style signature:
* 		void XSysAce_AndControlReg(u32 BaseAddress, u32 Data)
*
******************************************************************************/
#define XSysAce_AndControlReg(BaseAddress, Data) \
		XSysAce_SetControlReg((BaseAddress), \
			XSysAce_GetControlReg(BaseAddress) & (Data))


/*****************************************************************************/
/**
*
* Get the contents of the error register.
*
* @param	BaseAddress is the base address of the device.
*
* @return	The 32-bit value of the register.
*
* @note		C-Style signature:
*		u32 XSysAce_GetErrorReg(u32 BaseAddress)
*
******************************************************************************/
#define XSysAce_GetErrorReg(BaseAddress) \
		XSysAce_RegRead32((BaseAddress) + XSA_ER_OFFSET)


/*****************************************************************************/
/**
*
* Get the contents of the status register.
*
* @param	BaseAddress is the base address of the device.
*
* @return	The 32-bit value of the register.
*
* @note		C-Style signature:
*		u32 XSysAce_GetStatusReg(u32 BaseAddress)
*
******************************************************************************/
#define XSysAce_GetStatusReg(BaseAddress) \
	XSysAce_RegRead32((BaseAddress) + XSA_SR_OFFSET)


/*****************************************************************************/
/**
*
* Request then wait for the MPU lock. This is not a forced lock, so we must
* contend with the configuration controller.
*
* @param	BaseAddress is the base address of the device.
*
* @return	None.
*
* @note		C-Style signature:
* 		void XSysAce_WaitForLock(u32 BaseAddress)
*
******************************************************************************/
#define XSysAce_WaitForLock(BaseAddress)				\
{									\
	XSysAce_OrControlReg((BaseAddress), XSA_CR_LOCKREQ_MASK);	\
	while ((XSysAce_GetStatusReg(BaseAddress) & 			\
			XSA_SR_MPULOCK_MASK) == 0); 			\
}

/*****************************************************************************/
/**
*
* Enable ACE controller interrupts.
*
* @param	BaseAddress is the base address of the device.
* @param	Mask is the 32-bit value to write to the control register
*
* @return	None.
*
* @note		C-Style signature:
*		void XSysAce_EnableIntr(u32 BaseAddress, u32 Mask)
*
******************************************************************************/
#define XSysAce_EnableIntr(BaseAddress, Mask) \
	XSysAce_OrControlReg((BaseAddress), (Mask));


/*****************************************************************************/
/**
*
* Disable ACE controller interrupts.
*
* @param	BaseAddress is the base address of the device.
* @param	Mask is the 32-bit value to write to the control register
*
* @return	None.
*
* @note		C-Style signature:
* 		void XSysAce_DisableIntr(u32 BaseAddress, u32 Mask);
*
******************************************************************************/
#define XSysAce_DisableIntr(BaseAddress, Mask) \
	XSysAce_AndControlReg((BaseAddress), ~(Mask));


/*****************************************************************************/
/**
*
* Is the CompactFlash ready for a command?
*
* @param	BaseAddress is the base address of the device.
*
* @return	TRUE if it is ready, FALSE otherwise.
*
* @note		C-Style signature:
*		u32 XSysAce_IsReadyForCmd(u32 BaseAddress)
*
******************************************************************************/
#define XSysAce_IsReadyForCmd(BaseAddress) \
	(XSysAce_GetStatusReg(BaseAddress) & XSA_SR_RDYFORCMD_MASK)


/*****************************************************************************/
/**
*
* Is the ACE controller locked for MPU access?
*
* @param	BaseAddress is the base address of the device.
*
* @return	TRUE if it is locked, FALSE otherwise.
*
* @note		C-Style signature:
*		u32 XSysAce_IsMpuLocked(u32 BaseAddress)
*
******************************************************************************/
#define XSysAce_IsMpuLocked(BaseAddress) \
	(XSysAce_GetStatusReg(BaseAddress) & XSA_SR_MPULOCK_MASK)


/*****************************************************************************/
/**
*
* Have interrupts been enabled by the user? We look for the interrupt reset
* bit to be clear (meaning interrupts are armed, even though none may be
* individually enabled).
*
* @param	BaseAddress is the base address of the device.
*
* @return
*		- TRUE if the interrupt is enabled.
*		- FALSE if the interrupt is not enabled.
*
* @note		C-Style signature:
*		u32 XSysAce_IsIntrEnabled(u32 BaseAddress);
*
******************************************************************************/
#define XSysAce_IsIntrEnabled(BaseAddress) \
	((XSysAce_GetControlReg(BaseAddress) & XSA_CR_RESETIRQ_MASK) == 0)


/************************** Function Prototypes ******************************/

int XSysAce_ReadSector(u32 BaseAddress, u32 SectorId, u8 *BufferPtr);
int XSysAce_WriteSector(u32 BaseAddress, u32 SectorId, u8 *BufferPtr);

/*
 * Utility functions to read and write registers and data buffer
 */
u32 XSysAce_RegRead32(u32 Address);
u16 XSysAce_RegRead16(u32 Address);
void XSysAce_RegWrite32(u32 Address, u32 Data);
void XSysAce_RegWrite16(u32 Address, u16 Data);

int XSysAce_ReadDataBuffer(u32 BaseAddress, u8 *BufferPtr, int NumBytes);
int XSysAce_WriteDataBuffer(u32 BaseAddress, u8 *BufferPtr, int NumBytes);

#ifdef __cplusplus
}
#endif


#endif /* end of protection macro */
