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
/*****************************************************************************/
/**
* @file testfatfs.c
*
* The EDK xilfatfs standalone library is used to access the DOS FAT16
* filesystem on a CompactFlash card.
*
*  Simple test for read/write to Compact Flash
*
*      Tries to read the following files:
*      "a:\\test\\test.txt";
*      "a:\\test\\xflow.log";
*      "a:\\test\\xilfatfs.pdf";
*
*      If CONFIG_WRITE is enabled, additionally writes:
*      "a:\\test\\parse.c";
*
*      If CONFIG_DIR_SUPPORT is enabled, additionally creates:
*      "a:\\test\\talica";
*
* @note
*
* Please see the EDK OS and Libraries Document Collection (oslib_rm.pdf)
* for additional information on standalone library and OS support
*
* <pre>
* </pre>
*
******************************************************************************/

/***************************** Include Files *********************************/
#include <sysace_stdio.h>
#include "xsysace_l.h"
#include "xuartns550_l.h"
#include "xparameters.h"


#if !SIM
#include "stdio.h"
#endif


/************************** Constant Definitions *****************************/
#define CONFIG_WRITE
#define CONFIG_DIR_SUPPORT

#ifndef UART_BASEADDR
#define UART_BASEADDR XPAR_UARTNS550_0_BASEADDR
#endif
#define UART_CLOCK    XPAR_UARTNS550_0_CLOCK_FREQ_HZ

#if !SIM
#define UART_BAUDRATE 9600                      /* real hardware */
#else
#define UART_BAUDRATE (UART_CLOCK / 16 / 3)     /* simulation */
#endif


void read_file(char FileName[], int verbose)
{
        unsigned char readBuffer[512];
        SYSACE_FILE *ptest;
        int i= 0;
        int numread = 0;
        int total_bytes_read = 0;
        char file_data;
        ptest = sysace_fopen(FileName , "r" );

        if(ptest) {
                xil_printf("Reading file : %s\n\r", FileName);
                do {
                        numread = sysace_fread(readBuffer, 1, 512, ptest);

                        total_bytes_read = total_bytes_read + numread;
                        if (verbose) {
                                i = 0;
                                while( i < (numread )) {
                                        file_data = readBuffer[i];
                                        xil_printf("%c", readBuffer[i]);
                                        i++;
                                }
                        }
                } while(numread);
                sysace_fclose (ptest);
        } else {
                xil_printf("Failed to open %s: check if file present\n\r", FileName);
        }

        xil_printf("Total bytes read = %d\n\r", total_bytes_read);
}

#ifdef CONFIG_WRITE
void write_file(char FileName[], int verbose)
{
        unsigned char buf[] = "<This will be the data in the file>\n\r";
        SYSACE_FILE *ptest;
        int n = 0;
        ptest = sysace_fopen(FileName ,"w" );

        if(ptest) {
                print("Writing file contents.\n\r\n\r");
                n = sysace_fwrite(buf, 1, sizeof buf, ptest);
                xil_printf("# of bytes written: %d\n\r", n);
                sysace_fclose(ptest);
        } else {
                print("Failed to open: check if file exists\n\r");
        }
}
#endif

/*
 * Return to the bootloader via it's well-known vector at 0x40
 */
static void
return_to_loader()
{
    void (*loader_start1)();

    loader_start1 = (void*) (*(unsigned*)0x40);
    if (loader_start1) {
        (*loader_start1)();
    }
}

int main()
{
        char file1[] = "a:\\test\\test.txt";
        char file2[] = "a:\\test\\xflow.log";
        char file3[] = "a:\\test\\xilfatfs.pdf";
        char ch;


   XUartNs550_SetBaud(UART_BASEADDR, UART_CLOCK, UART_BAUDRATE);
   XUartNs550_SetLineControlReg(UART_BASEADDR, XUN_LCR_8_DATA_BITS);
  /* Clear UART input buffer */
  while (XUartNs550_GetLineStatusReg(UART_BASEADDR) & 0x1)
    ch = XUartNs550_RecvByte(UART_BASEADDR);

  xil_printf("\n\r********************************************************");
  xil_printf("\n\r********************************************************");
  xil_printf("\n\r**     ML605 - Sysace Test                            **");
  xil_printf("\n\r********************************************************");
  xil_printf("\n\r********************************************************\r\n");

        print("\n\rThis program attempts to access the CF card's file system\n\r");
        print("to perform file I/O operations.\n\r\n\r");
        print("Please insert a CF card with the contents of the directory\n\r");
        print("<ref design install dir>/sw/standalone/testfatfs/required_files/\n\r");
        print("copied into a directory named test on that CF card.\n\r\n\r");
        print("Warning: This program will attempt to create a file and directory\n\r");
        print("on the CF card.\n\r\n\r");

        print("\nFile I/O Test Program running.\n\r");

        XSysAce_RegWrite16(XPAR_SYSACE_COMPACTFLASH_BASEADDR + XSA_BMR_OFFSET, XSA_BMR_16BIT_MASK);
#ifdef NOTNOW
        // Reset the sysace controller to clean any bad state, leave it in MPU mode
	XSysAce_SetControlReg(XPAR_SYSACE_COMPACTFLASH_BASEADDR, XSA_CR_CFGSEL_MASK | XSA_CR_FORCECFGMODE_MASK);
	XSysAce_SetControlReg(XPAR_SYSACE_COMPACTFLASH_BASEADDR, XSA_CR_CFGSEL_MASK | XSA_CR_FORCECFGMODE_MASK | XSA_CR_CFGRESET_MASK);
	XSysAce_SetControlReg(XPAR_SYSACE_COMPACTFLASH_BASEADDR, XSA_CR_CFGSEL_MASK | XSA_CR_FORCECFGMODE_MASK);
#endif

        read_file(file1, 1);
        read_file(file2, 1);
        read_file(file3, 0);
        read_file("a:\\test\\noexist.c", 1);
        print("\n\rread done\n\r");

#ifdef CONFIG_WRITE
        write_file("a:\\test\\parse.c", 0);
        print("write done\n\r");
#endif

#if defined(CONFIG_WRITE) && defined(CONFIG_DIR_SUPPORT)
        print("executing mkdir talica..\n\r");
        if (sysace_mkdir("a:\\test\\talica") < 0)
                print("mkdir failed\n\r");
        else
                print("mkdir success\n\r");
#endif

	return_to_loader();

        return 0;
}
