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
*       (c) Copyright 2007, 2009 Xilinx Inc.
*       All rights reserved.
*
******************************************************************************/

#include <stdio.h>
#include "sleep.h"
#include "xgpio_l.h"
#include "xparameters.h"

// usec delay timer during initialization, important to change if
// clock speed changes
#define INIT_DELAY 10000
#define INST_DELAY 1000    //usec delay timer between instructions
#define DATA_DELAY 1000    //usec delay timer between data

/*
#------------------------------------------------------------------------------
# IO Pad Location Constraints / Properties for Character LCD GPIO
#------------------------------------------------------------------------------

NET gpio_char_lcd<6> LOC = AE13; # LCD_E
NET gpio_char_lcd<6> PULLDOWN;
NET gpio_char_lcd<5> LOC = AC17; # LCD_RS
NET gpio_char_lcd<4> LOC = AB17; # LCD_RW
NET gpio_char_lcd<3> LOC = AF12; # LCD_DB7
NET gpio_char_lcd<2> LOC = AE12; # LCD_DB6
NET gpio_char_lcd<1> LOC = AC10; # LCD_DB5
NET gpio_char_lcd<0> LOC = AB10; # LCD_DB4
*/

//==============================================================================
//
//								INTERNAL FUNCTIONS
//
//==============================================================================



void InitInst(void)
{
    XGpio_WriteReg(XPAR_LCD_GPIO_BASEADDR, 1, 0x00000003);
    usleep(1);
    XGpio_WriteReg(XPAR_LCD_GPIO_BASEADDR, 1, 0x00000043); //set enable and data
    usleep(1);
    XGpio_WriteReg(XPAR_LCD_GPIO_BASEADDR, 1, 0x00000003);
    usleep(INIT_DELAY);
}

void WriteInst(unsigned long inst1, unsigned long inst2)
{
    unsigned long printinst;

    printinst = 0x00000040 | inst1;

    XGpio_WriteReg(XPAR_LCD_GPIO_BASEADDR, 1, inst1);      //write data
    usleep(1);
    XGpio_WriteReg(XPAR_LCD_GPIO_BASEADDR, 1, printinst);  //set enable
    usleep(1);
    XGpio_WriteReg(XPAR_LCD_GPIO_BASEADDR, 1, inst1);      //turn off enable
    usleep(1);

    printinst = 0x00000040 | inst2;

    XGpio_WriteReg(XPAR_LCD_GPIO_BASEADDR, 1, printinst);  //set enable and data
    usleep(1);
    XGpio_WriteReg(XPAR_LCD_GPIO_BASEADDR, 1, inst2);      //turn off enable

    usleep(INST_DELAY);
}

void WriteData(unsigned long data1, unsigned long data2)
{
    unsigned long rs_data, enable_rs_data;
    //	int busy=true;

    rs_data = 0x00000020 | data1; //sets rs, data1
    enable_rs_data = 0x00000060 | data1;

    XGpio_WriteReg(XPAR_LCD_GPIO_BASEADDR, 1, rs_data); //write data, rs
    usleep(1);
    XGpio_WriteReg(XPAR_LCD_GPIO_BASEADDR, 1, enable_rs_data); //set enable, keep data, rs
    usleep(1);
    XGpio_WriteReg(XPAR_LCD_GPIO_BASEADDR, 1, rs_data); //turn off enable
    usleep(1);

    rs_data = 0x00000020 | data2; //sets rs, data2
    enable_rs_data = 0x00000060 | data2; //sets rs, data2

    XGpio_WriteReg(XPAR_LCD_GPIO_BASEADDR, 1, enable_rs_data); //set enable, rs, data
    usleep(1);
    XGpio_WriteReg(XPAR_LCD_GPIO_BASEADDR, 1, rs_data); //turn off enable

    usleep(DATA_DELAY);
}

//==================================================================================
//
//								EXTERNAL FUNCTIONS
//
//==================================================================================

void LCDOn()
{
    //printf("DISPLAY ON\r\n");
    WriteInst(0x00000000, 0x0000000E);
}

void LCDOff()
{
    // printf("DISPLAY OFF\r\n");
    WriteInst(0x00000000, 0x00000008);
}

void LCDClear()
{
    // printf("DISPLAY CLEAR\r\n");
    WriteInst(0x00000000, 0x00000001);
    WriteInst(0x00000000, 0x00000010);
}

void LCDInit()
{
    // Sets CHAR LCD Reg to Write Mode
    XGpio_WriteReg(XPAR_LCD_GPIO_BASEADDR, 0x4, 0x00000000);
    // Zeroes CHAR LCD Reg
    XGpio_WriteReg(XPAR_LCD_GPIO_BASEADDR, 1, 0x00000000);

    // LCD INIT
    usleep(15000);  //After VCC>4.5V Wait 15ms to Init Char LCD
    InitInst();
    usleep(4100);   //Wait 4.1ms
    InitInst();
    usleep(100);    //Wait 100us
    InitInst();
    InitInst();

    // Function Set
    WriteInst(0x00000002, 0x00000002);

    // Display Off
    WriteInst(0x00000000, 0x00000008);

    // Display Clear
    WriteInst(0x00000000, 0x00000001);

    // Entry Mode Set
    WriteInst(0x00000000, 0x00000006);

    // Display On
    WriteInst(0x00000000, 0x0000000E);
}

void MoveCursorHome()
{
    WriteInst(0x00000000, 0x00000002);
}

void MoveCursorLeft()
{
    WriteInst(0x00000001, 0x00000000);
}

void MoveCursorRight()
{
    WriteInst(0x00000001, 0x00000004);
}

void LCDSetLine(int line)
{
    //line1 = 1, line2 = 2
    int i;

    if((line - 1))
    {
         MoveCursorHome();
	 for(i=0; i<40; i++)
	 {
	      MoveCursorRight();
	 }
    }
    else
    {
         MoveCursorHome();
    }

}

void LCDPrintChar(char c)
{
    WriteData(((c >> 4) & 0x0000000F), (c & 0x0000000F));
}

void LCDPrintString(char * line1, char * line2)
{
    int i=0;

    LCDSetLine(1);

    for(i=0; i<16; i++)
    {
         if(line1[i])
              LCDPrintChar(line1[i]);
         else
	      break;
    }

    LCDSetLine(2);

    for(i=0; i<16; i++)
    {
         if(line2[i])
              LCDPrintChar(line2[i]);
         else
              break;
    }
    return;
}



//==============================================================================
//
//								TEST FUNCTIONS
//
//==============================================================================

#ifdef IN_XROM
void LCDTest()
{
    char c;
    int done;
    int linePosition; //line1 = 0, line2 = 1

    LCDClear();

    printf("\r\nYou should see your letters appear on the LCD as you type\r\n");
    printf("Press TAB to move from between Line1 and Line2\r\n");
    printf("Prses BACKSPACE to clear the LCD\r\n");
    printf("Press ESC to conclude the test\r\n");

    linePosition = 0;
    done = 0;
    do
    {
         c = InputGetChar();
         switch(c)
	 {
              case 0x9:
                   linePosition = !linePosition;
                   LCDSetLine(linePosition + 1);
                   break;
              case 0x1b:
                   doPrintMenu = true;
                   done = 1;
                   LCDClear();
                   break;
              case 0x8:
                   linePosition = 0;
                   LCDClear();
                   break;
              default:
                   LCDPrintChar(c);
                   break;
         }
    }while(!done);
}

void LCDTestMenu()
{
    LogLn();
    printf("Character LCD Tests\r\n");
    printf("  1. Turn LCD On\r\n");
    printf("  2. Turn LCD Off\r\n");
    printf("  3. Write To LCD\r\n");
    printf("  4. Print Hello World\r\n");
    printf("  0. Return to the Main Menu\r\n");
}

void LCDTestInput(char ch)
{
    switch(ch)
    {
         case '1':
              LCDOn();
              break;
         case '2':
              LCDOff();
              break;
         case '3':
              LCDTest();
              break;
         case '4':
              LCDClear();
              LCDPrintString("Hello", "World");
              break;
         case 0x1b:
         case '0':
              PopMenu(&printMenu, &handleInput);
              doPrintMenu = true;
              break;
         default:
              doPrintMenu = true;
              break;
    }
}
#endif
