#ifndef _ZH_USERLIST_

#include "netlist.h"
#include "session.h"
#include "userinfo.h"

#ifdef __cplusplus
extern "C"{
#endif

	
/*
 *最大用户连接数量
*/
#define TCP_MAX_CONNECTED	1000

/*
 *一个线程包括多少个连接运行, TCP_MAX_CONNECTED/TCP_SPLIT_SIZE +主线程=最多线程数量
*/
#define TCP_SPLIT_SIZE		16


typedef struct _TagUserNode
{
	TzhNetNode node;
	TzhNetSession sion;
	TagUserInfo userinfo;
}TagUserNode;

TagUserNode *UserFind(TzhNetList *pUserList, TzhNetSession sion);
TagUserNode *UserAdd(TzhNetList *pUserList, TzhNetSession sion);
TagUserNode *UserDelete(TzhNetList *pUserList, TzhNetSession sion);

typedef struct _TagUserListNode
{
	TzhNetNode node;
	TagUserNode *user_node;
}TagUserListNode;

TagUserListNode *UserListFind(TzhNetList *pUserNodeList, TagUserNode *user_node);
TagUserListNode *UserListAdd(TzhNetList *pUserNodeList, TagUserNode *user_node);
TagUserListNode *UserListDelete(TzhNetList *pUserNodeList, TagUserNode *user_node);

typedef struct _TagThreadNode
{
	TzhNetNode node;
	bool bThread;
	time_t dwSleepTime;
	TzhNetList pUserListNodeList;//每TCP_SPLIT_SIZE个链接一组
}TagThreadNode;

TagThreadNode *ThreadAdd(TzhNetList *pThreadList);
TagThreadNode *ThreadDelete(TzhNetList *pThreadList,TagThreadNode*thread_node);

#ifdef __cplusplus
}
#endif
#define _ZH_USERLIST_
#endif
