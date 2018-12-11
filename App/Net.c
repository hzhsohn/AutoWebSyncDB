#include "zhTcpServer_select\tcp_serv_select.h"

////////////////////////////////////////////////////////////////////////
//保活包
void NetKeepTimeout(TzhNetSession*sion)
{
	TagUserInfo *info;
	time_t dwTmp;
	if(ezhNetStateConnected==sion->cState)
	{
		info=(TagUserInfo*)sion->pInfo;
		dwTmp=zhPlatGetTime();

		//32秒一次补活包超时检测
		if(dwTmp - info->dwKeepTime>32000)
		{
			zhSionSafeClose(sion);
			PRINTF("close sock=%d",sion->s);
			PRINTF("no packet receive...");
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
void zhAccept(TzhNetSession*sion,TagUserInfo* info)
{
	strcpy((char*)info->szAccount,"一个新连接");
	PRINTF("zhAccept..!!  Socket=%d ",sion->s);
}

void zhRecv(TzhNetSession *sion,void* info,unsigned char*szBuf,int nLen)
{
	TzhPacket pack;
	
	//刷新保活包时间
	TagUserInfo* pInfo=(TagUserInfo*)info;
	pInfo->dwKeepTime=zhPlatGetTime();
	
	PRINTF("zhRecv  nLen=%d",nLen);
	zhPackWriteInit(&pack);
	zhPackWriteString(&pack,"服务器发送过去的消息..");	
	zhSionSendPacket(sion,&pack);
}

void zhDisconnect(TzhNetSession*sion,void* info)
{
	PRINTF("Disconnected..!!  Socket=%d ",sion->s);
}

void zhError(TzhNetSession* sion,void* info,EzhNetError err)
{
	switch (err)
	{
		//没有错误
		case ezhNetNoError:
			printf("zhtcp err=%d ezhNetNoError\n",err);
		break;
		//收到数据包出错
		case ezhNetErrorPacketInvalid:
			printf("zhtcp err=%d ezhNetErrorPacketInvalid\n",err);
		break;
		//缓冲区溢出
		case ezhNetErrorCacheOverflow:
			printf("zhtcp err=%d ezhNetErrorCacheOverflow\n",err);
		break;
		//CRC校检出错
		case ezhNetErrorCRC16:
			printf("zhtcp err=%d ezhNetErrorCRC16\n",err);
		break;
	}

	//遇到错误立即断开连接
	//zhSionSafeClose(sion);
}

void zhRealTime(TzhNetSession *sion,void* info)
{
	//printf("zhRealTime\n");
	NetKeepTimeout(sion);
}

void zhRecvPack(TzhNetSession *sion,TagUserInfo*info,unsigned short wCmd,TzhPacket *pack)
{
	int i;
	char str[1024];

	zhPackReadInt(pack, &i);
	zhPackReadString(pack, str);
	
	zhPackWriteInit(pack);
	zhPackWriteString(pack,"服务器发送过去的消息..");	
	zhSendAllUser(pack);

	PRINTF("wCmd=%d,i=%d,str=%s",wCmd,i,str);
}
