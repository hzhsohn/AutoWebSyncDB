#include "zhTcpServer_select\tcp_serv_select.h"

////////////////////////////////////////////////////////////////////////
//�����
void NetKeepTimeout(TzhNetSession*sion)
{
	TagUserInfo *info;
	time_t dwTmp;
	if(ezhNetStateConnected==sion->cState)
	{
		info=(TagUserInfo*)sion->pInfo;
		dwTmp=zhPlatGetTime();

		//32��һ�β������ʱ���
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
	strcpy((char*)info->szAccount,"һ��������");
	PRINTF("zhAccept..!!  Socket=%d ",sion->s);
}

void zhRecv(TzhNetSession *sion,void* info,unsigned char*szBuf,int nLen)
{
	TzhPacket pack;
	
	//ˢ�±����ʱ��
	TagUserInfo* pInfo=(TagUserInfo*)info;
	pInfo->dwKeepTime=zhPlatGetTime();
	
	PRINTF("zhRecv  nLen=%d",nLen);
	zhPackWriteInit(&pack);
	zhPackWriteString(&pack,"���������͹�ȥ����Ϣ..");	
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
		//û�д���
		case ezhNetNoError:
			printf("zhtcp err=%d ezhNetNoError\n",err);
		break;
		//�յ����ݰ�����
		case ezhNetErrorPacketInvalid:
			printf("zhtcp err=%d ezhNetErrorPacketInvalid\n",err);
		break;
		//���������
		case ezhNetErrorCacheOverflow:
			printf("zhtcp err=%d ezhNetErrorCacheOverflow\n",err);
		break;
		//CRCУ�����
		case ezhNetErrorCRC16:
			printf("zhtcp err=%d ezhNetErrorCRC16\n",err);
		break;
	}

	//�������������Ͽ�����
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
	zhPackWriteString(pack,"���������͹�ȥ����Ϣ..");	
	zhSendAllUser(pack);

	PRINTF("wCmd=%d,i=%d,str=%s",wCmd,i,str);
}
