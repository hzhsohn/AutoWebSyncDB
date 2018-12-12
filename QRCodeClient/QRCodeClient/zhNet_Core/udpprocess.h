/*
  udpprocess.h - udp transmission verify moudle
*/

/* Copyright (C) 2008-2, Han.zhihong, Developer. Created 2008. All
rights reserved.

  License to copy and use this software is granted provided that it
  is identified as the "Han.zhihong. Message-Digest
  Algorithm" in all material mentioning or referencing this software
  or this function.
  
  Han.zhihong. makes no representations concerning either
  the merchantability of this software or the suitability of this
  software for any particular purpose. It is provided "as is"
  without express or implied warranty of any kind.
	  
  These notices must be retained in any copies of any part of this
  documentation and/or software.
*/


#ifndef __C_ZH_NET_UDP_SOCKET_H__
#include "socket.h"
#include "packet.h"

#ifdef __cplusplus
extern "C"{
#endif

/*
 *udp sendto function,the packet not more than 1024B to well 
 *
 *因为是UDP发送,所以每个包不要超过1024Byte,为最佳
 *zhUdpSendTo 原始数据格式: 两字节CRC16校验+发送内容
 *
 *返回值为发送的字节
 */
int zhUdpSendToPack(SOCKET s,TzhPacket *pack,struct sockaddr_in *addr);
int zhUdpSendTo(SOCKET s,unsigned char *buf,int len,struct sockaddr_in *addr);

/*
 *udp recvfrom function
 *
 *接收UDP数据函数
 *zhUdpRecvFrom 原始数据格式:内容+两字节CRC16校验
 */
int zhUdpRecvFrom(SOCKET s,unsigned char *buf,int len,struct sockaddr_in *addr,int *addrlen);

/*
	地址转换函数
*/
#define zhUdpAddrToPram		zhSockAddrToPram
#define zhUdpPramToAddr		zhSockPramToAddr

#ifdef __cplusplus
}
#endif
#define __C_ZH_NET_UDP_SOCKET_H__
#endif
