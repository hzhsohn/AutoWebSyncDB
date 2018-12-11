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


#include "udpprocess.h"


int zhUdpSendToPack(SOCKET s,TzhPacket *pack,struct sockaddr_in *addr)
{
    if(pack)
    {
       return zhUdpSendTo(s,pack->btBuf,pack->wSize,addr);
    }
    else
    { return 0; }
}

int zhUdpSendTo(SOCKET s,unsigned char *buf,int len,struct sockaddr_in *addr)
{
	char *send_buf;
	int send_len;
	unsigned short sum;
	
	send_len=len+2;
	sum=zhPlatCRC16(buf,len);;

	//将包添加CRC校检
	send_buf=(char*)malloc(send_len);
	RE_MEMORY(&sum,sizeof(unsigned short));
	memcpy(&send_buf[0],&sum,2);
	memcpy(&send_buf[2],buf,len);
	//发送
	send_len=zhSockSendTo(s,send_buf,send_len,addr);
	free(send_buf);
	send_buf=NULL;
	return send_len;
}


int zhUdpRecvFrom(SOCKET s,unsigned char *buf,int buf_len,struct sockaddr_in *addr,int *addrlen)
{
	unsigned short sum,sum2;
	int ret;
	//接收
	ret=zhSockRecvFrom(s,(char*)buf,buf_len,addr,addrlen);
	if(ret>2)
	{
		ret=ret-2;
		memcpy(&sum,&buf[0],2);
		RE_MEMORY(&sum,2);
		sum2=zhPlatCRC16(&buf[2],ret);
		if(sum!=sum2)
		{
			//Sys_print16(ret,buf);
			//printf("\n");
			return 0;
		}
		//数据正确
		memmove(&buf[0],&buf[2],ret);
	}
	return ret;
}
