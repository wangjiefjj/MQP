/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"	/* EDK generated parameters */
#include "xspips.h"		/* SPI device driver */
#include "xgpiops.h"

// Zynq SPI device ID
#define SPI_DEVICE_ID		XPAR_XSPIPS_0_DEVICE_ID
// SPI addresses to read to / write from
#define READ 0x80 /*OR*/
#define WRITE 0x7F /*AND*/
#define CTL1 0x20
#define CTL2 0x21
#define CTL3 0x22
#define STATUS_ADDRESS 0x27
#define DATA_ADDRESS 0x28
#define CASCADE 0xC0

// create a new SPI instance
static XSpiPs SpiInstance;
// create a new GPIO instance
static XGpioPs Gpio;

void delay(int cycles);
void IMU_init();
void getIMUdata();

int xData, yData;

int main()
{
    init_platform();

    // initialize LED9
    XGpioPs_Config * ConfigPtr = XGpioPs_LookupConfig(XPAR_PS7_GPIO_0_DEVICE_ID);
    XGpioPs_CfgInitialize(&Gpio, ConfigPtr, ConfigPtr->BaseAddr);
    XGpioPs_SetDirectionPin(&Gpio, 7, 1);
    XGpioPs_SetDirectionPin(&Gpio, 50, 0);

    int Status;

    // initialize SPI
   	XSpiPs_Config *SpiConfig;
   	SpiConfig = XSpiPs_LookupConfig(SPI_DEVICE_ID);
   	// initialize the spi hardware with the device config
    Status = XSpiPs_CfgInitialize(&SpiInstance, SpiConfig, SpiConfig->BaseAddress);
   	// Set the Spi device as a master
   	XSpiPs_SetOptions(&SpiInstance, XSPIPS_MASTER_OPTION | XSPIPS_CLK_PHASE_1_OPTION | XSPIPS_FORCE_SSELECT_OPTION);
    // Set the SPI clock prescaler
    XSpiPs_SetClkPrescaler(&SpiInstance, XSPIPS_CLK_PRESCALE_256);
    // initialize the imu
	IMU_init();


	while(1){
		int val = XGpioPs_ReadPin(&Gpio, 50);
		if( val > 0)
			getIMUdata();
    }

    cleanup_platform();
    return 0;
}

void delay(int cycles){
	int i = 0;
	while(i<cycles)
		i++;
	return;
}

void IMU_init(){
	u8 DataBuffer[2]; // addr + 32 bits read val
	// high power mode
	DataBuffer[0] = (u8) CTL1 & WRITE;
	DataBuffer[1] = (u8) 0x7C;
	// ONLY SET SLAVE SELECT ON THE 1ST TRANSFER
	XSpiPs_SetSlaveSelect(&SpiInstance,0x01);
	XSpiPs_PolledTransfer(&SpiInstance, DataBuffer, NULL, 2);
	delay(500);

	DataBuffer[0] = (u8) CTL2 & WRITE;
	DataBuffer[1] = (u8) 0x00;
	XSpiPs_SetSlaveSelect(&SpiInstance,0x01);
	XSpiPs_PolledTransfer(&SpiInstance, DataBuffer, NULL, 2);
	delay(500);

	// turn on the device
	DataBuffer[0] = (u8) CTL3 & WRITE;
	DataBuffer[1] = (u8) 0x80;
	XSpiPs_PolledTransfer(&SpiInstance, DataBuffer, NULL, 2);
	delay(250);
	return;
}

void getIMUdata(){

	// check status address
	u8 DataBuffer[2]; // addr + 32 bits read val
	DataBuffer[0] = (u8) STATUS_ADDRESS | READ;
	DataBuffer[1] = (u8) 0x00;
	XSpiPs_PolledTransfer(&SpiInstance, DataBuffer, &DataBuffer[0], 2);

	// if XY data's available, indicate so and grab it
	if ((DataBuffer[1] & 0x03) == 0x03) {
		u8 XYdata[5];
		XYdata[0] = (u8) DATA_ADDRESS|CASCADE|READ;
		XYdata[1] = (u8) 0x00;
		XYdata[2] = (u8) 0x00;
		XYdata[3] = (u8) 0x00;
		XYdata[4] = (u8) 0x00;
		XSpiPs_PolledTransfer(&SpiInstance, XYdata, &XYdata[0], 5);
		xData = (XYdata[2]<<8)|XYdata[1];
	    yData = (XYdata[4]<<8)|XYdata[3];
	}
	return;
}
