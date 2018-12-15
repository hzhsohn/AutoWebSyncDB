/* 
  Copyright (C) 2008-2, Han.zhihong, Developer. Created 2008. 
  All rights reserved.

  License to copy and use this software is granted provided that it
  is identified as the "Han.zhihong. Message-Digest
  Algorithm" in all material mentioning or referencing this software
  or this function.
  
  Han.zhihong. makes no representations concerning either
  the merchantability of this software or the suitability of this
  software for any particular purpose. It is provided "as is"
  without express or implied warranty of any kind.
	  
  These notices must be retained in any copies of any part of this
  documentation and/or software.


  ==================================================================
  session.h:the connect session manage

  帧结构:
	unsigned char   yFrameFlag;	//固定 ZH_NET_FRAME_FLAG
	unsigned short	wDataLen;   //后随的包体的长度
	unsigned short	wCRC16;     //包体的CRC校检,当包长度为0时CRC校检值为0
	unsigned char	...;		//根据wDataLen而定的包体


  具体结构体定义在gcdef.h头文件里面
*/

#pragma once

#ifndef __ZH_C_2_SESSION_H_


#ifdef __cplusplus
extern "C"{
#endif

#include "encrypt.h"
#include "socket.h"
#include "packet.h"
#include "gcdef.h"

/*
 *error message
 *
 *错误信息
*/
typedef enum _EzhNetError
{
	//没有错误
	ezhNetNoError				,
	//收到数据包出错
	ezhNetErrorPacketInvalid	,
	//SOCKET出错
	ezhNetErrorSocket			,
	//缓冲区溢出
	ezhNetErrorCacheOverflow	,
	//CRC校检出错
	ezhNetErrorCRC16			
}EzhNetError;

/*
 *session's net event
 *
 *SESSION会话的网络的事件
*/
typedef enum _EzhNetEvent
{
	//没有事件
	ezhNetNoEvent				,
	//连接成功
	ezhNetEventConnected		,
	//连接失败
	ezhNetEventConnectTimeout	,
	//断开连接
	ezhNetEventDisconnect		,		
}EzhNetEvent;

/*
 *send data list struct
 *
 *发送链表的结构
*/
typedef struct _TzhNetPacketList
{
	char packet[sizeof(TzhPackFrame)];  //数据包
	int packet_len;      //帧长度
	int send_pos;		//发送的开始位置,与帧长度一样,代表已经处理完此记录

	struct _TzhNetPacketList *_next;
}TzhNetPacketList;

/*
 *connect timeout value
 *
 *连接超时时间
*/
#define ZH_NET_CONNECT_TIMEOUT		15*1000

/*
 *the stick package management buffer
 *
 *粘包处理缓冲区
*/
#define ZH_NET_PACKET_SIZE			sizeof(TzhPackFrame)*2


/*
 *enum session status
 *
 *枚举会话状态
*/
typedef enum _EzhNetState {
	ezhNetStateZero			=0,
	ezhNetStateInitizal		=1,
	ezhNetStateWaitSeed		=2,
	ezhNetStateConnecting	=3,
	ezhNetStateConnected	=4,
	ezhNetStateDead			=5
}EzhNetState;

/*
 *enum packet/cache/buffer size
 *
 *枚举缓冲区大小
*/
typedef enum _EzhPackCacheType {
	ezhPackCacheDefault=1024*8,
	ezhPackCache10K=1024*10,
	ezhPackCache30K=1024*30,
	ezhPackCache50K=1024*50,
	ezhPackCache64K=1024*64
}EzhPackCacheType;

/*
 *session packet logic manage 
 *
 *会话层数据包管理
*/
typedef struct _TzhNetBPack
{
	bool					bNetPackRecvBuf;
	unsigned short					wNetPackPos;
	unsigned char					btCache[ZH_NET_PACKET_SIZE];
}TzhNetBPack;

/*
 *listern subject
 *
 *监听的结构
*/
typedef struct _TzhNetListen
{
	SOCKET		s;
	/*
	 *encrypt key and type,using for the transmission logic
	 *
	 *加密钥匙和加密类型
	*/
	unsigned int			nEncryptKey;
	bool		isStartupVariableFlowEncrypt;
}TzhNetListen;

/*
 *session subject
 *
 *单个会话结构
*/
typedef struct _TzhNetSession
{
	SOCKET		s;

	/*session status*/
	EzhNetState	cState;
	
	/*
	 *encrypt key and type,using for the transmission logic
	 *
	 *加密钥匙和加密类型
	*/
	unsigned int			nEncryptKey;
	bool		isStartupVariableFlowEncrypt;
	
	/*
	 *check encrypt
	 *
	 *标记客户是否已经获取加密种子
	*/
	bool isAlreadyGetEncryptSeed;

	/*
	 *begin connect time
	 *
	 *开始连接的时间
	*/
	time_t		dwStartTime;

	/*
	 *stick packet processing
	 *
	 *解决粘包
	*/
	TzhNetBPack			tagPack;

	/*
	 *infomation point
	 *
	 *资料结构指针
	*/
	void			*pInfo;

	/*
	 *send data list
	 *
	 *发送数据的排列的链表
	*/
	TYPE_CS csSendData;                   //发送列表的互斥锁
	TzhNetPacketList *sendDataList;       //发送列表
	TzhNetPacketList *pSendDataList_last; //发送列表的最后一个元素指针
	int sendDataListCount;		  //发送列表的数量

}TzhNetSession;

/*
 *server port for initizal bind and listen function
 *
 *服务器监听用的初始化函数
 *GCHEM_Encrypt_Type type 连接使用的加密模式
 *encrypt_default_key 初始化加密的密码
*/
bool zhSionStartup(TzhNetListen* lisn,unsigned short wBindPort,bool isStartupVariableFlowEncrypt);

/*
 *client port for initizal function
 *
 *客户端用的初始化函数
*/
bool zhSionInit(TzhNetSession *sion,unsigned short wBindPort);

/*
 *set session info data
 *
 *设备session对应的数据结构体
*/
void zhSionSetInfo(TzhNetSession *sion,void* pInfo);


/*
 *client connect function,using by not blacking
 *
 *客户端连接函数,是非阻塞使用
 *
 *返回值
 *   true 为连接初始化成功
 *   false 初始连接失败
*/
bool zhSionConnect(TzhNetSession *sion,char*szIp,unsigned short wRemotoPort);

/*
 *server accept session event,cycle using 
 *
 *有新连接接入,循环使用
*/
bool zhSionAccept(TzhNetListen* lisn,TzhNetSession *sion);

/*
 *close session link
 *
 *关闭连接
*/
bool zhSionSafeClose(TzhNetSession *sion);

/*
 *setting socket transmission buffer size
 *
 *设置SOCKET的传输缓冲区大小
*/
void zhSionSetBigSockCache(TzhNetSession *sion,int buf_size);

/*
 *send data function
 *
 *发送函数,将数据包添加至发送链表
 *
 *返回值
 *  true 插入到发送链表成功
 *  false 插入到发送链表失败,一般是内存不足引起
*/
bool zhSionSend(TzhNetSession *sion,char*szPack,int nLen);
bool zhSionSendPacket(TzhNetSession *sion,TzhPacket*pack);

/*
 *将发送数据加到发送列表
 *
*/
bool zhSionInsertSendList(TzhNetSession *sion,char* yBuf,int nLen);

/*
 *剩余发送缓冲记录数量
 *
*/
int zhSionGetSurplusSendCount(TzhNetSession *sion);

/*
 *network system process,must be using in the program,cycle using
 *
 *接收数据到缓冲区的收集器,循环调用
 *
 *返回值
 *   false 处理失败
 *    true 正确 
*/
bool zhSionCacheData(TzhNetSession *sion,EzhNetError* err);

/*
 *
 *处理数据缓冲区的功能,如果不调用zhSionCacheData,这函数永远不会有数据返回
 *数据在返回到上zhSionReadData同一层时,已经经过过滤,校验,排除不合格的数据
 *
 *返回值
 *    -1= 有错误,
 *     0= 缓冲区数据不满一帧
 *     0> 返回帧大小
 *     frame 数据帧内容
 *     err 错误的消息
 *
 *例子:
 *     int ret;
 *     while(1)
 *     {
 *         ret=zhSionReadData(...);
 *		   if(0==ret)
 *		   { break; }
 *		   else if(-1==ret)
 *		   {
 *             switch(err)
 *             {...}
 *         }
 *		   else
 *		   { 处理frame数据... }
 *     }
*/
int zhSionReadData(TzhNetSession *sion,unsigned char *frame,EzhNetError* err);

/*
 *返回网络的事件
 * 
 *例子
 *    switch(zhSionStateThread(...))
 *    {...}
*/
EzhNetEvent zhSionStateThread(TzhNetSession *sion);

/*
 *get local ip and port for session
 *
 *从会话中获取本地IP和端口信息
*/
bool zhSionLocalAddr (TzhNetSession *sion,char *addr,unsigned short *port,unsigned int *ip);

/*
 *get remote socket ip and port for session
 *
 *从会话中获取对方IP和端口信息
*/
bool zhSionRemoteAddr(TzhNetSession *sion,char *addr,unsigned short *port,unsigned int *ip);


/*
 *logical function of the session layer, using private 
 *
 *会话层逻辑函数,对私使用
*/
void _zhSionSendKeyEncrypt(TzhNetSession *sion);

#ifdef __cplusplus
}
#endif

#define __ZH_C_2_SESSION_H_
#endif
