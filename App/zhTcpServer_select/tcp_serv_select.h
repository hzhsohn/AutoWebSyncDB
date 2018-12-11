#ifndef _ZH_NET_PROCESS_TCP_H_
#include <stdio.h>
#include "userlist.h"

#ifdef __cplusplus
extern "C"{
#endif

/*
 *call back function point
 *
 *回调函数类型
*/
typedef void ZH_ON_ACCEPT (TzhNetSession *sion,void* info);
typedef void ZH_ON_DISCONNECT (TzhNetSession *sion,void* info);
typedef void ZH_ON_RECEIVE (TzhNetSession *sion,void* info,unsigned char*szBuf,int nLen);
typedef void ZH_ON_ERROR (TzhNetSession *sion,void* info,EzhNetError err);
typedef void ZH_ON_REALTIME (TzhNetSession *sion,void* info);


//
//initizal server function
//
//初始化服务器功能
//
void zhInitTcpProc(unsigned short		wBindPort,
				    bool				isVariableFlowEncrypt,
				    ZH_ON_ACCEPT		*pfAccept,
					ZH_ON_DISCONNECT	*pfDisconnect,
					ZH_ON_RECEIVE		*pfRecive,
					ZH_ON_ERROR			*pfError,
					ZH_ON_REALTIME		*pfRealTime);



//
//send message to all sessions
//
//发送消息到所有连接上的客户端
//
void zhSendAllUser(TzhPacket*pack);

#ifdef __cplusplus
}
#endif
#define _ZH_NET_PROCESS_TCP_H_
#endif
