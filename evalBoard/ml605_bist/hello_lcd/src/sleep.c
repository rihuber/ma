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
*       (c) Copyright 2007 Xilinx Inc.
*       All rights reserved.
*
******************************************************************************/

#include "sleep.h"
//#include "xtime_l.h"
#include "xparameters.h"

void nanosleep(unsigned int nanoseconds)
{
  /* not implemented */
}

void usleep(unsigned int useconds)
{

  int i,j;
  for (j=0;j<useconds;j++)
    for (i=0;i<7;i++) asm("nop");


//  XTime tEnd, tCur;

//  XTime_GetTime(&tCur);
//  tEnd = tCur + ((XTime) useconds) * (XPAR_CPU_PPC405_CORE_CLOCK_FREQ_HZ / 1000000);
//  do
//  {
//    XTime_GetTime(&tCur);
//  } while (tCur < tEnd);
}

void sleep(unsigned int seconds)
{

  int i,j;
  for (j=0;j<seconds;j++)
    for (i=0;i<25000000;i++) asm("nop");

//  XTime tEnd, tCur;

//  XTime_GetTime(&tCur);
//  tEnd  = tCur + ((XTime) seconds) * XPAR_CPU_PPC405_CORE_CLOCK_FREQ_HZ;
//  do
//  {
//    XTime_GetTime(&tCur);
//  } while (tCur < tEnd);
}

