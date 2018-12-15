#include "session.h"

TzhNetListen g_listern;
TzhNetSession clientUser;

void zhRecv(TzhNetSession *sion,void* info,char*szBuf,int nLen);
void zhDisconnect(TzhNetSession*sion,void* info);
void zhError(TzhNetSession* sion,void* info,EzhNetError err);

int main(int argc,char *argv[])
{
	unsigned char frame[ZH_NET_PACKET_BODY_LENGTH];
	EzhNetError err;
	int ret;
	TzhNetSession tmpUser;
	
	//初始化网络
	ret=zhSionStartup(&g_listern,2323,false);
	if(ret)
	{
		PRINTF("Startup Server ok..!!");
		PRINTF("BindPort=%d",2323);
		while(true)
		{
				zhPlatSleep(1);
		
				//
				if(zhSionAccept(&g_listern,&tmpUser))
				{
					clientUser=tmpUser;
				}

				//网络数据处理-------begin
				if(false==zhSionCacheData(&clientUser,&err))
				{
					zhError(&clientUser,&clientUser.pInfo,err);
				}
				while(1)
				{
					ret=zhSionReadData(&clientUser,frame,&err);
					if(0==ret)
					{ break; }
					else if(ret>0)
					{
						//处理frame数据
						zhRecv(&clientUser,
							clientUser.pInfo,
							frame,
							ret);
					}
					else
					{
						zhError(&clientUser, clientUser.pInfo, err);
					}
				}
				switch(zhSionStateThread(&clientUser))
				{
						//连接成功
					case ezhNetEventConnected:
						break;
						//连接失败
					case ezhNetEventConnectTimeout:
						break;
						//断开连接
					case ezhNetEventDisconnect:
						zhDisconnect(&clientUser,
							clientUser.pInfo);
						break;
						//没有事件
					case ezhNetNoEvent:
						break;
				}
				//------------------end
		}
	}
	return 0;
}

void zhRecv(TzhNetSession *sion,void* info,unsigned char*szBuf,int nLen)
{
	TzhPacket pack;
	int idid;
	char sstr[550];

	zhPackReadInit(&pack,(BYTE*)szBuf,nLen);
	zhPackReadInt(&pack,&idid);
	zhPackReadString(&pack,sstr);

	PRINTF("zhRecv  nLen=%d  id=%d str=%s",nLen,idid,sstr);

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
		//收到数据包出错
		case ezhNetErrorPacketInvalid:
		break;
		//缓冲区溢出
		case ezhNetErrorCacheOverflow:
		break;
		//CRC校检出错
		case ezhNetErrorCRC16:
		break;
	}

}
