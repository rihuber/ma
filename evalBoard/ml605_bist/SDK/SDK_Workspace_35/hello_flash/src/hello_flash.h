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
*       (c) Copyright 2009 Xilinx Inc.
*       All rights reserved.
*
******************************************************************************/

/*****************************************************************************/
/**
* @file hello_flash.h
*
* @note     None.
*
* <pre>
*
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- --------------------------------------------------------
* </pre>
*
******************************************************************************/


#ifndef INTG_H  /* Prevent circular inclusions */
#define INTG_H  /* by using protection macros. */

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/

/*
 * Includes that define system constants.
 */
#include "xparameters.h"

/*
 * Includes that define driver APIs for these we will be using.
 */
#include "xintc.h"
#include "xstatus.h"
//#include "xbasic_types.h"

/*
 * Select the processor underlying.
 */
#ifdef __MICROBLAZE__
#include "mb_interface.h"
#endif

#ifdef __PPC__
#include "xexception_l.h"
#endif

/************************** Constant Definitions *****************************/

#define LOOP_NUM            2            /* Test loop number. */
#define TOTAL_BYTES         32
#define LOOP_TIME_OUT       0x000FFFFF

/*
 * Constants indicating INTC device ID and interrupt ID. They are defined in
 * xparameters.h.
 */

#define INTC_DEVICE_ID  XPAR_XPS_INTC_0_DEVICE_ID


/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/

/************************** Function Prototypes ******************************/

#ifdef __cplusplus
}
#endif

#endif            /* End of protection macro.*/
