#include "tcp_serv_select.h"
#include "session.h"


#define TCP_PRINT_LOG		PRINTF

TzhNetListen g_listern;
TYPE_CS g_userCs;
TzhNetList g_userList;//唯一用户链表
TYPE_CS g_threadCs;
TzhNetList g_threadList;//线程处理组的数据

time_t g_dwNewTime;
time_t g_dwOldTime;

//回调
ZH_ON_ACCEPT		*g_pfAccept=NULL;
ZH_ON_DISCONNECT	*g_pfDisconnect=NULL;
ZH_ON_RECEIVE		*g_pfReceive=NULL;
ZH_ON_ERROR			*g_pfError=NULL;
ZH_ON_REALTIME		*g_pfRealTime=NULL;

/////////////////////////////////////////////////////////////////////////
//
//add client session to the session list
//
//将客户端的会话添加到SOCKET链表里
//
TagUserNode* ConnectAdd(TzhNetSession tmpSion);

//
//collection of multiple threads processing linked list,
//every 64 sockets in the chain will create a new thread processing
//
//多个线程处理链表集合,在链表里每达到64个socket就会新建一个线程进行处理
//
void ProcThread(TagThreadNode *pThreadNode);

/////////////////////////////////////////////////////////////////////////////////////////
TagUserNode* ConnectAdd(TzhNetSession tmpSion)
{
	TagThreadNode *pThreadNode;
	TagUserNode*node;

	LOCK_CS(&g_userCs);
	LOCK_CS(&g_threadCs);
	node=UserAdd(&g_userList,tmpSion);
	if(!node)
	{
		goto _end;
	}
	//初始化用户资料
	initizalUser(&node->userinfo);
	//附值结构
	zhSionSetInfo(&node->sion,(void*)&node->userinfo);
	for(pThreadNode = (TagThreadNode *)zhNetListFirst(&g_threadList); 
		pThreadNode != NULL;
		pThreadNode = (TagThreadNode *)zhNetListNext((TzhNetNode *)pThreadNode))
	{
		if(true==pThreadNode->bThread)
		{
			if(zhNetListCount(&pThreadNode->pUserListNodeList)<TCP_SPLIT_SIZE)
			{
				UserListAdd(&pThreadNode->pUserListNodeList,node);
				goto _end;
			}
		}
	}

	pThreadNode=ThreadAdd(&g_threadList);
	UserListAdd(&pThreadNode->pUserListNodeList,node);
	CREATE_THREAD(ProcThread,pThreadNode);		

_end:
	UNLOCK_CS(&g_threadCs);
	UNLOCK_CS(&g_userCs);
	TCP_PRINT_LOG("ClientCount=%d",zhNetListCount(&g_userList));
	return node;
}

void ProcThread(TagThreadNode *pThreadNode)
{
	time_t dwTmp;
	int nDataProc;
	TagUserListNode *pUser;
	if(pThreadNode)
	{
		TCP_PRINT_LOG("Create New Proc Thread... Current Count=%d",zhNetListCount(&g_threadList));
		while(pThreadNode->bThread)
		{
			nDataProc=0;

			LOCK_CS(&g_threadCs);
			for(pUser = (TagUserListNode *)zhNetListFirst(&pThreadNode->pUserListNodeList);pUser != NULL;)
			{
				if(pUser->user_node->sion.cState==ezhNetStateZero)
				{
					//执行所有连接的操作的时候记得要锁定 g_userCs 临界区,不然会有机会在某个
					//链接掉线的时候造成程序漰溃,或者遍历不完整
					LOCK_CS(&g_userCs);
					UserDelete(&g_userList,pUser->user_node->sion);
					pUser=UserListDelete(&pThreadNode->pUserListNodeList,pUser->user_node);
					TCP_PRINT_LOG("ClientCount=%d",zhNetListCount(&g_userList));
					UNLOCK_CS(&g_userCs);
				}
				else
				{
					//网络数据处理-------begin
					unsigned char frame[ZH_NET_PACKET_BODY_LENGTH];
					EzhNetError err;
					int ret;
					if(false==zhSionCacheData(&pUser->user_node->sion,&err))
					{
						g_pfError(&pUser->user_node->sion,&pUser->user_node->sion.pInfo,err);
					}
					while(1)
					{
						ret=zhSionReadData(&pUser->user_node->sion,frame,&err);
						if(0==ret)
						{ break; }
						else if(ret>0)
						{
							nDataProc++;
							//处理frame数据
							g_pfReceive(&pUser->user_node->sion,
								pUser->user_node->sion.pInfo,
								frame,
								ret);
						}
						else
						{
							g_pfError(&pUser->user_node->sion,
								pUser->user_node->sion.pInfo,
								err);
						}
					}
					switch(zhSionStateThread(&pUser->user_node->sion))
					{
							//连接成功
						case ezhNetEventConnected:
							break;
							//连接失败
						case ezhNetEventConnectTimeout:
							break;
							//断开连接
						case ezhNetEventDisconnect:
							g_pfDisconnect(&pUser->user_node->sion,
								pUser->user_node->sion.pInfo);
							break;
							//没有事件
						case ezhNetNoEvent:
							break;
					}
					//------------------end
					g_pfRealTime(&pUser->user_node->sion,pUser->user_node->sion.pInfo);//保活包超时检测
					pUser = (TagUserListNode *)zhNetListNext((TzhNetNode *)pUser);
				}
			}
			
			if(zhNetListCount(&pThreadNode->pUserListNodeList)<=0)
			{
				pThreadNode->bThread=false;
			}
			UNLOCK_CS(&g_threadCs);

			//如果连接300毫秒以上不休眠即执行一次,如果传输太频繁以确保其它线程正常运行
			//nDataProc如果有网络数据即不休眠
			dwTmp=zhPlatGetTime();
			if(0==nDataProc || dwTmp-pThreadNode->dwSleepTime>300)
			{
				pThreadNode->dwSleepTime=dwTmp;
				zhPlatSleep(1);
			}
		}
		LOCK_CS(&g_threadCs);
		ThreadDelete(&g_threadList,pThreadNode);
		pThreadNode=NULL;
		TCP_PRINT_LOG("Release Proc Thread... Current Count=%d",zhNetListCount(&g_threadList));
		UNLOCK_CS(&g_threadCs);
	}
}

void zhInitTcpProc(unsigned short		wBindPort,
				    bool				isVariableFlowEncrypt,
				    ZH_ON_ACCEPT		*pfAccept,
					ZH_ON_DISCONNECT	*pfDisconnect,
					ZH_ON_RECEIVE		*pfReceive,
					ZH_ON_ERROR			*pfError,
					ZH_ON_REALTIME		*pfRealTime)
{
	bool ret;

	g_pfAccept=pfAccept;
	g_pfDisconnect=pfDisconnect,
	g_pfReceive=pfReceive,
	g_pfError=pfError,
	g_pfRealTime=pfRealTime;

	INIT_CS(&g_threadCs);
	INIT_CS(&g_userCs);

	//初始化网络
	zhNetListInit(&g_userList);
	g_dwOldTime=zhPlatGetTime();

	ret=zhSionStartup(&g_listern,wBindPort,isVariableFlowEncrypt);
	if(ret)
	{
		TCP_PRINT_LOG("Startup Server ok..!!");
		TCP_PRINT_LOG("BindPort=%d",wBindPort);

	}
	else
	{TCP_PRINT_LOG("Startup Server Fail..!!");}
}

void zhTcpServ_loop()
{
	TzhNetSession tmpUser;
	//计算处理时间,在小于80毫秒内连续加入的连续都不需要1毫秒的延时
	g_dwNewTime=zhPlatGetTime();
	if(g_dwNewTime-g_dwOldTime>100){zhPlatSleep(1);}
		
	if(zhSionAccept(&g_listern,&tmpUser))
	{
		//可以在这里加入连接数量限制
		if(zhNetListCount(&g_userList)<TCP_MAX_CONNECTED)
		{
			TagUserNode*unode;
			//是否取用大SOCKET(64K)缓冲区,不取用不然有时候发送大于8K以上的包会丢失
			//如果是小型嵌入式设备3K就足够了,根据情况使用
			zhSionSetBigSockCache(&tmpUser,ezhPackCache64K);
			unode=ConnectAdd(tmpUser);
			TCP_PRINT_LOG("Session Connect, Socket=%d , dwStartTime=%d,user_count=%d",tmpUser.s,tmpUser.dwStartTime,zhNetListCount(&g_userList));
			g_dwOldTime=zhPlatGetTime();
			g_pfAccept(&unode->sion,unode->sion.pInfo);
		}
		else
		{
			TCP_PRINT_LOG("Session connected too much...");
			zhSionSafeClose(&tmpUser);
		}
	}
}

void zhTcpServ_free()
{	
	DELETE_CS(&g_threadCs);
	DELETE_CS(&g_userCs);
}

void zhSendAllUser(TzhPacket*pack)
{
	TagUserNode *pUser;
	for(pUser=(TagUserNode *)zhNetListFirst(&g_userList);pUser != NULL;pUser = (TagUserNode *)zhNetListNext((TzhNetNode *)pUser))
	{
		zhSionSend(&pUser->sion,(char*)pack->btBuf,pack->wSize);
	}
}
