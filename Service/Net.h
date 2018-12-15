#include "zhTcpServer_select\tcp_serv_select.h"

#ifndef _JS_TCP_NET_H_
#define _JS_TCP_NET_H_

#ifdef __cplusplus
extern "C"{
#endif


void zhAccept(TzhNetSession* sion,void* info);
void zhRecv(TzhNetSession* sion,void* info,unsigned char* szBuf,int nLen);
void zhDisconnect(TzhNetSession* sion,void* info);
void zhError(TzhNetSession* sion,void* info,EzhNetError err);
void zhRealTime(TzhNetSession *sion,void* info);
void zhRecvPack(TzhNetSession* sion,TagUserInfo* info,unsigned short wCmd,TzhPacket* pack);

#ifdef __cplusplus
}
#endif


#endif