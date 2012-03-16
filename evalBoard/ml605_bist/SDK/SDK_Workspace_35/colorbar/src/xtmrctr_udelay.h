/**************************************************************************
*
*     XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"
*     SOLELY FOR USE IN DEVELOPING PROGRAMS AND SOLUTIONS FOR
*     XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION
*     AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE, APPLICATION
*     OR STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS
*     IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,
*     AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE
*     FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY
*     WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE
*     IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR
*     REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF
*     INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*     FOR A PARTICULAR PURPOSE.
*     
*     (c) Copyright 2008 Xilinx, Inc.
*     All rights reserved.
*
**************************************************************************/

/**************************************************************************
* Filename:     xtmrctr_udelay.h
* 
* Description:    
* Definitions for use with xtmrctr_udelay.c
* 
*
* MODIFICATION HISTORY:
*
* Ver    Who   Date     Changes
* ------ ----- -------- -----------------------------------------------------
* 1.00.a bhill 3/6/08   Initial release
*
*
* $Id: xtmrctr_udelay.h,v 1.1 2007/11/14 22:37:42 bhill Exp $
******************************************************************************/
#ifndef _XTMRCTR_UDELAY_H_
#define _XTMRCTR_UDELAY_H_

/*
 * xenv_standalone.h defines udelay as "nothing" for a Microblaze system
 * xtmrctr_udelay.c will actually implement this function, so the definition
 * must be undone.
 */
#ifdef udelay
#undef udelay
#endif

/************************** Function Prototypes ******************************/
XStatus TmrCtrInit(void);
void    udelay(u32 usec);

#endif
