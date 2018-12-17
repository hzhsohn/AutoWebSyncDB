#include "stdafx.h"
#include "ClientNet.h"
#include "md5\md5.h"

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
			zhPackWriteUnsignedChar(&pack, ezhCToSDataKeep);
			//����
			zhSionSend(sion,(char*)pack.btBuf,pack.wSize);
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
		TRACE("%s,Connecting!! socket=%d\n",info,sion->s);
		SendMessage(g_hwndMainForm,WM_USER+1000,0,0);
	}
	else
	{
		TRACE("%s,Connect Fail..!!\n",info);
		SendMessage(g_hwndMainForm,WM_USER+1001,0,0);
	}
}

void zhRecv(TzhNetSession*sion,void*info,unsigned char*szBuf,int nLen)
{
	unsigned char cmd=0;
	TzhPacket pack;

	zhPackReadInit(&pack,(BYTE*)szBuf,nLen);
	zhPackReadUnsignedChar(&pack,&cmd);
	switch(cmd)
	{
	case ezhSToCDataKeep:
		break;
	case ezhSToCDataJsonToCacheSuccess: //���䵽����������ɹ�,β��16�ֽ�MD5ֵ
		break;
	case ezhSToCDataJsonToCacheFail:	//���䵽���������ʧ��,β��16�ֽ�MD5ֵ
		break;
	case ezhSToCDataCacheUploadResult:	//16�ֽ�MD5ֵ,1�ֽ��ϴ����1�ɹ�,2ʧ��
		break;
	}
	TRACE("%s,Data -> Size=%d ,  Cmd=%d \n",info,pack.wSize,cmd);
}

void zhDisconnect(TzhNetSession*sion,void*info)
{
	dwReconnectTime=0;
	TRACE("%s,Disconnect..! socket=%d\n",info,sion->s);
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

bool ClientNetSend(const char*json)
{
	char strMD5[20]={0};
	MDString((char*)json,strMD5);
	//16�ֽ�MD5ֵ+�ַ���JSON����
	TzhPacket pack;
	zhPackWriteInit(&pack);
	zhPackWriteUnsignedChar(&pack, ezhCToSDataJsonToCache);
	zhPackWriteBinary(&pack, strMD5,16);
	zhPackWriteString(&pack, (char*)json);
	//����
	return zhSionSend(&user,(char*)pack.btBuf,pack.wSize);
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