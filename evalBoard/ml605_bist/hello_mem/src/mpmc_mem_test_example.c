/******************************************************************************
*
*	XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"
*	AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND
*	SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,
*	OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,
*	APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION
*	THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,
*	AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE
*	FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY
*	WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE
*	IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR
*	REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF
*	INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*	FOR A PARTICULAR PURPOSE.
*
*	(c) Copyright 2008, 2009 Xilinx Inc.
*	All rights reserved.
*
******************************************************************************/
/*****************************************************************************/
/**
* @file mpmc_mem_test_example.c
*
* This file contains a memory test example using the MPMC hardware device.
* This example tests the memory by writing and reading test patterns to the
* specified region of the memory. This example tests all the instances of
* MPMC present in the system. The constant, MAX_INSTANCES, defines maximum
* number of instances this example can test. This constant is set to 6, by
* default. The user can modify this constant, to test as many instances as
* desired.
*
* The various memory tests that are done as a part of this example are :
*  - Write all zeroes, read and verify.
*  - Write all ones, read and verify.
*  - Test for stuck together Row/Column.
*  - Maximum Band Address/Row/Column noise.
*  - Data NOT EQUAL TO Address Test.
*
* @note
*
* The following definitions have to be changed by the user depending on
* his requirements :
*
* MPMC_TEST - This definition provides an option to disable or enable each
* individual memory test. All the memory tests are enabled by default.
*
* MPMC_CACHE_TEST - This definition specifies the combination of
* Instruction Cache and Data Cache that are tested a part of the memory
* tests. The different combinations that can be tested are
* - ICache is enabled and DCache is enabled
* - ICache is disabled and DCache is disabled
* - ICache is enabled and DCache is disabled
* - ICache is disabled and DCache is enabled
* MPMC_CACHE_TEST_0 and MPMC_CACHE_TEST_1 are enabled by default.
*
* MAX_INSTANCES - This definition specifies the maximum number of MPMC instances
* this example can test. The default value of this constant is set to 6.
*
* NUM_ITERATIONS - This definition specifies the number of times the test
* has to be run. If it is set to -1 the tests will run infinitely. The default
* value of this constant is set to 1.
*
* PPC_CACHABLE_MEM_REGIONS - This definition specifies the memory regions to
* be marked as cacheable for a PPC processor. The user needs to modify its value
* based on the address and size of the MPMC memory in the system.
* Refer to xcache_l.c for more information.
*
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- --------------------------------------------------------
* 1.00a cc   09/10/08 First release.
* </pre>
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "xparameters.h"
#include "xuartns550_l.h"
#include "xmpmc.h"

#ifdef __PPC__
#include "xcache_l.h"
#endif /* __PPC__ */

#ifdef __MICROBLAZE__
#include "mb_interface.h"
#endif /* __MICROBLAZE__ */


#if !SIM
#include "stdio.h"
#endif
/************************** Constant Definitions ******************************/

#ifndef UART_BASEADDR
#define UART_BASEADDR XPAR_UARTNS550_0_BASEADDR
#endif
#define UART_CLOCK    XPAR_UARTNS550_0_CLOCK_FREQ_HZ

#if !SIM
#define UART_BAUDRATE 9600                      /* real hardware */
#else
#define UART_BAUDRATE (UART_CLOCK / 16 / 3)     /* simulation */
#endif

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define MPMC_NUM_INSTANCES	XPAR_XMPMC_NUM_INSTANCES

/*
 * Bit masks for the tests that are to be done on the memory.
 */
#define MPMC_TEST_0	0x0001 /* MPMC_TEST_0 - Write all zeros and verify */
#define MPMC_TEST_1	0x0002 /* MPMC_TEST_1 - Write all ones and verify */
#define MPMC_TEST_2	0x0004 /* MPMC_TEST_2 - Test for stuck together row/col
						bits*/
#define MPMC_TEST_3	0x0008 /* MPMC_TEST_3 - MAX BA/ROW/COL NOISE */
#define MPMC_TEST_4	0x0010 /* MPMC_TEST_4 - DATA = !ADDR test */

/*
 * Tests to be done. All the tests are enabled by default.
 */
#define MPMC_TEST	(MPMC_TEST_0 | MPMC_TEST_1 | MPMC_TEST_2 | \
			 MPMC_TEST_3 | MPMC_TEST_4)

/*
 * Cache options for the tests.
 */
#define MPMC_CACHE_TEST_0	0x0001 /* ICache: ON , DCache: ON */
#define MPMC_CACHE_TEST_1	0x0002 /* ICache: OFF, DCache: OFF */
#define MPMC_CACHE_TEST_2	0x0004 /* ICache: ON, DCache: OFF */
#define MPMC_CACHE_TEST_3	0x0008 /* ICache: OFF, DCache: ON */

/*
 * Cache options for the tests to be done.
 * MPMC_CACHE_TEST_0 and MPMC_CACHE_TEST_1 are enabled by default.
 */
#define MPMC_CACHE_TEST		(MPMC_CACHE_TEST_0 | MPMC_CACHE_TEST_1)

#define MAX_INSTANCES		6
/*
 * Specifies how many passes of the test to run, -1 == infinite.
 */
#define NUM_ITERATIONS		1

/*
 * This constant defines the memory regions to be marked as cacheable for a PPC
 * processor. Refer to xcache_l.c for more information.
 */
#define PPC_CACHABLE_MEM_REGIONS	0x80000000

/**************************** Type Definitions ********************************/

/*
 * This type is a word address ptr.
 */
typedef volatile u32 *U32Ptr;

/***************** Macros (Inline Functions) Definitions **********************/

/*
 * Macros to read and write words directly to memory.
 */
#define WR_WORD(ADDR, DATA) (*(U32Ptr)(ADDR) = (DATA))
#define RD_WORD(ADDR, DATA) ((DATA) = *(U32Ptr)(ADDR))

/*
 * Macros to enable/disable caches on different processors.
 */
#ifdef __PPC__

#define ENABLE_ICACHE()  XCache_EnableICache(PPC_CACHABLE_MEM_REGIONS)
#define ENABLE_DCACHE()  XCache_EnableDCache(PPC_CACHABLE_MEM_REGIONS)
#define DISABLE_ICACHE() XCache_DisableICache()
#define DISABLE_DCACHE() XCache_DisableDCache()

#endif /* __PPC__ */

#ifdef __MICROBLAZE__

#define ENABLE_ICACHE()  microblaze_enable_icache()
#define ENABLE_DCACHE()  microblaze_enable_dcache()
#define DISABLE_ICACHE() microblaze_disable_icache()
#define DISABLE_DCACHE() microblaze_disable_dcache()

#endif /* __MICROBLAZE__ */

#define printf xil_printf /* Small foot print printf */

/************************** Function Prototypes *******************************/

static u32 MpmcMemTest(XMpmc *Mpmc);
static int MpmcMemTestExample(u16 DeviceId);

/************************** Variable Definitions ******************************/

static XMpmc Mpmc;			/* Instance of the Mpmc driver */

static u32 PassCount = 1;		/* Current iteration */

/*
 * This array specifies the offset from the Memory Baseaddress, for each MPMC
 * instance, from which the memory tests have to be done. By default these
 * values are set to 0.
 * Offset can be used to run the program out of the memory to test and not
 * clobber our software. Set this value to power of 2 larger than the size of
 * this program.
 * Offset should be set to a value less than half the size of the
 * MPMC memory for 'Maximum Bank Address/Row/Column noise' test (MPMC_TEST_3)
 * and to a value less than the size of the MPMC memory for remaining tests.
 */
const u32 MemOffset[MAX_INSTANCES] = {
	0x00200000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};

/*
 * The following variables are used to read from the MPMC memory, these are
 * defined as global to avoid having large buffers on the stack.
 */
#if (MPMC_TEST & MPMC_TEST_3)
 u32 DataBuffer[32];
#endif /* MPMC_TEST & MPMC_TEST_3 */

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

/******************************************************************************/
/**
*
* Main function to call the MPMC memory test example.
*
* @param	None.
*
* @return	Total number of errors for all iterations.
*
* @note		None.
*
*******************************************************************************/
int main(void)
{
	u16 DeviceId = 0;
	u32 TotalErrors = 0;

   XUartNs550_SetBaud(UART_BASEADDR, UART_CLOCK, UART_BAUDRATE);
   XUartNs550_SetLineControlReg(UART_BASEADDR, XUN_LCR_8_DATA_BITS);

  xil_printf("\n\r********************************************************");
  xil_printf("\n\r********************************************************");
  xil_printf("\n\r**     ML605 - MPMC TEST                              **");
  xil_printf("\n\r********************************************************");
  xil_printf("\n\r********************************************************\r\n");

	while (DeviceId < MPMC_NUM_INSTANCES) {
		TotalErrors += MpmcMemTestExample(DeviceId++);
	}

	if (TotalErrors) {
		printf("\r\n### Program finished with errors ###\r\n");
	} else {
		printf("\r\n### Program finished successfully ###\r\n");
	}

	return_to_loader();

	return TotalErrors;
}

/******************************************************************************/
/**
*
* This function runs the memory tests on a Mpmc instance specified by DeviceId.
*
* @param	DeviceId is device ID of the XMpmc Device, typically
*		XPAR_<MPMC_instance>_DEVICE_ID value from xparameters.h.
*
* @return	Total number of errors for all iterations, for a single Mpmc
*		instance.
*
* @note		None.
*
*******************************************************************************/
int MpmcMemTestExample(u16 DeviceId)
{
	XMpmc_Config *CfgPtr;
	u32 MemError = 0;
	u32 TotalErrors = 0;
	int Status;

	/*
	 * Initialize the MPMC device.
	 */
	CfgPtr = XMpmc_LookupConfig(DeviceId);
	if (CfgPtr == XNULL) {
		return XST_FAILURE;
	}

	Status = XMpmc_CfgInitialize(&Mpmc, CfgPtr, CfgPtr->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * If NUM_ITERATIONS set to -1, loop will be infinite.
	 */
	while (PassCount <= NUM_ITERATIONS || NUM_ITERATIONS == -1) {
		printf("\n\r\nMulti-Port Memory Controller Memory Test");
		printf("\r\nTesting address range 0x%08X-0x%08X.",
			Mpmc.ConfigPtr.MemBaseAddress + MemOffset[DeviceId],
			Mpmc.ConfigPtr.MemHighAddress);
		printf("\r\nIteration %d", PassCount);
		if (NUM_ITERATIONS != -1) {
			printf(" of %d", NUM_ITERATIONS);
		}

#if (MPMC_CACHE_TEST & MPMC_CACHE_TEST_0)
		ENABLE_ICACHE();
		ENABLE_DCACHE();
		printf("\r\nPass A) ICache:  On, DCache:  On");
		MemError = MpmcMemTest(&Mpmc);
		printf("\r\n\t\tNumber of errors in this pass = %d\r\n",
			MemError);
		TotalErrors += MemError;
#endif /* MPMC_CACHE_TEST & MPMC_CACHE_TEST_0 */

#if (MPMC_CACHE_TEST & MPMC_CACHE_TEST_1)
		DISABLE_ICACHE();
		DISABLE_DCACHE();
		printf("\r\nPass B) ICache: Off, DCache: Off");
		MemError = MpmcMemTest(&Mpmc);
		printf("\r\n\t\tNumber of errors in this pass = %d\r\n",
			MemError);
		TotalErrors += MemError;
#endif /* MPMC_CACHE_TEST & MPMC_CACHE_TEST_1 */

#if (MPMC_CACHE_TEST & MPMC_CACHE_TEST_2)
		ENABLE_ICACHE();
		DISABLE_DCACHE();
		printf("\r\nPass C) ICache:  On, DCache: Off");
		MemError = MpmcMemTest(&Mpmc);
		printf("\r\n\tNumber of errors in this pass = %d\r\n",
			MemError);
		TotalErrors += MemError;
#endif /* MPMC_CACHE_TEST & MPMC_CACHE_TEST_2 */

#if (MPMC_CACHE_TEST & MPMC_CACHE_TEST_3)
		DISABLE_ICACHE();
		ENABLE_DCACHE();
		printf("\r\nPass D) ICache: Off, DCache:  On");
		MemError = MpmcMemTest(&Mpmc);
		printf("\r\n\tNumber of errors in this pass = %d\r\n",
			MemError);
		TotalErrors += MemError;
#endif /* MPMC_CACHE_TEST & MPMC_CACHE_TEST_0 */

		printf("\r\nMPMC memory test ");
		printf("iteration #%d ", PassCount);

		if (TotalErrors == 0) {
			printf("has PASSED!");
		} else {
			printf("has FAILED!");
		}
		PassCount++;
	}

	printf("\r\nTotal number of errors for all iterations = %d",
		TotalErrors);

	return TotalErrors;
}

/******************************************************************************/
/**
*
* This function runs the MPMC memory tests for a MPMC instance. It returns back
* 0 if there were no errors, else it returns the number of memory errors.
*
* @param 	Mpmc is a pointer to the instance of MPMC to be worked on.
*
* @return
*		 - 0 - No Errors.
*		 - Greater than 1 indicates the number of errors detected.
*
* @note		None.
*
*******************************************************************************/
static u32 MpmcMemTest(XMpmc *Mpmc)
{
	u32 MyAdr;
	u32 StartAdr;
	u32 MemSize;
	u32 Range;
	u32 Offset;
	u32 Error = 0;
	u32 TotalErrors = 0;
	u32 DataRead;
	u32 WriteAdr;
	int i;
	int j;
#if (MPMC_TEST & MPMC_TEST_3)
	u32 BankAddr;
	u32 Row;
	u32 Col;
	u16 MemWidth;
	u16 StartBit;
	u32 RowAddrMask;
	u32 ColAddrMask;
	u32 BankAddrMask;
#endif /* MPMC_TEST & MPMC_TEST_3 */

	Offset   = MemOffset[Mpmc->ConfigPtr.DeviceId];
	StartAdr = Mpmc->ConfigPtr.MemBaseAddress + Offset;
	MemSize  = Mpmc->ConfigPtr.MemHighAddress -
		   Mpmc->ConfigPtr.MemBaseAddress + 1;
	Range    = MemSize - Offset;


	/*
	 * MPMC_TEST_0 - Write all zeros and verify.
	 */
#if (MPMC_TEST & MPMC_TEST_0)
	if (Offset > MemSize) {
		printf("\n\r\tMemory Offset can't be greater than the size of" \
			"the memory");
		return ++Error;
	}

	printf("\r\n\tTEST0: " \
		"Write all memory to 0x00000000 and check" \
		"\r\n\t\tWriting...");

	/*
	 * Clear the range of memory.
	 */
	for (MyAdr = StartAdr; MyAdr < StartAdr + Range; MyAdr += 4) {
		WR_WORD(MyAdr, 0x00000000);
	}

	printf("\r\n\t\tReading...");
	/*
	 * Read it all back.
	 */
	for (MyAdr = StartAdr; MyAdr < StartAdr + Range; MyAdr += 4) {
		RD_WORD(MyAdr, DataRead);
		if (DataRead != 0x00000000) {
			Error++;
			printf("\r\n\t\tTEST0 - ERROR #%d: Address = 0x%08x, " \
				"Data Expected was 0x00000000, Data Received " \
				"was 0x%08x", Error, MyAdr, DataRead);
		}
	}

	printf("\r\n\tTest Complete Status = ");

	if (Error) {
		printf("FAILURE");
	} else {
		printf("SUCCESS");
	}

	TotalErrors += Error;
#endif /* MPMC_TEST & MPMC_TEST_0 */


	/*
	 * MPMC_TEST_1 - Write all ones and verify.
	 */
#if (MPMC_TEST & MPMC_TEST_1)
	if (Offset > MemSize) {
		printf("\n\r\tMemory Offset can't be greater than the size of" \
			"the memory");
		return ++Error;
	}

	printf("\r\n\tTEST1: " \
		"Write all memory to 0xFFFFFFFF and check" \
		"\r\n\t\tWriting...");

	/*
	 * Clear range of memory to 0xFFFFFFFF.
	 */
	for (MyAdr = StartAdr; MyAdr < StartAdr + Range; MyAdr += 4) {
		WR_WORD(MyAdr, 0xFFFFFFFF);
	}

	printf("\r\n\t\tReading...");

	for (MyAdr = StartAdr; MyAdr < StartAdr + Range; MyAdr += 4) {
	     RD_WORD(MyAdr, DataRead);

		if (DataRead != 0xFFFFFFFF) {
			Error++;
			printf("\r\n\t\tTEST1 - ERROR #%d: Address = 0x%08x, " \
				"Data Expected was 0xFFFFFFFF, Data Received " \
				"was 0x%08x", Error, MyAdr, DataRead);
		}
	}
	printf("\r\n\tTest Complete Status = ");
	if (Error) {
		printf("FAILURE \r\n");
	} else {
		printf("SUCCESS \r\n");
	}

	TotalErrors += Error;
#endif /* MPMC_TEST & MPMC_TEST_1 */

	/*
	 * MPMC_TEST_2 - Test for stuck together row/col bits.
	 */
#if (MPMC_TEST & MPMC_TEST_2)
	if (Offset > MemSize) {
		printf("\n\r\tMemory Offset can't be greater than the size of" \
			"the memory");
		return ++Error;
	}

	printf("\r\n\tTEST2: " \
		"Testing for stuck together bank/row/col bits" \
		"\r\n\t\tClearing memory to zeros...");

	/*
	 * Clear all of memory.
	 */
	for (MyAdr = StartAdr; MyAdr < StartAdr + Range; MyAdr += 4) {
		WR_WORD(MyAdr, 0x00000000);
	}

	printf("\r\n\t\tWriting and Reading...");

	for (i = 1, WriteAdr = StartAdr; WriteAdr < StartAdr + Range;
	     ++i, WriteAdr = StartAdr | (u32) (1 << i)) {

		/*
		 * Write out the value we'll be checking for later.
		 */
		WR_WORD(WriteAdr, 0xFFFFFFFF);

		for (j = 1, MyAdr = StartAdr; MyAdr < StartAdr + Range;
		     ++j, MyAdr = StartAdr | (u32)(1 << j)) {

			/*
			 * Read back a word of data.
			 */
			RD_WORD(MyAdr, DataRead);

			/*
			 * Check to make sure it is zero.
			 */
			if ((DataRead != 0) && (WriteAdr != MyAdr)) {
				Error++;
				printf("\r\n\t\tTEST2 - ERROR #%d:, " \
					"Address = 0x%08x, " \
					"Data Expected was 0x00000000, " \
					"Data Received was 0x%08x",
					Error, MyAdr, DataRead);
			}
		}

		/*
		 * Clean out the old value before pointer update.
		 */
		WR_WORD(WriteAdr, 0x00000000);
	}
	printf("\r\n\tTest Complete Status = ");

	if (Error) {
		printf("FAILURE \r\n");
	} else {
		printf("SUCCESS \r\n");
	}

	TotalErrors += Error;
#endif /* MPMC_TEST & MPMC_TEST_2 */

	/*
	 * MPMC_TEST_3 - MAX BA/ROW/COL NOISE.
	 */
#if (MPMC_TEST & MPMC_TEST_3)
	if (Offset > (MemSize / 2)) {
		printf("\n\r\tMemory Offset can't be greater than half the" \
			"size of the memory");
		return ++Error;
	}

	printf("\r\n\tTEST3: " \
		"Testing for maximum ba/row/col noise");
	printf("\r\n\t\tThis test performs 16 word writes followed by 16 word" \
		" reads");
	printf("\r\n\t\tEach 64 bytes inverts the ba/row/col address" \
		"\r\n\t\tInitializing Memory to 0xA5A5A5A5...");

	/*
	 * Calculate our bit masks for Column, Row and Bank addresses,
	 * we are assuming the memory is a power of 2 and even alignment.
	 */
	MemWidth = Mpmc->ConfigPtr.MemDataWidth;
	StartBit = (MemWidth == 8 ? 0 : MemWidth == 16 ? 1 :
		    MemWidth == 32 ? 2 : MemWidth == 64 ? 3 : 4);
	ColAddrMask = (((1 << Mpmc->ConfigPtr.MemColBits) - 1) << (StartBit)) &
		      (MemSize - 1);
	RowAddrMask = (((1 << Mpmc->ConfigPtr.MemRowBits) - 1) << (StartBit +
		       Mpmc->ConfigPtr.MemColBits)) & (MemSize - 1);
	BankAddrMask = (((1 << Mpmc->ConfigPtr.MemBankBits) - 1) <<
			 (StartBit + Mpmc->ConfigPtr.MemColBits +
			 Mpmc->ConfigPtr.MemRowBits)) & (MemSize - 1);

	/*
	 * Set the range of memory to 0xA5A5A5A5.
	 */
	for (MyAdr = StartAdr; MyAdr < StartAdr + Range; MyAdr += 4) {
		WR_WORD(MyAdr, 0xA5A5A5A5);
	}

	/*
	 * Setup initial values.
	 */
	WriteAdr = StartAdr;
	MyAdr = WriteAdr;

	printf("\r\n\t\tWriting and Reading...");

	/*
	 * Going to ping pong and meet in middle.
	 */
	for (i = 0; i < ((Range - Offset) / 128); i++) {
		BankAddr  = WriteAdr & BankAddrMask;
		Row = WriteAdr & RowAddrMask;
		Col = WriteAdr & ColAddrMask & 0xFFFFFFFC;

		/*
		 * Write out 64 bytes.
		 */
		for (j = 0; j < 8; j++) {
			WR_WORD(WriteAdr, 0xFFFFFFFF);
			WR_WORD(WriteAdr + 4, 0x00000000);
			WriteAdr += 8;
		}

		WriteAdr = MyAdr;

		/*
		 * Read back the 64 bytes.
		 */
		for (j = 0; j < 16; j++) {
			RD_WORD(WriteAdr, DataBuffer[j]);
			WriteAdr += 4;
		}

		WriteAdr = MyAdr;

		/*
		 * Lets compare the results to the expected values now....
		 */
		for (j = 0; j < 16; j += 2) {
			if (DataBuffer[j] != 0xFFFFFFFF) {
				Error++;
				printf("\r\n\t\tTEST3.1 - ERROR #%d: " \
					"Address = 0x%08x, " \
					"Data Expected was 0xFFFFFFFF, " \
					"Data Received was 0x%08x", Error,
					(MyAdr + 4 * j), DataBuffer[j]);
			}

			if (DataBuffer[j + 1] != 0x00000000) {
				Error++;
				printf("\r\n\t\tTEST3.2 - ERROR #%d: " \
					"Address = 0x%08x, " \
					"Data Expected was 0x00000000, " \
					"Data Received was 0x%08x", Error,
					MyAdr + (4 * j) + 4, DataBuffer[j + 1]);
			}
		}

		/*
		 * Calculate the inverse address for DDR SDRAM.
		 */
		BankAddr  = ~BankAddr  & BankAddrMask;
		Row = ~Row & RowAddrMask;
		Col = ~Col & ColAddrMask & 0xFFFFFFFC;
		WriteAdr = (BankAddr | Row | Col) - 60 +
			   Mpmc->ConfigPtr.MemBaseAddress;
		MyAdr = WriteAdr;

		/*
		 * Write out 64 bytes.
		 */
		for (j = 0; j < 8; j++) {
			WR_WORD(WriteAdr, 0xFFFFFFFF);
			WR_WORD(WriteAdr + 4, 0x00000000);
			WriteAdr += 8;
		}

		WriteAdr = MyAdr;

		/*
		 * Read back the 64 bytes.
		 */
		for (j = 16; j < 32; j++) {
			RD_WORD(WriteAdr, DataBuffer[j]);
			WriteAdr += 4;
		}

		/*
		 * Lets compare the results to the expected values now....
		 */
		for (j = 16; j < 32; j += 2) {
			if (DataBuffer[j] != 0xFFFFFFFF) {
				Error++;
				printf("\r\n\t\tTEST3.3 - ERROR #%d: " \
					"Address = 0x%08x, " \
					"Data Expected was 0xFFFFFFFF, " \
					"Data Received was 0x%08x", Error,
					(MyAdr + 4 * (j - 16)), DataBuffer[j]);
			}
			if (DataBuffer[j + 1] != 0x00000000) {
				Error++;
				printf("\r\n\t\tTEST3.4 - ERROR #%d: " \
					"Address = 0x%08x, " \
					"Data Expected was 0xFFFFFFFF, " \
					"Data Received was 0x%08x", Error,
					(MyAdr + (4 * (j - 16)) + 4),
					DataBuffer[j + 1]);
			}
		}

		/*
		 * Calculate the "normal" address for the next cycles.
		 */
		BankAddr  = ~BankAddr  & BankAddrMask;
		Row = ~Row & RowAddrMask;
		Col = ~Col & ColAddrMask & 0xFFFFFFFC;

		/*
		 * Increase by one 64 byte quantum.
		 */
		WriteAdr = (BankAddr | Row | Col) + 64 +
			   Mpmc->ConfigPtr.MemBaseAddress;
	}

	printf("\r\n\tTest Complete Status = ");
	if (Error) {
		printf("FAILURE");
	} else {
		printf("SUCCESS");
	}

	TotalErrors += Error;
#endif /* MPMC_TEST & MPMC_TEST_3 */

	/*
	 * MPMC_TEST_4 - DATA = !ADDR test
	 */
#if (MPMC_TEST & MPMC_TEST_4)
	if (Offset > MemSize) {
		printf("\n\r\tMemory Offset can't be greater than the size of" \
			"the memory");
		return ++Error;
	}

	printf("\r\n\tTEST4: " \
		"Testing for Inverse Data at Address" \
		"\r\n\t\tWriting...");

	for (MyAdr = StartAdr; MyAdr < StartAdr + Range; MyAdr += 4) {
		WR_WORD(MyAdr, ~MyAdr);
	}

	printf("\r\n\t\tReading...");

	/*
	 * Read it all back.
	 */
	for (MyAdr = StartAdr; MyAdr < StartAdr + Range; MyAdr += 4) {
		RD_WORD(MyAdr, DataRead);

		if (DataRead != ~MyAdr) {
		Error++;
		printf("\n\t\t\tTEST4 - ERROR #%d: Address = 0x%08x, " \
			"Data Expected was 0x%08x, Data Received was 0x%08x",
			Error, MyAdr, ~MyAdr, DataRead);
		}
	}

	printf("\r\n\tTest Complete Status = ");
	if (Error) {
		printf("FAILURE \r\n");
	} else {
		printf("SUCCESS \r\n");
	}

	TotalErrors += Error;
#endif /* MPMC_TEST & MPMC_TEST_4 */

	return TotalErrors;
}

