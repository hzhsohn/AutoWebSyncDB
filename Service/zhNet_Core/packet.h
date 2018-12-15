/* 
  packet.h:the base data processed send to network
*/

/* 
  Copyright (C) 2008-2, Han.zhihong, Developer. Created 2008. 
  All rights reserved.

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

#pragma once
#ifndef _ZH_NET_C_SET_PACKET_

#ifdef __cplusplus
extern "C"{
#endif

#include "platform.h"
#include "gcdef.h"

/*
 *transmission data struct
 *
 *传输数据时打包的结构
*/
typedef struct _TzhPacket
{
	unsigned short	wPos;
	unsigned short	wSize;
	unsigned char	btBuf[ZH_NET_PACKET_BODY_LENGTH];
}TzhPacket;

/*
 *packet process function 
 *
 *数据包加解压处理函数
*/
void	zhPackWriteInit(TzhPacket *p);
void	zhPackReadInit(TzhPacket *p,unsigned char*szBuf,int nBufSize);

int		zhPackWriteChar(TzhPacket *p, char i);
int		zhPackReadChar(TzhPacket *p, char* i);
int		zhPackWriteShort(TzhPacket *p, short i);
int		zhPackReadShort(TzhPacket *p, short* i);
int		zhPackWriteInt(TzhPacket *p, int i);
int		zhPackReadInt(TzhPacket *p, int* i);
int		zhPackWriteLong(TzhPacket *p, long i);
int		zhPackReadLong(TzhPacket *p, long* i);
int		zhPackWriteString(TzhPacket *p, char* str);
int		zhPackWriteStringUnicode(TzhPacket *p, char* str);
int		zhPackReadString(TzhPacket *p, char* str);
int		zhPackReadStringUnicode(TzhPacket *p, char* str);
int		zhPackWriteBinary(TzhPacket *p, char* str ,int len);
int		zhPackReadBinary(TzhPacket *p, char* str ,int len);
int		zhPackWriteBool(TzhPacket *p,bool b);
int		zhPackReadBool(TzhPacket *p, bool*b);
int		zhPackWriteUnsignedChar(TzhPacket *p,unsigned char i);
int		zhPackReadUnsignedChar(TzhPacket *p,unsigned char* i);
int		zhPackWriteUnsignedShort(TzhPacket *p,unsigned short i);
int		zhPackReadUnsignedShort(TzhPacket *p,unsigned short* i);
int		zhPackWriteUnsignedInt(TzhPacket *p,unsigned int i);
int		zhPackReadUnsignedInt(TzhPacket *p,unsigned int* i);
int		zhPackWriteUnsignedLong(TzhPacket *p,unsigned long i);
int		zhPackReadUnsignedLong(TzhPacket *p,unsigned long* i);

#ifdef __cplusplus
}
#endif
#define _ZH_NET_C_SET_PACKET_
#endif
