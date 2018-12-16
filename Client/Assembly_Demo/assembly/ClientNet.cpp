#include "stdafx.h"
#include "ClientNet.h"

TzhNetSession user;
TzhPacket pack;
time_t dwKeepTime=0;
time_t dwReconnectTime=0;
extern HWND g_hwndMainForm;

void NetKeepTime(TzhNetSession*sion)
{
    //�����Ҫ����zhSionReadData�������� 
	
	time_t dwTmp;
	if(ezhNetStateConnected==sion->cState)
	{
		dwTmp=zhPlatGetTime();
		//15��һ�η��Ͳ����һ�� 
		if(dwTmp - dwKeepTime>15000)
		{            
			//�����
			TzhPacket pack;
			zhPackWriteInit(&pack);
			zhPackWriteInt(&pack, 0);
			//����
			zhSionSend(sion,(char*)pack.btBuf,pack.wSize);
			//
            dwKeepTime=dwTmp;
		}
	}
	
	//�Ͽ�����
	if(ezhNetStateZero==sion->cState)
	{
		//5����������
		dwTmp=zhPlatGetTime();
		if(dwTmp - dwReconnectTime>5000)
		{
			//��ʼ������
			zhSionInit(&user,0);
			zhSionConnect(&user,"localhost",7666);
			//
            dwReconnectTime=dwTmp;
		}
	}
}

void zhConnect(TzhNetSession*sion,void*info,bool bResult)
{
	if(bResult)
	{
		TRACE("%s,Connecting!! socket=%d",info,sion->s);
		SendMessage(g_hwndMainForm,WM_USER+1000,0,0);
	}
	else
	{
		TRACE("%s,Connect Fail..!!",info);
		SendMessage(g_hwndMainForm,WM_USER+1001,0,0);
	}
}

void zhRecv(TzhNetSession*sion,void*info,unsigned char*szBuf,int nLen)
{
	char Str[1024];
	TzhPacket pack;

	zhPackReadInit(&pack,(BYTE*)szBuf,nLen);
	zhPackReadString(&pack,Str);
	TRACE("%s,Data -> Size=%d ,  Str=%s ",info,pack.wSize,Str);
}

void zhDisconnect(TzhNetSession*sion,void*info)
{
	dwReconnectTime=0;
	TRACE("%s,Disconnect..! socket=%d",info,sion->s);
	SendMessage(g_hwndMainForm,WM_USER+1001,0,0);
}

void zhError(TzhNetSession*sion,void* info,EzhNetError err)
{
	TRACE("zhtcp %s,err=%d\n",info,err);
}

int ClientNetInit()
{	
	TRACE("��ʼ������\n");
	//��ʼ������
	zhSionInit(&user,0);
	zhSionSetInfo(&user,"");

	//���û�������С
	zhSionSetBigSockCache(&user,ezhPackCacheDefault);
	
	//
	zhSionConnect(&user,"localhost",7666);
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
