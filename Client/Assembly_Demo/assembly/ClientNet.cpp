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
			TRACE("ezhSToCDataKeep \n");
			SendMessage(g_hwndMainForm,WM_USER+1002,0,0);
		break;
	case ezhSToCDataJsonToCacheSuccess: //���䵽����������ɹ�,β��32�ֽ�MD5ֵ
		{
			char key[36]={0};
			zhPackReadBinary(&pack,key,32);
			TRACE("ezhSToCDataJsonToCacheSuccess \n");
			SendMessage(g_hwndMainForm,WM_USER+1003,(WPARAM)&key,0);
		}
		break;
	case ezhSToCDataJsonToCacheFail:	//���䵽���������ʧ��,β��32�ֽ�MD5ֵ
		{
			char key[36]={0};
			zhPackReadBinary(&pack,key,32);
			TRACE("ezhSToCDataJsonToCacheFail \n");
			SendMessage(g_hwndMainForm,WM_USER+1004,(WPARAM)&key,0);
		}
		break;
	case ezhSToCDataCacheUploadResult:	//32�ֽ�MD5ֵ,1�ֽ��ϴ����1�ɹ�,2ʧ��
		{
			char key[36]={0};
			unsigned char ret=0;
			zhPackReadBinary(&pack,key,32);
			zhPackReadUnsignedChar(&pack,&ret);
			TRACE("ezhSToCDataCacheUploadResult key=%s ret=%d\n",key,ret);
			SendMessage(g_hwndMainForm,WM_USER+1005,(WPARAM)&key,(int)ret);
		}
		break;
	}
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

bool ClientNetSend(const char* md532,const char*json)
{
	//32�ֽ�MD5ֵ+�ַ���JSON����
	TzhPacket pack;
	zhPackWriteInit(&pack);
	zhPackWriteUnsignedChar(&pack, ezhCToSDataJsonToCache);
	zhPackWriteBinary(&pack, (char*)md532,32);
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
