
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
*     (c) Copyright 2009 Xilinx, Inc.
*     All rights reserved.
*
**************************************************************************/

/**************************************************************************
* Filename:     hello_gpio.c
*
* Description:
* Application that tests the GPIO through toggling output pins.
*
*
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -------------------------------------------------------
* 1.00  jel  5-15-07  Initial Release
* 1.01  jel  10-16-07 Added Hardware and Simulation Support
*
******************************************************************************/

/***************************** Include Files *********************************/
#include "xparameters.h"
#include "xuartns550_l.h"
#include "xgpio_l.h"
#include "xgpio.h"

#include "stdio.h"


/************************** Constant Definitions *****************************/
#define GPIO_0_BASEADDR XPAR_LEDS_8BIT_BASEADDR
#ifndef UART_BASEADDR
#define UART_BASEADDR XPAR_UARTNS550_0_BASEADDR
#endif
#define UART_CLOCK    XPAR_UARTNS550_0_CLOCK_FREQ_HZ

#define UART_BAUDRATE 9600                      /* real hardware */

/************************** Variable Defintions ******************************/
/* Instance For GPIO */
XGpio GpioOutput;

/************************** Function Prototypes ******************************/
void GpioOutputExample(Xuint16 DeviceId, Xuint32 GpioWidth);

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

int main(void)
{

  XUartNs550_SetBaud(UART_BASEADDR, UART_CLOCK, UART_BAUDRATE);
  XUartNs550_SetLineControlReg(UART_BASEADDR, XUN_LCR_8_DATA_BITS);

  xil_printf("\n\r********************************************************");
  xil_printf("\n\r********************************************************");
  xil_printf("\n\r**     ML605 - LED Test                               **");
  xil_printf("\n\r********************************************************");
  xil_printf("\n\r********************************************************\r\n");
  xil_printf("Watch the LEDs\r\n");

  GpioOutputExample(XPAR_LEDS_8BIT_DEVICE_ID, 8);

  return_to_loader();

  return 0;
}

void GpioOutputExample(Xuint16 DeviceId, Xuint32 GpioWidth)
{
  volatile int Delay;
  Xuint32 LedBit;
  Xuint32 LedLoop;
  XStatus Status;
  int numTimes = 6;

  /*
   * Initialize the GPIO driver so that it's ready to use,
   * specify the device ID that is generated in xparameters.h
   */
  Status = XGpio_Initialize(&GpioOutput, DeviceId);
  if (Status != XST_SUCCESS)
  {
	  #if !SIM
      print("Gpio Instance Didn't Initialize!\r\n");
      #endif
  }

  /*
   * Set the direction for all signals to be outputs
   */
  XGpio_SetDataDirection(&GpioOutput, 1, 0x0);

  /*
   * Set the GPIO outputs to low
   */
  XGpio_DiscreteWrite(&GpioOutput, 1, 0x0);
  while (numTimes > 0)
  {
      for (LedBit = 0x0; LedBit < GpioWidth; LedBit++)
      {

          for (LedLoop = 0; LedLoop < 1; LedLoop++)
          {

            /*
             * Set the GPIO Output to High
             */
            XGpio_DiscreteWrite(&GpioOutput, 1, 1 << LedBit);

            /*
             * Wait a small amount of time so the LED is visible
             */
            for (Delay = 0; Delay < 100000; Delay++);

            /*
             * Clear the GPIO Output
             */
            XGpio_DiscreteClear(&GpioOutput, 1, 1 << LedBit);
          }

      }
      numTimes--;
  }
}
