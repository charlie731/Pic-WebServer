///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2014 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
////                                                                   ////
//// http://www.ccsinfo.com                                            ////
///////////////////////////////////////////////////////////////////////////

#ifndef __CCS_ANNOUNCE_C
#define __CCS_ANNOUNCE_C

#include "TCPIPConfig.h"

#if defined(STACK_USE_CCS_ANNOUNCE)

#include "TCPIP Stack/TCPIP.h"

// The announce port
#ifndef ANNOUNCE_PORT
#define ANNOUNCE_PORT   6789
#endif

#ifndef ANNOUNCE_RATE
#define ANNOUNCE_RATE   (TICKS_PER_SECOND*3)
#endif

#ifndef ANNOUNCE_NUM
#define ANNOUNCE_NUM 3
#endif

#ifndef ANNOUNCE_MSG
#define ANNOUNCE_MSG "PIC TCP/IP Announce"
#endif

#ifndef ANNOUNCE_ON_SEND
#define ANNOUNCE_ON_SEND()
#endif

struct
{
   UDP_SOCKET socket;
   TICK t;
   unsigned int8 num;
   NODE_INFO node;
   int1 nodeValid;
} g_Announce = {INVALID_UDP_SOCKET};

void AnnounceInit(void)
{
   if (g_Announce.socket != INVALID_UDP_SOCKET)
   {
      UDPClose(g_Announce.socket);
   }
  
   memset(&g_Announce, 0x00, sizeof(g_Announce));
   
   g_Announce.t = TickGet();
   g_Announce.num = ANNOUNCE_NUM;
   g_Announce.socket = INVALID_UDP_SOCKET;
}

#define AnnounceOpenUDP() UDPOpen(ANNOUNCE_PORT, &g_Announce.node, ANNOUNCE_PORT)

static int1 AnnounceSend(void)
{
   unsigned int8 len;
   int1 ret = FALSE;
   
   if (g_Announce.socket == INVALID_UDP_SOCKET)
   {
      g_Announce.socket = AnnounceOpenUDP();
   }
   if (g_Announce.socket == INVALID_UDP_SOCKET)
      return(FALSE);
   
   ANNOUNCE_ON_SEND();
   
   len = strlen(ANNOUNCE_MSG);
   
   if (len <= UDPIsPutReady(g_Announce.socket))
   {
      ret = TRUE;

      UDPPutArray(ANNOUNCE_MSG, len);

      UDPFlush();
   }
   
   UDPClose(g_Announce.socket);
   g_Announce.socket = INVALID_UDP_SOCKET;
  
   return(ret);
}

void AnnounceTask(void)
{  
  #if defined(ANNOUNCE_SCAN)
   char scr[80];
   static unsigned int8 len;
  #endif
   
   if (!DHCPBoundOrDisabled())
   {
      g_Announce.nodeValid = FALSE;
      
      if (g_Announce.socket != INVALID_UDP_SOCKET)
      {
         UDPClose(g_Announce.socket);
         g_Announce.socket = INVALID_UDP_SOCKET;
      }
   }
  
   if (!g_Announce.nodeValid && DHCPBoundOrDisabled())
   {
      // broadcast MAC address
      memset(&g_Announce.node.MACAddr, 0xFF, 6);
      
      // Set the IP subnet's broadcast address (broadcast UDP address)
      g_Announce.node.IPAddr.Val = (AppConfig.MyIPAddr.Val & AppConfig.MyMask.Val) |
                   ~AppConfig.MyMask.Val;

     #if defined(ANNOUNCE_SCAN)
      len = strlen(ANNOUNCE_SCAN);
      if (len >= sizeof(scr))
      {
         len = sizeof(scr) - 1;
      }
     #endif
                   
      g_Announce.nodeValid = TRUE;
   }
   
  #if defined(ANNOUNCE_SCAN)
   if (g_Announce.nodeValid && (g_Announce.socket == INVALID_UDP_SOCKET))
   {
      g_Announce.socket = AnnounceOpenUDP();
   }  
   
   if
   (
      g_Announce.nodeValid &&
      (g_Announce.socket != INVALID_UDP_SOCKET) &&
      (UDPIsGetReady(g_Announce.socket) >= len)
   )
   {
      UDPGetArray(scr, len);
      scr[len] = 0;
      
      if (strcmp(scr, ANNOUNCE_SCAN) == 0)
      {
         g_Announce.num = 1;
         g_Announce.t = TickGet() - ANNOUNCE_RATE;
      }
   }
  #endif
   
   if 
   (
      g_Announce.nodeValid &&
      MACIsLinked() &&
      DHCPBoundOrDisabled() &&
      g_Announce.num && 
      ((TickGet() - g_Announce.t) >= (TICK)ANNOUNCE_RATE) &&
      AnnounceSend()
   )
   {
      g_Announce.num--;
      g_Announce.t = TickGet();
   }
}

#endif //#if defined(STACK_USE_ANNOUNCE)
#endif   //#ifndef __CCS_ANNOUNCE_C
