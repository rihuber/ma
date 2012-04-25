/*******************************************************************************
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
*     (c) Copyright 2007 Xilinx, Inc.
*     All rights reserved.
*
******************************************************************************/

#ifndef LCD_H
#define LCD_H

//=====================
//	TEST FUNCTIONS
//=====================
void LCDTest();
void LCDTestMenu();
void LCDTestInput(char ch);


//=====================
//	EXTERNAL FUNCTIONS
//=====================
void LCDOn();
void LCDOff();
void LCDClear();
void LCDInit();

void MoveCursorHome();
void MoveCursorRight();
void MoveCursorLeft();

void LCDSetLine(int line);
void LCDPrintChar(char c);
void LCDPrintString(char * line1, char * line2);


//=====================
//	INTERNAL FUNCTIONS
//=====================
void InitInst(void);
void WriteInst(unsigned long inst1, unsigned long inst2);
void WriteData(unsigned long data1, unsigned long data2);

#endif
