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
// #define FLASH_IMAGE_BASEADDR           0xff800000

// 0x87000000
#define FLASH_IMAGE_BASEADDR     (XPAR_FLASH_MEM0_BASEADDR + 0x00900000)
#define SECTOR_SIZE              0x00020000
#define BOOTLOADER_BASEADDR      (FLASH_IMAGE_BASEADDR + (0 * SECTOR_SIZE))
#define HELLO_UART_BASEADDR      (FLASH_IMAGE_BASEADDR + (1 * SECTOR_SIZE))
#define HELLO_GPIO_BASEADDR      (FLASH_IMAGE_BASEADDR + (2 * SECTOR_SIZE))
#define HELLO_TIMER_BASEADDR     (FLASH_IMAGE_BASEADDR + (3 * SECTOR_SIZE))
#define HELLO_FLASH_BASEADDR     (FLASH_IMAGE_BASEADDR + (4 * SECTOR_SIZE))
#define HELLO_IIC_BASEADDR       (FLASH_IMAGE_BASEADDR + (5 * SECTOR_SIZE))
#define HELLO_EMAC_BASEADDR      (FLASH_IMAGE_BASEADDR + (6 * SECTOR_SIZE))
#define HELLO_SWITCH_BASEADDR    (FLASH_IMAGE_BASEADDR + (7 * SECTOR_SIZE))
#define HELLO_MPMC_BASEADDR      (FLASH_IMAGE_BASEADDR + (8 * SECTOR_SIZE))

