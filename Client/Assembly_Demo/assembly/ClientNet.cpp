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
    //这代码要放在zhSionReadData函数后面 
	
	time_t dwTmp;
	if(ezhNetStateConnected==sion->cState)
	{
		dwTmp=zhPlatGetTime();
		//15秒一次发送补活包一次 
		if(dwTmp - dwKeepTime>15000)
		{            
			//保活包
			TzhPacket pack;
			zhPackWriteInit(&pack);
			zhPackWriteUnsignedChar(&pack, ezhCToSDataKeep);
			//发送
			zhSionSend(sion,(char*)pack.btBuf,pack.wSize);
			dwKeepTime=dwTmp;
		}
	}
	
	//断开重连
	if(ezhNetStateZero==sion->cState)
	{
		//5秒重新连接
		dwTmp=zhPlatGetTime();
		if(dwTmp - dwReconnectTime>5000)
		{
			//初始化网络
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
	case ezhSToCDataJsonToCacheSuccess: //传输到缓存服务器成功,尾随16字节MD5值
		break;
	case ezhSToCDataJsonToCacheFail:	//传输到缓存服务器失败,尾随16字节MD5值
		break;
	case ezhSToCDataCacheUploadResult:	//16字节MD5值,1字节上传结果1成功,2失败
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
	TRACE("初始化连接\n");
	//初始化网络
	zhSionInit(&user,0);
	zhSionSetInfo(&user,"");
	//设置缓冲区大小
	zhSionSetBigSockCache(&user,ezhPackCacheDefault);	
	//
	zhSionConnect(&user,"localhost",7666);
	return 0;
}

bool ClientNetSend(const char*json)
{
	char strMD5[20]={0};
	MDString((char*)json,strMD5);
	//16字节MD5值+字符串JSON数据
	TzhPacket pack;
	zhPackWriteInit(&pack);
	zhPackWriteUnsignedChar(&pack, ezhCToSDataJsonToCache);
	zhPackWriteBinary(&pack, strMD5,16);
	zhPackWriteString(&pack, (char*)json);
	//发送
	return zhSionSend(&user,(char*)pack.btBuf,pack.wSize);
}
int ClientNetLoop()
{
	//网络数据处理-------begin
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
			//处理frame数据
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
			//连接成功
		case ezhNetEventConnected:
			zhConnect(&user, user.pInfo,true);
			break;
			//连接失败
		case ezhNetEventConnectTimeout:
			break;
			//断开连接
		case ezhNetEventDisconnect:
			zhDisconnect(&user, user.pInfo);
			break;
			//没有事件
		case ezhNetNoEvent:
			break;
	}
	//------------------end
	NetKeepTime(&user);

	return 0;
}
