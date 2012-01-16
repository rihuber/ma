/* $Id: xilflash_properties.h,v 1.2 2008/04/11 10:12:40 svemula Exp $ */
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
*       (c) Copyright 2008 Xilinx Inc.
*       All rights reserved.
*
******************************************************************************/
/*****************************************************************************/
/**
*
* @file xilflash_properties.h
*
* This file contains various data common to flash devices most of which can be
* derived from the CFI query.
*
* @note
*
* There is no implementation file with this component.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00a rmm  10/25/07 First release
* 1.00a mta  10/25/07 Updated to flash library
* 1.01a ksu  04/10/08 Added support for AMD CFI Interface
* </pre>
*
***************************************************************************/

#ifndef XFLASH_PROPERTIES_H	/* prevent circular inclusions */
#define XFLASH_PROPERTIES_H	/* by using protection macros */

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "xbasic_types.h"
#include "xstatus.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/**
 * Flash timing
 */
typedef struct {
	u16 WriteSingle_Us;	/**< Time to program a single word unit
				  *   Units are in microseconds */
	u16 WriteBuffer_Us;	/**< Time to program the contents of the
				  *  write buffer. Units are in microseconds
				  *  If the part does not support write
				  *  buffers, then this value should be
				  *  zero */
	u16 EraseBlock_Ms;	/**< Time to erase a single block
				  *  Units are in milliseconds */
	u16 EraseChip_Ms;	/**< Time to perform a chip erase
				  *  Units are in milliseconds */
} XFlashTiming;

/**
 * Flash identification
 */
typedef struct {
	u8 ManufacturerID;	/**< Manufacturer of parts */
	u8 DeviceID;		/**< Part number of manufacturer */
	u16 CommandSet;		/**< Command algorithm used by part. Choices
				  *  are defined in XFL_CMDSET constants */
} XFlashPartID;

/**
 * Programming parameters
 */
typedef struct {
	u32 WriteBufferSize;	 	/**< Number of bytes that can be
					  * programmed at once */
	u32 WriteBufferAlignmentMask;	/**< Alignment of the write buffer */
	u32 EraseQueueSize;		/**< Number of erase blocks that can be
					  *  queued up at once */
} XFlashProgCap;

/**
 * Consolidated parameters
 */
typedef struct {
	XFlashPartID PartID;		/**< Uniquely identifies the part */
	XFlashTiming TimeTypical;	/**< Typical timing data */
	XFlashTiming TimeMax;		/**< Worst case timing data */
	XFlashProgCap ProgCap;		/**< Programming capabilities */
} XFlashProperties;

/************************** Function Prototypes ******************************/


#ifdef __cplusplus
}
#endif

#endif /* end of protection macro */
