#include "zhTcpServer_select\tcp_serv_select.h"

#ifndef _JS_TCP_NET_H_
#define _JS_TCP_NET_H_
#include "zhlist\zhlist_exp.h"


#ifdef __cplusplus
extern "C"{
#endif

//协议占一个字节
typedef enum _EzhDataProtocol{
	ezhCToSDataKeep=0,
	ezhSToCDataKeep=1,
	ezhCToSDataJsonToCache=2,//32字节MD5值+字符串JSON数据
	ezhSToCDataJsonToCacheSuccess=3,//传输到缓存服务器成功,尾随32字节MD5值
	ezhSToCDataJsonToCacheFail=4,//传输到缓存服务器失败,尾随32字节MD5值
	ezhSToCDataCacheUploadResult=5//32字节MD5值,0未知错误,1字节上传结果1成功,2失败,3数据库重复添加相同内容,4json格式错误
}EzhDataProtocol;

typedef struct _TzhJSONData
{
	char key[36];
	char json[2000];
}TzhJSONData;


void zhAccept(TzhNetSession* sion,void* info);
void zhRecv(TzhNetSession* sion,void* info,unsigned char* szBuf,int nLen);
void zhDisconnect(TzhNetSession* sion,void* info);
void zhError(TzhNetSession* sion,void* info,EzhNetError err);
void zhRealTime(TzhNetSession *sion,void* info);
void zhRecvPack(TzhNetSession* sion,TagUserInfo* info,unsigned short wCmd,TzhPacket* pack);

//--------------------------------------------
//回复客户端
bool netSendKeep(TzhNetSession *sion);
bool netSendJsonToCacheSucc(TzhNetSession *sion,char* md532);
bool netSendJsonToCacheFail(TzhNetSession *sion,char* md532);
void netSendCacheAllUserUploadResult(char* md532,unsigned char retVal);

//-----------------------------------
TzhList* getJsonDataList();

#ifdef __cplusplus
}
#endif


#endif