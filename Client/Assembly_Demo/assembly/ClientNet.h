#include <stdio.h>
#include "packet.h"
#include "session.h"

#ifndef _CLIENT_NET_H_

#ifdef __cplusplus
extern "C"{
#endif

//Э��ռһ���ֽ�
typedef enum _EzhDataProtocol{
	ezhCToSDataKeep=0,
	ezhSToCDataKeep=1,
	ezhCToSDataJsonToCache=2,//32�ֽ�MD5ֵ+�ַ���JSON����
	ezhSToCDataJsonToCacheSuccess=3,//���䵽����������ɹ�,β��32�ֽ�MD5ֵ
	ezhSToCDataJsonToCacheFail=5,//���䵽���������ʧ��,β��32�ֽ�MD5ֵ
	ezhSToCDataCacheUploadResult//32�ֽ�MD5ֵ,1�ֽ��ϴ����1�ɹ�,2ʧ��
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