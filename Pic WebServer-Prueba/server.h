#include <18F4620.h>
#device ADC=8

/*
TCP/IP Stack enabled.
Many TCP/IP configuration settings (servers enabled, ports used,
etc) are defined in TCPIPConfig.h.
Many hardware configuration settings (SPI port and GPIO pins used)
are defined in HardwareProfile.h.
*/

#include "tcpip/p18cxxx.h"
#use delay(internal=32MHz)
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream=TERMINAL)


#define MIN(a,b)  ((a > b) ? b : a)

#include <stdint.h>
#include "tcpip/StackTsk2.h"
#include "tcpip/TCPIPConfig.h"
#include "tcpip/HardwareProfile.h"

typedef struct
{
   BYTE vSocketPurpose;
   BYTE vMemoryMedium;
   WORD wTXBufferSize;
   WORD wRXBufferSize;
} TCPSocketInitializer_t;

#if TCP_CONFIGURATION > 0
   TCPSocketInitializer_t TCPSocketInitializer[TCP_CONFIGURATION] =
   {
      #if defined(STACK_USE_CCS_HTTP2_SERVER)
         {TCP_PURPOSE_HTTP_SERVER, TCP_ETH_RAM, STACK_CCS_HTTP2_SERVER_TX_SIZE, STACK_CCS_HTTP2_SERVER_RX_SIZE},
      #endif
      #if defined(STACK_USE_SMTP_CLIENT)
         {TCP_PURPOSE_DEFAULT, TCP_ETH_RAM, STACK_CCS_SMTP_TX_SIZE, STACK_CCS_SMTP_RX_SIZE},
      #endif
      #if defined(STACK_USE_MY_TELNET_SERVER)
         {TCP_PURPOSE_TELNET, TCP_ETH_RAM, STACK_MY_TELNET_SERVER_TX_SIZE, STACK_MY_TELNET_SERVER_RX_SIZE},
      #endif
      #if defined(STACK_USE_CCS_HTTP_CLIENT)
         {TCP_PURPOSE_GENERIC_TCP_CLIENT, TCP_ETH_RAM, STACK_MY_HTTPC_TX_SIZE, STACK_MY_HTTPC_RX_SIZE},
      #endif
   };
#else
   #undef TCP_CONFIGURATION
   #define TCP_CONFIGURATION 1
   TCPSocketInitializer_t TCPSocketInitializer[TCP_CONFIGURATION] =
   {
      {TCP_PURPOSE_DEFAULT, TCP_ETH_RAM, 250, 250}
   };
#endif

#include "tcpip/StackTsk2.c"

