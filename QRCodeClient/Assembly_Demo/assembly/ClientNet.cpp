#include "ClientNet.h"

TzhNetSession user;
TzhPacket pack;
DWORD dwKeepTime=0;



void NetKeepTime(TzhNetSession*sion)
{
    //�����Ҫ����zhSionReadData�������� 
	//TzhUserInfo *info;
	DWORD dwTmp;
	if(ezhNetStateConnected==sion->cState)
	{
		dwTmp=zhPlatGetTime();
		//15��һ�η��Ͳ����һ�� 
		if(dwTmp - dwKeepTime/*info->dwKeepTime*/>1500)
		{            
			//���
			TzhPacket pack;
			zhPackWriteInit(&pack);
			zhPackWriteInt(&pack, 2);
			zhPackWriteString(&pack, "�ɰ�");
			//����
			zhSionSend(sion,(char*)pack.btBuf,pack.wSize);
			//
            dwKeepTime=dwTmp;
		}
	}
}

void zhConnect(TzhNetSession*sion,void*info,bool bResult)
{
	if(bResult)
	{
		PRINTF("%s,Connecting!! socket=%d",info,sion->s);
	}
	else
	{
		PRINTF("%s,Connect Fail..!!",info);
	}
}

void zhRecv(TzhNetSession*sion,void*info,unsigned char*szBuf,int nLen)
{
	char Str[1024];
	TzhPacket pack;

	zhPackReadInit(&pack,(BYTE*)szBuf,nLen);
	zhPackReadString(&pack,Str);
	PRINTF("%s,Data -> Size=%d ,  Str=%s ",info,pack.wSize,Str);
}

void zhDisconnect(TzhNetSession*sion,void*info)
{
	PRINTF("%s,Disconnect..! socket=%d",info,sion->s);
}

void zhError(TzhNetSession*sion,void* info,EzhNetError err)
{
	printf("zhtcp %s,err=%d\n",info,err);
}

int ClientNetInit()
{
	
	//��ʼ������
	printf("��ʼ����");
	zhSionInit(&user,0);
	zhSionSetInfo(&user,"�ҽ�С��");
	zhSionConnect(&user,"localhost",7666);

	//���û�������С
	zhSionSetBigSockCache(&user,ezhPackCacheDefault);
	
	return 0;
}

int ClientNetLoop()
{
	//�������ݴ���-------begin
	unsigned char frame[ZH_NET_PACKET_BODY_LENGTH];
	EzhNetError err;
	int ret;
	if(false==zhSionCacheData(&user,&err))
	{
		zhError(&user,&user.pInfo,err);
	}
	while(1)
	{
		ret=zhSionReadData(&user,frame,&err);
		if(0==ret)
		{ break; }
		else if(ret>0)
		{
			//����frame����
			zhRecv(&user,
				user.pInfo,
				frame,
				ret);
		}
		else
		{
			zhError(&user,&user.pInfo,err);
		}
	}
	switch(zhSionStateThread(&user))
	{
			//���ӳɹ�
		case ezhNetEventConnected:
			zhConnect(&user, user.pInfo,true);
			break;
			//����ʧ��
		case ezhNetEventConnectTimeout:
			break;
			//�Ͽ�����
		case ezhNetEventDisconnect:
			zhDisconnect(&user, user.pInfo);
			break;
			//û���¼�
		case ezhNetNoEvent:
			break;
	}
	//------------------end
	NetKeepTime(&user);

	return 0;
}