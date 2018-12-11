#include <stdio.h>
#include "zhNet_Core\packet.h"
#include "zhNet_Core/session.h"

#ifndef _CLIENT_NET_H_

#ifdef __cplusplus
extern "C"{
#endif


int ClientNetInit();
int ClientNetLoop();



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