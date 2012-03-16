/**************************************************************
 * (c) Copyright 2005 - 2009 Xilinx, Inc. All rights reserved.
 *
 * This file contains confidential and proprietary information
 * of Xilinx, Inc. and is protected under U.S. and
 * international copyright and other intellectual property
 * laws.
 *
 * DISCLAIMER
 * This disclaimer is not a license and does not grant any
 * rights to the materials distributed herewith. Except as
 * otherwise provided in a valid license issued to you by
 * Xilinx, and to the maximum extent permitted by applicable
 * law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
 * WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
 * AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
 * BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
 * INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
 * (2) Xilinx shall not be liable (whether in contract or tort,
 * including negligence, or under any other theory of
 * liability) for any loss or damage of any kind or nature
 * related to, arising under or in connection with these
 * materials, including for any direct, or any indirect,
 * special, incidental, or consequential loss or damage
 * (including loss of data, profits, goodwill, or any type of
 * loss or damage suffered as a result of any action brought
 * by a third party) even if such damage or loss was
 * reasonably foreseeable or Xilinx had been advised of the
 * possibility of the same.
 *
 * CRITICAL APPLICATIONS
 * Xilinx products are not designed or intended to be fail-
 * safe, or for use in any application requiring fail-safe
 * performance, such as life-support or safety devices or
 * systems, Class III medical devices, nuclear facilities,
 * applications related to the deployment of airbags, or any
 * other applications that could lead to death, personal
 * injury, or severe property or environmental damage
 * (individually and collectively, "Critical
 * Applications"). Customer assumes the sole risk and
 * liability of any use of Xilinx products in Critical
 * Applications, subject only to applicable laws and
 * regulations governing limitations on product liability.
 *
 * THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
 * PART OF THIS FILE AT ALL TIMES. 
 *************************************************************/
/**
 *
 * @file xlltemac_porting_guide.h
 *
 * This is a guide on how to move from using the temac driver for the plb_temac
 * core to the lltemac driver for the xps_ll_temac core.
 *
 * <pre>
 * MODIFICATION HISTORY:
 *
 * Ver   Who  Date     Changes
 * ----- ---- -------- -------------------------------------------------------
 * 1.00a jvb  10/04/07 First release
 * 
 * </pre>
 *
 * <h2>Overview</h2>
 *
 * The xps_ll_temac core is a bit different from previous ethernet IP
 * provided in the EDK. The xps_ll_temac core provides no direct bus
 * attachment, but rather expects data to come through a local link
 * connection. An additional core needs to be connected this local link
 * interface along with a connection to a bus that can be accessed by
 * the processor.
 *
 * The EDK provides various such cores, xps_ll_fifo, and mpmc/sdma. The
 * xps_ll_fifo core provides a fifo interface to the plbv46 bus and
 * transfers data to the xps_ll_temac's local link interface. The dma
 * cores transfer data directly out of memory into the local link
 * interface.
 *
 * Because fifo and dma functionality is not included directly in the
 * xps_ll_temac, the lltemac driver also does not have any routines to
 * transfer data. Instead the lltemac driver relies upon a second driver
 * that can control, for example, the xps_ll_fifo or dma cores.
 *
 * This guide will lay out the steps for porting your code from using the
 * temac driver to the lltemac driver.
 *
 * There are 2 primary differences between the temac driver and the
 * lltemac driver.
 * 	-# The Fifo and Dma functionality is outside of the lltemac driver.
 * 	-# Interrupt handling is performed outside of the lltemac driver.
 *
 * <h2>Initialization</h2>
 *
 * What used to be a single main operation to initialize the plb_temac
 * driver, is now broken out into multiple steps. For a system with the
 * xps_ll_temac core, the lltemac driver along with the driver for the
 * core attached to the xps_ll_temac's local link interface must both be
 * initialized.
 *
 * Here is an example of how a plb_temac instance might be initialized:
 * <pre>
 *	XTemac_Config *TemacCfg;
 *	XTemac TemacInst;
 *
 *	TemacCfg = XTemac_LookupConfig(XTEMAC_DEVICE_ID);
 *	XTemac_CfgInitialize(&TemacInst, TemacCfg, TemacCfg->BaseAddress);
 * </pre>
 *
 * Here is an example of the lltemac method, utilizing the llfifo driver:
 * <pre>
 *	XLlTemac_Config *TemacCfg;
 *	XLlTemac TemacInst;
 *	XLlFifo FifoInst;
 *
 *	TemacCfg = XLltemac_LookupConfig(XTEMAC_DEVICE_ID);
 *	XLlTemac_CfgInitialize(&TemacInst, TemacCfg, TemacCfg->BaseAddress);
 *
 *	if (XLlTemac_IsDma(&TemacInst)) {
 *		XLlDma_Initialize(&FifoInst, TemacCfg->LLDevBaseAddress);
 *	} else {
 *		XLlFifo_Initialize(&FifoInst, TemacCfg->LLDevBaseAddress);
 * 	}
 * </pre>
 *
 * <h2>Interrupt Handlers</h2>
 * The temac driver handled the temac interrupts and provided a way to
 * register callback routines using the XTemac_SetHandler() routine. The
 * lltemac driver does not handle interrupts. Instead the application or
 * O/S driver must register its own interrupt handlers.
 *
 * Note that for data transfers, the application or O/S driver will want to
 * handle the interrupts for the xps_ll_fifo or mpmc/sdma core and not
 * use the tx or rx completion interrupts in the xps_ll_temac core. The
 * xps_ll_temac rx and tx completion interrupts will trigger transfer
 * completions at the wrong time, either before data has been
 * transferred to the xps_ll_fifo or mpmc/smda core, or too late, after
 * data has been transmitted out of the xps_ll_temac core when the
 * driver only cares that the data is out of the xps_ll_fifo or
 * mpmc/sdma core.
 *
 * The only interrupts from the xps_ll_temac core the application or O/S
 * driver may care about are the fifo overflow, or rx reject interrupts.
 * These are informational interrupts and no action is necessary to recover.
 *
 * Here is an example of how the application or O/S code might register a
 * callback routine for dma receive:
 * <pre>
 *	void DmaRecvHandler(void *ref) { ... }
 *
 *	XTemac_SetHandler(&TemacInst, XTE_HANDLER_SGRECV, DmaRecvHandler, ref);
 * </pre>
 *
 * For the lltemac driver the handler must be registered with the O/S or
 * with the Xilinx interrupt driver, xintc:
 * <pre>
 *	void DmaRecvHandler(XLlDma_BdRing *RxRingPtr) { ... }
 *
 *	// EDK puts local link attached core interrupt id into lltemac config
 *	// Call the xintc driver to set up the interrupt handler.
 *	XIntc_Connect(&IntcInst, TemacCfg->LLDmaRxIntr, (XinterruptHandler) DmaRecvHandler, RxRingPtr);
 * </pre>
 *
 * <h2>Transferring data using Fifo mode</h2>
 *
 * The plb_temac's fifo mode supported filling the fifos with multiple
 * packets before transmitting data. The xps_ll_fifo, that comes with
 * the EDK version 9.2 or later, does not support this feature. If your existing
 * code uses plb_temac to stuff the fifos, then your algorithm for
 * writing to the fifos may have to be modified. Your existing code may
 * transfer one frame at a time. If this is the case, then your
 * algorithm will be fine, and just the routine calls will need to be
 * modified.
 *
 * Note that the xps_ll_fifo core has internal buffer sizes fixed at 2K
 * bytes, which prevents the use of jumbo frames in ethernet.
 *
 * Here is an example of how the temac driver might be used to write
 * packets using fifo mode:
 * <pre>
 *	// packet data in the variable, char *Frames[];
 *	// number of bytes in the frame in, int FrameSizes[];
 *	// number of frames in, int framesToSend;
 *
 * 	ramesWritten = 0;
 *	for (index = 0; index < framesToSend &&
 *			XTemac_FifoGetFreeBytes(&TemacInst, XTE_SEND) > FrameSizes[i];
 * 			index++) {
 *		XTemac_FifoWrite(&TemacInst, Frame, FrameSizes[i], XTE_END+_OF_PACKET);
 *		FramesWritten++;
 *	}
 *
 *	for (index = 0; index < FramesWritten; index++) {
 *		XTemac_FifoSend(&TemacInst, FrameSizes[i]);
 *	}
 *
 *	// Poll for completion XTemac_FifoQuerySendStatus(), or rely on interrupt handler
 * </pre>
 *
 * For the llfifo driver the following would be used instead:
 * <pre>
 *	// packet data in the variable, char *Frames[];
 *	// number of bytes in the frame in, int FrameSizes[];
 *	// number of frames in, int framesToSend;
 *
 *	for (index = 0; index < framesToSend; index++) {
 *		XLlFifo_Write(&FifoInst, Frames[i], FrameSizes[i]);
 *		XLlFifo_TxSetLen(&FifoInst, FrameSizes[i]);
 *
 *		// Poll for completion XllFifo_Status(), or rely on interrupt handler
 *	}
 * </pre>
 *
 *
 * <h2>Transferring data using Dma mode</h2>
 *
 * The new dma driver for the MPMC/SDMA soft core works nearly the
 * same as the driver use in the plb_temac. A few differences to note
 * are:
 * 	-# The lldma driver operates on a ring data structure instead of
 * 	   using a Direction parameter on the routines.
 * 	-# The lldma driver supports buffer descriptors residing in cache.
 * 	   When the buffer descriptors reside in cache, the macros
 * 	   <code>XCACHE_INVALIDATE_DCACHE_RANGE</code>, and
 * 	   <code>XCACHE_FLUSH_DCACHE_RANGE</code> must be defined. Both of
 * 	   these macros take two parameters: a processor address and a length.
 * 	   Note that the O/S drivers provided by Xilinx for VxWorks and Linux
 * 	   already provide these macros in the xenv.h file.
 *
 * See the lltemac and temac driver examples for more details.
 *
 * <h2>Routine Equivalence</h2>
 *
 * Just about everything that could be done with the temac driver can be
 * done in some way using a combination of drivers: lltemac, llfifo,
 * lldma.
 *
 * The following is a list of routines in the temac driver and and the
 * routines that would be used out of the lltemac, llfifo, lldma set
 * instead.
 *
 * <table border=1 cellspacing=0 cellpadding=4>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><B>plb_temac routine</B></small>
 * 		</td>
 * 		<td>
 * 			<small><B>replacement routine</B></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_CfgInitialize(Inst, Config, Virt)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLltemac_CfgInitialize(Inst, Config, Virt)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_FifoGetFreeBytes(Inst, Direction)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLlfifo_RxOccupancy(Inst) or XLlfifo_TxVacancy(Inst)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_FifoQueryRecvStatus(Inst)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLlfifo_Status(Inst)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_FifoQuerySendStatus(Inst, StatusPtr)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>*StatusPtr = XLlfifo_Status(Inst)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_FifoRead(Inst, Buf, Bytes, Eop)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLlfifo_Read(Inst, Buf, Bytes)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_FifoRecv(Inst, BytesPtr)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>*BytesPtr = XLlfifo_RxGetLen(Inst)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_FifoSend(Inst, Bytes)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLlfifo_TxSetLen(Inst, Bytes)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_FifoWrite(Inst, Buf, Bytes, Eop)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLlfifo_Write(Inst, Buf, Bytes)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_IntrFifoDisable(Inst, Direction)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLlfifo_IntDisable(Inst, Mask)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_IntrFifoEnable(Inst, Direction)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLlfifo_IntEnable(Inst, Mask)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_IntrSgCoalGet(Inst, Direction, ThreshPtr, TimerPtr)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLldma_BdRingGetCoalesce(RingPtr, ThreshPtr, TimerPtr)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_IntrSgCoalSet(Inst, Direction, Thresh, Timer)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLldma_BdRingSetCoalesce(RingPtr, Thresh, Timer)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_IntrSgDisable(Inst, Direction)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLldma_BdTringIntDisable(RingPtr, Mask)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_IntrSgEnable(Inst, Direction)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLldma_BdRingIntEnable(RingPtr, Mask)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_LookupConfig(id)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLltemac_LookupConfig(id)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_GetPhysicalInterface(Inst)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLltemac_GetPhysicalInterface(Inst)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_IsFifo(Inst)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLltemac_IsFifo(Inst)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_IsRecvFrame(Inst)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLlfifo_RxOccupancy(Inst)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_IsRecvFrameDropoped(Inst)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLltemac_isRecvFrameDropped(Inst)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_IsRxCsum(Inst)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLltemac_IsRxCsum(Inst)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_IsRxDre(Inst)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><i>N/A</i></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_IsSgDma(Inst)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLltemac_IsDma(Inst)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_IsStarted(Inst)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLltemac_IsStarted(Inst)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_IsTxCsum(Inst)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLltemac_IsTxCsum(Inst)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_SgSendBdCsumEnable(BdPtr)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLldma_BdWrite((BdPtr), XLldma_BD_STSCTRL_USR0_OFFSET (XLldma_BdRead((BdPtr), XLldma_BD_STSCTRL_USR0_OFFSET)) | 1 )</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_SgSendBdCsumDisable(BdPtr)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLldma_BdWrite((BdPtr), XLldma_BD_STSCTRL_USR0_OFFSET (XLldma_BdRead((BdPtr), XLldma_BD_STSCTRL_USR0_OFFSET)) &amp; 0xFFFFFFFE)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_SgSendBdCsumSetup(BdPtr, Start, Insert)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLldma_BdWrite((BdPtr), XLldma_BD_USR1_OFFSET, (Start) &lt;&lt; 16 | (Insert))</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_SgSendBdCsumSeed(BdPtr, Seed)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLldma_BdWrite((BdPtr), XLldma_BD_USR2_OFFSET, Seed)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_SgRecvBdCsumGet(BdPtr)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLldma_BdRead((BdPtr), XLldma_BD_USR3_OFFSET)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_SgRecvBdNext(Inst, BdPtr)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLldma_BdRingNext(RingPtr, BdPtr)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_SgRecvBdPrev(Inst, BdPtr)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLldma_BdRingPrev(RingPtr, BdPtr)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_SgSendBdNext(Inst, BdPtr)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLldma_BdRingNext(RingPtr, BdPtr)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_SgSendBdPrev(Inst, BdPtr)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLldma_BdRingPrev(RingPtr, BdPtr)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_Reset(Inst)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLltemac_Reset(Inst)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_SelfTest(Inst)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><i>N/A</i></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_SetHandler(...)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><i>N/A</i></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_SetOptions(Inst, Options)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLltemac_SetOptions(Inst, Options)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_SgAlloc(Inst, Direction, NumBd, BdPtr)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLldma_BdRingAlloc(RingPtr, NumBd, BdPtr)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_SgCheck(Inst, Direction)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLldma_BdRingCheck(RingPtr)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_SgCommit(Inst, Direction, NumBd, BdPtr)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLldma_BdRingToHw(RingPtr, NumBd, BdPtr)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_SgFree(Inst, Direction, NumBd, BdPtr)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLldma_BdRingFree(RingPtr, NumBd, BdPtr)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_SgGetProcessed(Inst, Direction, NumBd, BdPtr)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLldma_BdRingFromHw(RingPtr, NumBd, BdPtr)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_SgSetSpace(Inst, Direction, PhysAddr, VirtAddr, Alignment, BdCount, BdTemplate)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLldma_BdRingCreate(RingPtr, PhysAddr, VirtAddr, Alignment, BdCount), XLldma_BdRingClone(RingPtr, BdTemplate)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_SgUnAlloc(Inst, Direction, NumBd, BdPtr)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLldma_BdRingUnAlloc(RingPtr, NumBd, BdPtr)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_Start(Inst)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLltemac_Start(Inst)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_Stop(Inst)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLltemac_Stop(Inst)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_SetOptions(Inst, Options)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLltemac_SetOptions(Inst, Options)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_ClearOptions(Inst, Options)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLltemac_ClearOptions(Inst, Options)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_GetOptions(Inst)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLltemac_GetOptions(Inst)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_SetMacAddress(Inst, MacAddressPtr)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLltemac_SetMacAddress(Inst, MacAddressPtr)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_GetMacAddress(Inst, MacAddressPtr)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLltemac_GetMacAddress(Inst, MacAddressPtr)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_SetMaPauseAddress(Inst, AddressPtr)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLltemac_SetMaPauseAddress(Inst, AddressPtr)lltemac</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_GetMaPauseAddress(Inst, AddressPtr)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLltemac_GetMaPauseAddress(Inst, AddressPtr)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_SendPausePacket(Inst, PauseValue)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLltemac_SendPausePacket(Inst, PauseValue)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_GetSgmiiStatus(Inst, SpeedPtr)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code> XLltemac_GetSgmiiStatus(Inst, SpeedPtr)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_GetRgmiiStatus(Inst, SpeedPtr, FullDuplexPtr, LinkupPtr)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLltemac_GetRgmiiStatus(Inst, SpeedPtr, FullDuplexPtr, LinkupPtr)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_GetOperatingSpeed(Inst)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLltemac_GetOperatingSpeed(Inst)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_SetOperatingSpeed(Inst, speed)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLltemac_SetOperatingSpeed(Inst, speed)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_PhySetMdioDivisor(Inst, Divisor)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLltemac_PhySetMdioDivisor(Inst, Divisor)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_PhyRead(Inst, PhyAddr, RegNum, DataPtr)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLltemac_PhyRead(Inst, PhyAddr, RegNum, DataPtr)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_PhyWrite(Inst, PhyAddr, RegNum, Data)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLltemac_PhyWrite(Inst, PhyAddr, RegNum, Data)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_MulticastAdd(Inst, AddressPtr, Entry)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLltemac_MulticastAdd(Inst, AddressPtr, Entry)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_MulticastGet(Inst, AddressPtr, Entry)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLltemac_MulticastGet(Inst, AddressPtr, Entry)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_MulticastClear(Inst, Entry)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><code>XLltemac_MulticastAdd(Inst, Entry)</code></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_GetSoftStats(Inst, StatsPtr)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><i>N/A</i></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_ClearSoftStats(Inst)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><i>N/A</i></small>
 * 		</td>
 * 	</tr>
 * 	<tr valing=top>
 * 		<td>
 * 			<small><code>XTemac_SelfTest(Inst)</code></small>
 * 		</td>
 * 		<td>
 * 			<small><i>N/A</i></small>
 * 		</td>
 * 	</tr>
 * </table>
 *
 * <h2>References</h2>
 * - Linux 2.6 driver: <<i>EDK_install</i>>/sw/ThirdParty/bsp/linux_2_6_v1_00_b/drivers/lltemac_linux_2_6_v1_00_a/src/xlltemac_main.c
 * - VxWorks 5.4 driver: <<i>EDK_install</i>>/sw/XilinxProcessorIPLib/drivers/lltemac_vxworks5_4_v1_00_a/src/xlltemac_end_adapter.c
 */
