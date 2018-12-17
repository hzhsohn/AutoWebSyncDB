#include "zhTcpServer_select\tcp_serv_select.h"

#ifndef _JS_TCP_NET_H_
#define _JS_TCP_NET_H_

#ifdef __cplusplus
extern "C"{
#endif

//Э��ռһ���ֽ�
typedef enum _EzhDataProtocol{
	ezhCToSDataKeep=0,
	ezhSToCDataKeep=1,
	ezhCToSDataJsonToCache=2,//16�ֽ�MD5ֵ+�ַ���JSON����
	ezhSToCDataJsonToCacheSuccess=3,//���䵽����������ɹ�,β��16�ֽ�MD5ֵ
	ezhSToCDataJsonToCacheFail=5,//���䵽���������ʧ��,β��16�ֽ�MD5ֵ
	ezhSToCDataCacheUploadResult//16�ֽ�MD5ֵ,1�ֽ��ϴ����1�ɹ�,2ʧ��
}EzhDataProtocol;


void zhAccept(TzhNetSession* sion,void* info);
void zhRecv(TzhNetSession* sion,void* info,unsigned char* szBuf,int nLen);
void zhDisconnect(TzhNetSession* sion,void* info);
void zhError(TzhNetSession* sion,void* info,EzhNetError err);
void zhRealTime(TzhNetSession *sion,void* info);
void zhRecvPack(TzhNetSession* sion,TagUserInfo* info,unsigned short wCmd,TzhPacket* pack);

//--------------------------------------------
//�ظ��ͻ���
bool netSendKeep(TzhNetSession *sion);
bool netSendJsonToCacheSucc(TzhNetSession *sion,char* md516);
bool netSendJsonToCacheFail(TzhNetSession *sion,char* md516);
bool netSendCacheUploadResult(TzhNetSession *sion,char* md516,bool isOK);


#ifdef __cplusplus
}
#endif


#endif