/*********************************************************************
 * FileName: HTTPPrint.h
 * Provides callback headers and resolution for user's custom
 * HTTP Application.
 * Processor: PIC18,PIC24E, PIC24F, PIC24H, dsPIC30F, dsPIC33F, dsPIC33E,PIC32
 * Compiler:  Microchip C18, C30, C32
 * 
 * This file is automatically generated by the MPFS Utility
 * ALL MODIFICATIONS WILL BE OVERWRITTEN BY THE MPFS GENERATOR
 *
 * Software License Agreement
 *
 * Copyright (C) 2012 Microchip Technology Inc.  All rights
 * reserved.
 *
 * Microchip licenses to you the right to use, modify, copy, and 
  * distribute: 
 * (i)  the Software when embedded on a Microchip microcontroller or 
 *      digital signal controller product ("Device") which is 
 *      integrated into Licensee's product; or 
 * (ii) ONLY the Software driver source files ENC28J60.c, ENC28J60.h,
 *		ENCX24J600.c and ENCX24J600.h ported to a non-Microchip device
 *		used in conjunction with a Microchip ethernet controller for
 *		the sole purpose of interfacing with the ethernet controller.
 *
 * You should refer to the license agreement accompanying this 
 * Software for additional information regarding your rights and 
 * obligations.
 *
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE 
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER 
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 *
 *********************************************************************/

#ifndef __HTTPPRINT_H
#define __HTTPPRINT_H

#include "TCPIP Stack/TCPIP.h"

#if defined(STACK_USE_HTTP2_SERVER)

extern HTTP_STUB httpStubs[MAX_HTTP_CONNECTIONS];
extern BYTE curHTTPID;

void HTTPPrint(DWORD callbackID);
void HTTPPrint_an_title(WORD);
void HTTPPrint_an_val(WORD);
void HTTPPrint_button(WORD);
void HTTPPrint_tcpip(WORD);
void HTTPPrint_stackmsg(void);
void HTTPPrint_lcdmsg(void);

void HTTPPrint(DWORD callbackID)
{
	switch(callbackID)
	{
        case 0x00000000:
			HTTPIncFile((ROM BYTE*)"ajax.inc");
			break;
        case 0x00000001:
			HTTPPrint_an_title(0);
			break;
        case 0x00000002:
			HTTPPrint_an_title(1);
			break;
        case 0x00000003:
			HTTPPrint_an_val(0);
			break;
        case 0x00000004:
			HTTPPrint_an_val(1);
			break;
        case 0x00000005:
			HTTPPrint_button(0);
			break;
        case 0x00000006:
			HTTPPrint_tcpip(1);
			break;
        case 0x00000007:
			HTTPPrint_tcpip(6);
			break;
        case 0x00000008:
			HTTPPrint_tcpip(16);
			break;
        case 0x00000009:
			HTTPPrint_tcpip(2);
			break;
        case 0x0000000a:
			HTTPPrint_tcpip(4);
			break;
        case 0x0000000b:
			HTTPPrint_tcpip(3);
			break;
        case 0x0000000c:
			HTTPPrint_tcpip(5);
			break;
        case 0x0000000d:
			HTTPPrint_tcpip(11);
			break;
        case 0x0000000e:
			HTTPPrint_tcpip(7);
			break;
        case 0x0000000f:
			HTTPPrint_tcpip(9);
			break;
        case 0x00000010:
			HTTPPrint_tcpip(10);
			break;
        case 0x00000011:
			HTTPPrint_tcpip(8);
			break;
        case 0x00000012:
			HTTPPrint_tcpip(0);
			break;
        case 0x00000013:
			HTTPPrint_tcpip(12);
			break;
        case 0x00000014:
			HTTPPrint_tcpip(14);
			break;
        case 0x00000015:
			HTTPPrint_tcpip(13);
			break;
        case 0x00000016:
			HTTPPrint_tcpip(15);
			break;
        case 0x00000017:
			HTTPPrint_stackmsg();
			break;
        case 0x00000018:
			HTTPPrint_lcdmsg();
			break;
		default:
			// Output notification for undefined values
			TCPPutROMArray(sktHTTP, (ROM BYTE*)"!DEF", 4);
	}

	return;
}

void HTTPPrint_(void)
{
	TCPPut(sktHTTP, '~');
	return;
}

#endif

#endif
