#ifndef __ZH_TAGNODE_NET_ZHLIST_H_

#ifdef __cplusplus
extern "C"{
#endif

/*
 *doubly linked list node struct
 *
 *双向链表节点
*/
typedef struct _TzhNetNode
{
	struct _TzhNetNode *next;
	struct _TzhNetNode *prev;
}TzhNetNode;

/*
 *doubly linked list header struct
 *
 *双向链表头
*/
typedef struct _TzhNetList
{
	TzhNetNode node;
	int count;
}TzhNetList;

//
//doubly linked function
//
//双向链表处理函数
//
TzhNetNode *zhNetListFirst(TzhNetList *pList);
TzhNetNode *zhNetListLast(TzhNetList *pList);
TzhNetNode *zhNetListNext(TzhNetNode *pNode);
TzhNetNode *zhNetListPrev(TzhNetNode *pNode);
int zhNetListCount(TzhNetList *pList);
int zhNetListFind(TzhNetList *pList, TzhNetNode *pNode);
void zhNetListAdd(TzhNetList *pList, TzhNetNode *pNode);
void zhNetListDelete(TzhNetList *pList, TzhNetNode *pNode);
void zhNetListFree(TzhNetList *pList);
void zhNetListInit(TzhNetList *pList);
void zhNetListInsert(TzhNetList *pList, TzhNetNode *pPrev, TzhNetNode *pNode);

#ifdef __cplusplus
}
#endif

#define __ZH_TAGNODE_NET_ZHLIST_H_
#endif


