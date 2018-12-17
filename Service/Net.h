#include "zhTcpServer_select\tcp_serv_select.h"

#ifndef _JS_TCP_NET_H_
#define _JS_TCP_NET_H_

#ifdef __cplusplus
extern "C"{
#endif

//协议占一个字节
typedef enum _EzhDataProtocol{
	ezhCToSDataKeep=0,
	ezhSToCDataKeep=1,
	ezhCToSDataJsonToCache=2,//16字节MD5值+字符串JSON数据
	ezhSToCDataJsonToCacheSuccess=3,//传输到缓存服务器成功,尾随16字节MD5值
	ezhSToCDataJsonToCacheFail=5,//传输到缓存服务器失败,尾随16字节MD5值
	ezhSToCDataCacheUploadResult//16字节MD5值,1字节上传结果1成功,2失败
}EzhDataProtocol;


void zhAccept(TzhNetSession* sion,void* info);
void zhRecv(TzhNetSession* sion,void* info,unsigned char* szBuf,int nLen);
void zhDisconnect(TzhNetSession* sion,void* info);
void zhError(TzhNetSession* sion,void* info,EzhNetError err);
void zhRealTime(TzhNetSession *sion,void* info);
void zhRecvPack(TzhNetSession* sion,TagUserInfo* info,unsigned short wCmd,TzhPacket* pack);

//--------------------------------------------
//回复客户端
bool netSendKeep(TzhNetSession *sion);
bool netSendJsonToCacheSucc(TzhNetSession *sion,char* md516);
bool netSendJsonToCacheFail(TzhNetSession *sion,char* md516);
bool netSendCacheUploadResult(TzhNetSession *sion,char* md516,bool isOK);


#ifdef __cplusplus
}
#endif


#endif