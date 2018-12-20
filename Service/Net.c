#include "zhTcpServer_select\tcp_serv_select.h"
#include "Net.h"
//JSON的数据链表
TzhList g_lstJSONData;

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
	PRINTF("zhAccept..!!  Socket=%d ",sion->s);
}

void zhRecv(TzhNetSession *sion,void* info,unsigned char*szBuf,int nLen)
{
	TzhPacket pack;
	unsigned char cmd=0;
	
	//刷新保活包时间
	TagUserInfo* pInfo=(TagUserInfo*)info;
	pInfo->dwKeepTime=zhPlatGetTime();
	//
	zhPackReadInit(&pack,(BYTE*)szBuf,nLen);
	zhPackReadUnsignedChar(&pack,&cmd);
	switch(cmd)
	{
	case ezhCToSDataKeep:
			netSendKeep(sion);
		break;
	case ezhCToSDataJsonToCache: //32字节MD5值+字符串JSON数据
		{
			TzhJSONData*jd;
			jd=(TzhJSONData*)malloc(sizeof(TzhJSONData));
			memset(jd,0,sizeof(TzhJSONData));			
			zhPackReadBinary(&pack,jd->key,32);
			zhPackReadString(&pack,jd->json);
			if(zhListNodeAdd(&g_lstJSONData,jd,sizeof(TzhJSONData),0))
			{
				netSendJsonToCacheSucc(sion,jd->key);
			}
			else
			{
				netSendJsonToCacheFail(sion,jd->key);
			}
			PRINTF("ezhCToSDataJsonToCache ->key=%s strJson=%s \n",jd->key,jd->json);
		}
		break;
	}
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


bool netSendKeep(TzhNetSession *sion)
{
	TzhPacket pack;
	zhPackWriteInit(&pack);
	zhPackWriteUnsignedChar(&pack, ezhSToCDataKeep);
	return zhSionSend(sion,(char*)pack.btBuf,pack.wSize);
}
bool netSendJsonToCacheSucc(TzhNetSession *sion,char* md532)
{
	TzhPacket pack;
	zhPackWriteInit(&pack);
	zhPackWriteUnsignedChar(&pack, ezhSToCDataJsonToCacheSuccess);
	zhPackWriteBinary(&pack, md532,32);
	return zhSionSend(sion,(char*)pack.btBuf,pack.wSize);
}
bool netSendJsonToCacheFail(TzhNetSession *sion,char* md532)
{
	TzhPacket pack;
	zhPackWriteInit(&pack);
	zhPackWriteUnsignedChar(&pack, ezhSToCDataJsonToCacheFail);
	zhPackWriteBinary(&pack, md532,32);
	return zhSionSend(sion,(char*)pack.btBuf,pack.wSize);
}
void netSendCacheAllUserUploadResult(char* md532,unsigned char retVal)
{
	TzhPacket pack;
	zhPackWriteInit(&pack);
	zhPackWriteUnsignedChar(&pack, ezhSToCDataCacheUploadResult);
	zhPackWriteBinary(&pack, md532,32);
	zhPackWriteUnsignedChar(&pack, retVal);
	zhSendAllUser(&pack);
}

TzhList* getJsonDataList()
{
	return &g_lstJSONData;
}