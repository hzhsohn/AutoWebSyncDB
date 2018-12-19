#include <stdio.h>
#include "packet.h"
#include "session.h"

#ifndef _CLIENT_NET_H_

#ifdef __cplusplus
extern "C"{
#endif

//协议占一个字节
typedef enum _EzhDataProtocol{
	ezhCToSDataKeep=0,
	ezhSToCDataKeep=1,
	ezhCToSDataJsonToCache=2,//32字节MD5值+字符串JSON数据
	ezhSToCDataJsonToCacheSuccess=3,//传输到缓存服务器成功,尾随32字节MD5值
	ezhSToCDataJsonToCacheFail=5,//传输到缓存服务器失败,尾随32字节MD5值
	ezhSToCDataCacheUploadResult//32字节MD5值,1字节上传结果1成功,2失败
}EzhDataProtocol;

//-------------------------------
int ClientNetInit();
bool ClientNetSend(const char* md532,const char*json);
int ClientNetLoop();


//-------------------------------
void NetKeepTime(TzhNetSession*sion);
void zhConnect(TzhNetSession*sion,void*info,bool bResult);
void zhRecv(TzhNetSession*sion,void*info,char*szBuf,int nLen);
void zhDisconnect(TzhNetSession*sion,void*info);
void zhError(TzhNetSession *sion,void* info,EzhNetError err);


#ifdef __cplusplus
}
#endif

#define _CLIENT_NET_H_
#endif