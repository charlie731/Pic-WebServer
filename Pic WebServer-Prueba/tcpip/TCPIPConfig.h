#ifndef __TCPIPCONFIG_H
#define __TCPIPCONFIG_H

#include "GenericTypeDefs.h"
#include "Compiler.h"

#define STACK_USE_ICMP_SERVER    1
#define STACK_USE_TCP            1
#define STACK_USE_DHCP_CLIENT    1

#define STACK_CCS_SMTP_TX_SIZE    0
#define STACK_CCS_SMTP_RX_SIZE    0

#define STACK_USE_CCS_HTTP2_SERVER
#define STACK_CCS_HTTP2_SERVER_TX_SIZE    1500
#define STACK_CCS_HTTP2_SERVER_RX_SIZE    500
#define HTTP_NUM_SOCKETS                  1
#define HTTP_PORT                         (80u)
#define HTTP_USE_DOUBLE_ESCAPE            1
#import(RAW, file="mpfs/mpfsimg.bin", location=MPFS_Start)

#define STACK_MY_TELNET_SERVER_TX_SIZE    0
#define STACK_MY_TELNET_SERVER_RX_SIZE    0

#define STACK_MY_HTTPC_RX_SIZE    0
#define STACK_MY_HTTPC_TX_SIZE    0

#define MY_DEFAULT_HOST_NAME      "CCS_EXAMPLES"

#define MY_DEFAULT_MAC_BYTE1      (0x0)
#define MY_DEFAULT_MAC_BYTE2      (0x8)
#define MY_DEFAULT_MAC_BYTE3      (0xdc)
#define MY_DEFAULT_MAC_BYTE4      (0x18)
#define MY_DEFAULT_MAC_BYTE5      (0x7c)
#define MY_DEFAULT_MAC_BYTE6      (0x6)

#define MY_DEFAULT_IP_ADDR_BYTE1  (10ul)
#define MY_DEFAULT_IP_ADDR_BYTE2  (2ul)
#define MY_DEFAULT_IP_ADDR_BYTE3  (10ul)
#define MY_DEFAULT_IP_ADDR_BYTE4  (200ul)

#define MY_DEFAULT_GATE_BYTE1     (10ul)
#define MY_DEFAULT_GATE_BYTE2     (2ul)
#define MY_DEFAULT_GATE_BYTE3     (10ul)
#define MY_DEFAULT_GATE_BYTE4     (1ul)

#define MY_DEFAULT_MASK_BYTE1     (255ul)
#define MY_DEFAULT_MASK_BYTE2     (255ul)
#define MY_DEFAULT_MASK_BYTE3     (255ul)
#define MY_DEFAULT_MASK_BYTE4     (0ul)

#define TCP_CONFIGURATION      1

#define TCP_ETH_RAM_SIZE (STACK_CCS_SMTP_TX_SIZE + \
                          STACK_CCS_SMTP_RX_SIZE + \
                          STACK_CCS_HTTP2_SERVER_TX_SIZE + \
                          STACK_CCS_HTTP2_SERVER_RX_SIZE + \
                          STACK_MY_TELNET_SERVER_TX_SIZE + \
                          STACK_MY_TELNET_SERVER_RX_SIZE + \
                          STACK_MY_HTTPC_TX_SIZE + \
                          STACK_MY_HTTPC_RX_SIZE + \
                          100*TCP_CONFIGURATION)

// Define names of socket types
#define TCP_PURPOSE_GENERIC_TCP_CLIENT 0
#define TCP_PURPOSE_GENERIC_TCP_SERVER 1
#define TCP_PURPOSE_TELNET             2
#define TCP_PURPOSE_FTP_COMMAND        3
#define TCP_PURPOSE_FTP_DATA           4
#define TCP_PURPOSE_TCP_PERFORMANCE_TX 5
#define TCP_PURPOSE_TCP_PERFORMANCE_RX 6
#define TCP_PURPOSE_UART_2_TCP_BRIDGE  7
#define TCP_PURPOSE_HTTP_SERVER        8
#define TCP_PURPOSE_DEFAULT            9
#define TCP_PURPOSE_BERKELEY_SERVER    10
#define TCP_PURPOSE_BERKELEY_CLIENT    11
#define TCP_PURPOSE_CCS_SMTP           0x40

#ifndef MAX_HTTP_CONNECTIONS
   #define  MAX_HTTP_CONNECTIONS 1
#endif
#ifndef MAX_UDP_SOCKETS
   #define MAX_UDP_SOCKETS 7
#endif

#endif
