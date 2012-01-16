/////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2004 Xilinx, Inc. All Rights Reserved.
//
// You may copy and modify these files for your own internal use solely with
// Xilinx programmable logic devices and  Xilinx EDK system or create IP
// modules solely for Xilinx programmable logic devices and Xilinx EDK system.
// No rights are granted to distribute any files unless they are distributed in
// Xilinx programmable logic devices.
//
/////////////////////////////////////////////////////////////////////////////////

/*
 * Xilinx EDK - Simple SREC Bootloader
 * This simple bootloader is provided with Xilinx EDK for you to easily re-use
 * in your own software project. It is capable of booting an SREC format image
 * file (Mototorola S-record format), given the location of the image in memory.
 * In particular, this bootloader is designed for images stored in non-volatile
 * flash memory that is addressable from the processor. 
 *
 * Please modify the define "FLASH_IMAGE_BASEADDR" in the blconfig.h header
 * file to point to the memory location from which the bootloader has to pick
 * up the flash image from.
 *
 * You can include these sources in your software application project in XPS
 * and build the project for the processor for which you want the bootload to
 * happen. You can also subsequently modify these sources to adapt the
 * bootloader for any specific scenario that you might require it for.
 *
 */

/* ADDITIONS BHILL */
#include "xparameters.h"
#include "xuartns550_l.h"
#define VERBOSE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "blconfig.h"
#include "portab.h"
#include "errors.h"
#include "srec.h"

/* Defines */
#define CR       13

/* Comment the following line, if you want a smaller and faster bootloader
 * which will be silent
 */
#define VERBOSE

/* Declarations */
static void display_progress (uint32_t lines);
static uint8_t load_exec ();
static uint8_t flash_get_srec_line (uint8_t *buf);
extern int srec_line;

#ifdef __cplusplus
extern "C" {
#endif

extern void outbyte(char c); 

#ifdef __cplusplus
}
#endif

/* Data structures */
static srec_info_t srinfo;
static uint8_t sr_buf[SREC_MAX_BYTES];
static uint8_t sr_data_buf[SREC_DATA_MAX_BYTES];

static uint8_t *flbuf;

#ifdef VERBOSE
static int8_t *errors[] = { 
    "Error while copying executable image into RAM",
    "Error while reading an SREC line from flash",
    "SREC line is corrupted",
    "SREC has invalid checksum."
};
#endif

/* We don't use interrupts/exceptions. 
   Dummy definitions to reduce code size on MicroBlaze */
#ifdef __MICROBLAZE__
void _interrupt_handler () {}
void _exception_handler () {}
void _hw_exception_handler () {}
#endif

/* Set a well-known "return to loader" vector at address 0x40 */
static void
set_ret_to_loader_vec ()
{
    unsigned *loader_vec;
    extern unsigned _start1;

    loader_vec = (unsigned*)0x40;
    *loader_vec = (unsigned)&_start1;
}


int main()
{
    uint8_t      ret;
    int          choice, bytes, total_bytes;

    /* Set a well-known "return to loader" vector*/
    set_ret_to_loader_vec();

    /* Initialize RS232_Uart_1 - Set baudrate and number of stop bits */
    XUartNs550_SetBaud(XPAR_RS232_UART_1_BASEADDR, XPAR_XUARTNS550_CLOCK_HZ,
                       9600);
    XUartNs550_SetLineControlReg(XPAR_RS232_UART_1_BASEADDR,
                                  XUN_LCR_8_DATA_BITS);

    print("\n\r********************************************************");
    print("\n\r********************************************************");
    print("\n\r**     Xilinx Virtex-6 FPGA ML605 Evaluation Kit      **");
    print("\n\r********************************************************");
    print("\n\r********************************************************\r\n");

    do {
        print("Choose Feature to Test:\r\n");
        print("1: UART Test\r\n");
        print("2: LED Test\r\n");
        print("3: Timer Test\r\n");
        print("4: FLASH Test\r\n");
        print("5: IIC Test\r\n");
        print("6: Ethernet Loopback Test\r\n");
        print("7: Switch Test\r\n");
        print("8: External Memory Test\r\n");
        print("9: System Monitor Test\r\n");
        print("A: PushButton Test\r\n");
        print("B: LCD Test\r\n");
        print("C: System ACE CF Test\r\n");
        print("D: DVI/VGA Test\r\n");
        choice = inbyte();

        if (isalpha(choice)) {
            choice = toupper(choice);
        }

    } while (!isdigit(choice) && (choice < 'A' || choice > 'D')) ;
    xil_printf("%c\r\n", choice);

	 if (choice < 'A') {
	   choice -= '0';
	 } else {
	   choice -= 'A';
	   choice += 10;
    }
    flbuf = (uint8_t*)(FLASH_IMAGE_BASEADDR + (choice * SECTOR_SIZE));
    /* 
     * Read the SRECORDS in the selected file into RAM.
     */
    xil_printf("Reading SRECORDS from Linear BPI Flash at: 0x%x\r\n", (unsigned)flbuf);

    ret = load_exec ();

    /* If we reach here, we are in error */
    
#ifdef VERBOSE
    if (ret > LD_SREC_LINE_ERROR) {
        print ("ERROR in SREC line: ");
        putnum (srec_line);
        print (errors[ret]);    
    } else {
        print ("ERROR: ");
        print (errors[ret]);
    }
#endif

    return ret;
}

#ifdef VERBOSE
static void display_progress (uint32_t count)
{
    // HACK BHILL
    if (count % 100) 
        return;

    /* Send carriage return */
    outbyte (CR);  
    print  ("Bootloader: Processed (0x)");
    putnum (count);
    print (" S-records");
}
#endif

static uint8_t load_exec ()
{
    uint8_t ret;
    void (*laddr)();
    int8_t done = 0;
    
    srinfo.sr_data = sr_data_buf;
    
    while (!done) {
        if ((ret = flash_get_srec_line (sr_buf)) != 0) 
	    return ret;
        
	if ((ret = decode_srec_line (sr_buf, &srinfo)) != 0)
	    return ret;
        
#ifdef VERBOSE
        display_progress (srec_line);
#endif
	switch (srinfo.type) {
	    case SREC_TYPE_0:
		break;
	    case SREC_TYPE_1:
	    case SREC_TYPE_2:
	    case SREC_TYPE_3:
		memcpy ((void*)srinfo.addr, (void*)srinfo.sr_data, srinfo.dlen);
		break;
	    case SREC_TYPE_5:
		break;
	    case SREC_TYPE_7:
	    case SREC_TYPE_8:
	    case SREC_TYPE_9:
		laddr = (void (*)())srinfo.addr;
		done = 1;
		ret = 0;
		break;
	}
    }

#ifdef VERBOSE
    print ("\r\nExecuting program starting at address: ");
    putnum ((uint32_t)laddr);
    print ("\r\n");
#endif

    (*laddr)();                 
  
    /* We will be dead at this point */
    return 0;
}


static uint8_t flash_get_srec_line (uint8_t *buf)
{
    uint8_t c;
    int8_t count = 0;

    while (1) {
	c  = *flbuf++;
	if (c == 0xD) {   
            /* Eat up the 0xA too */
	    c = *flbuf++; 
	    return 0;
	}
	
	*buf++ = c;
	count++;
	if (count > SREC_MAX_BYTES) 
	    return LD_SREC_LINE_ERROR;
    }
}

#ifdef __PPC__

#include <unistd.h>

/* Save some code and data space on PowerPC 
   by defining a minimal exit */
void exit (int ret)
{
    _exit (ret);
}
#endif
