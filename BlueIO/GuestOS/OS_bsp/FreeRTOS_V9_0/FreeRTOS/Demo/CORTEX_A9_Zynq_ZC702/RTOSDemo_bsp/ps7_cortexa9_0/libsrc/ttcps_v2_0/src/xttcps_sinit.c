/******************************************************************************
*
* Copyright (C) 2010 - 2014 Xilinx, Inc.  All rights reserved.
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
/*****************************************************************************/
/**
*
* @file xttcps_sinit.c
* @addtogroup ttcps_v2_0
* @{
*
* The implementation of the XTtcPs driver's static initialization functionality.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who    Date     Changes
* ----- ------ -------- ---------------------------------------------
* 1.00a drg/jz 01/21/10 First release
* </pre>
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "xparameters.h"
#include "xttcps.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/
extern XTtcPs_Config XTtcPs_ConfigTable[];

/*****************************************************************************/
/**
*
* Looks up the device configuration based on the unique device ID. A table
* contains the configuration info for each device in the system.
*
* @param	DeviceId contains the unique ID of the device
*
* @return
*
* A pointer to the configuration found or NULL if the specified device ID was
* not found. See xttcps.h for the definition of XTtcPs_Config.
*
* @note		None.
*
******************************************************************************/
XTtcPs_Config *XTtcPs_LookupConfig(u16 DeviceId)
{
	XTtcPs_Config *CfgPtr = NULL;
	unsigned Index;

	for (Index = 0; Index < XPAR_XTTCPS_NUM_INSTANCES; Index++) {
		if (XTtcPs_ConfigTable[Index].DeviceId == DeviceId) {
			CfgPtr = &XTtcPs_ConfigTable[Index];
			break;
		}
	}

	return CfgPtr;
}
/** @} */
