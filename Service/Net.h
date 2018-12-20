#include "zhTcpServer_select\tcp_serv_select.h"

#ifndef _JS_TCP_NET_H_
#define _JS_TCP_NET_H_
#include "zhlist\zhlist_exp.h"


#ifdef __cplusplus
extern "C"{
#endif

//Э��ռһ���ֽ�
typedef enum _EzhDataProtocol{
	ezhCToSDataKeep=0,
	ezhSToCDataKeep=1,
	ezhCToSDataJsonToCache=2,//32�ֽ�MD5ֵ+�ַ���JSON����
	ezhSToCDataJsonToCacheSuccess=3,//���䵽����������ɹ�,β��32�ֽ�MD5ֵ
	ezhSToCDataJsonToCacheFail=4,//���䵽���������ʧ��,β��32�ֽ�MD5ֵ
	ezhSToCDataCacheUploadResult=5//32�ֽ�MD5ֵ,0δ֪����,1�ֽ��ϴ����1�ɹ�,2ʧ��,3���ݿ��ظ������ͬ����,4json��ʽ����
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
//�ظ��ͻ���
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