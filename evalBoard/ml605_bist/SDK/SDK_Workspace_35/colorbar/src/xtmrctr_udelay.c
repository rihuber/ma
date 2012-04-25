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
* Filename:    xtmrctr_udelay.c
*
* Description:
* Implements the function udelay(microseconds) which will pause for a short
* delay.  This is accomplished with an xps_timer peripheral.
*
*
* MODIFICATION HISTORY:
*
* Ver    Who   Date     Changes
* ------ ----- -------- -----------------------------------------------------
* 1.00.a bhill 3/6/08   Initial release
*
*
* $Id: xtmrctr_udelay.c,v 1.1 2007/11/14 22:37:42 bhill Exp $
******************************************************************************/

/***************************** Include Files *********************************/
#include "xparameters.h"
#include "xtmrctr.h"
#include "xtmrctr_udelay.h"

/************************** Constant Definitions *****************************/
/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are only defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define TMRCTR_DEVICE_ID            XPAR_DUAL_TIMER_COUNTER_DEVICE_ID

/*
 * The frequency with which the timer is updated.
 * This is defined under "Software Platform Settings"
 */
#define TIMER_COUNTER_FREQ XPAR_CPU_CORE_CLOCK_FREQ_HZ
/*
 * Nanoseconds per timer tick
 */
#define NS_PER_TICK (10000 / (TIMER_COUNTER_FREQ / 100000))
#define NS_PER_USEC 1000

/*
 * This example only uses the 1st of the 2 timer counters contained in a
 * single timer counter hardware device
 */
#define TIMER_COUNTER_0     0

/************************** Function Prototypes ******************************/
XStatus TmrCtrPolledExample(Xuint16 DeviceId, Xuint8 TmrCtrNumber);

/************************** Variable Definitions *****************************/
static XTmrCtr TimerCounter; /* The instance of the Tmrctr Device */


/*
 * TmrCtrInit:
 * Initialize the timer instance this code uses.
 */
XStatus TmrCtrInit (void)
{
    XStatus Status;

    /*
     * Initialize the timer counter so that it's ready to use,
     * specify the device ID that is generated in xparameters.h
     */
    Status = XTmrCtr_Initialize(&TimerCounter, TMRCTR_DEVICE_ID);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    /*
     * Enable the Autoreload mode of the timer counters.
     */
    XTmrCtr_SetOptions(&TimerCounter, TIMER_COUNTER_0, XTC_AUTO_RELOAD_OPTION);
    return 0;
}

/*
 * udelay:
 * Pause here for a minimum of the specified number of microseconds.
 */
void udelay (u32 usec)
{
    u32 Value1, Value2;

    /*
     * Start the timer counter such that it's incrementing by default
     */
    XTmrCtr_Start(&TimerCounter, TMRCTR_DEVICE_ID);

    /*
     * Get a snapshot of the timer counter value to compare against later
     */
    Value1 = XTmrCtr_GetValue(&TimerCounter, TMRCTR_DEVICE_ID);

    /*
     * Read the value of the timer counter, waiting for the specified number
     * of microseconds to elapse.
     */
    do {
        Value2 = XTmrCtr_GetValue(&TimerCounter, TMRCTR_DEVICE_ID);
    } while ( ((Value2 - Value1) * NS_PER_TICK) <
              (usec * NS_PER_USEC) );

    XTmrCtr_Stop(&TimerCounter, TMRCTR_DEVICE_ID);
}


