/*
  gcdef.h:the transmission manage
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



#ifndef __gcdef_h__
#include "platform.h"

#pragma pack(1)
 
/*
 *
 *Begin---------->Network layer protocol in GCHPacket.h module docking
 *
 *Begin---------->网络层对接协议在GCHPacket.h模块内
*/

#define ZH_NET_FRAME_FLAG	0x0A	//zhNet里面的帧固定值

typedef struct _TzhPackFrameHeader
{
/*
 *Packet type, EzhPackType structural break
 *
 *包的类型,EzhPackType结构休
*/
	unsigned char    yFrameFlag;	//固定 ZH_NET_FRAME_FLAG
	unsigned short	wDataLen;   //后随的包体的长度
	unsigned short	wCRC16;     //包体的CRC校检,当包长度为0时CRC校检值为0
}TzhPackFrameHeader;

/*
 *Begin---------->In the GCHNetUtil.h definition
 *
 *Begin---------->在GCHNetUtil.h定义
 */
 
/*
 *Each time the maximum length, not including package head
 *
 *每次发包的最大长度，不包括包头
*/
#define ZH_NET_PACKET_BODY_LENGTH			1048			
			
//End---------->


typedef struct _TzhPackFrame
{
	TzhPackFrameHeader header;
	char	data[ZH_NET_PACKET_BODY_LENGTH];
}TzhPackFrame;
//End---------->

#pragma pack()

#define __gcdef_h__
#endif
