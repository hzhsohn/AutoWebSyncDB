#include "userlist.h"


///////////////////////////////// TagUserNode  Begin/////////////////////////////////////////
TagUserNode *UserFind(TzhNetList *pUserList, TzhNetSession sion)
{
	TagUserNode *pUser;

	for(pUser = (TagUserNode *)zhNetListFirst(pUserList); pUser != NULL;pUser = (TagUserNode *)zhNetListNext((TzhNetNode *)pUser))
	{
		if(pUser->sion.s == sion.s)
		{return pUser;}
	}
	return NULL;
}

TagUserNode * UserAdd(TzhNetList *pUserList, TzhNetSession sion)
{
	TagUserNode *pUser=NULL;
	if(UserFind(pUserList, sion) != NULL)
	{
		printf("TagUserNode: Session %d already exist\n", sion.s);
		goto end;
	}
	
	pUser = (TagUserNode *)calloc(1, sizeof(TagUserNode));

	if(pUser == NULL) {
		printf("TagUserNode: error to get memory\n");
		goto end;
	}

	pUser->sion=sion;
	zhNetListAdd(pUserList, (TzhNetNode *)pUser);

end:	
	return pUser;
}

TagUserNode *UserDelete(TzhNetList *pUserList, TzhNetSession sion)
{
	TagUserNode *pUser;
	TagUserNode *pNew=0;

	pUser = UserFind(pUserList, sion);

	if(pUser == NULL)
	{
		printf("Session: no socket=%d %s\n", sion.s);
		goto end;
	}
	pNew=(TagUserNode *)zhNetListNext((TzhNetNode *)pUser);
	zhNetListDelete(pUserList, (TzhNetNode *)pUser);
	free(pUser);

end:
	return pNew;
}

///////////////////////////////// TagUserNode  End/////////////////////////////////////////
/////////////////////////////// TagUserListNode Begin//////////////////////////////////////
TagUserListNode *UserListFind(TzhNetList *pUserNodeList, TagUserNode *user_node)
{
	TagUserListNode *pUserListNode;

	for(pUserListNode = (TagUserListNode *)zhNetListFirst(pUserNodeList); pUserListNode != NULL;pUserListNode = (TagUserListNode *)zhNetListNext((TzhNetNode *)pUserListNode))
	{
		if(pUserListNode->user_node== user_node)
		{return pUserListNode;}
	}
	return NULL;
}

TagUserListNode *UserListAdd(TzhNetList *pUserNodeList, TagUserNode *user_node)
{
	TagUserListNode *pUserNode=NULL;
	if(UserListFind(pUserNodeList, user_node) != NULL)
	{
		printf("TagUserListNode: user_node %p already exist\n", user_node);
		goto end;
	}
	
	pUserNode = (TagUserListNode *)calloc(1, sizeof(TagUserListNode));

	if(pUserNode == NULL) {
		printf("TagUserListNode: error to get memory\n");
		goto end;
	}

	pUserNode->user_node=user_node;
	zhNetListAdd(pUserNodeList, (TzhNetNode *)pUserNode);

end:	
	return pUserNode;
}
TagUserListNode *UserListDelete(TzhNetList *pUserNodeList, TagUserNode *user_node)
{
	TagUserListNode *pUserNode;
	TagUserListNode *pNew=0;

	pUserNode = UserListFind(pUserNodeList, user_node);

	if(pUserNode == NULL)
	{
		printf("Session: no socket=%d %s\n", user_node->sion.s);
		goto end;
	}
	pNew=(TagUserListNode *)zhNetListNext((TzhNetNode *)pUserNode);
	zhNetListDelete(pUserNodeList, (TzhNetNode *)pUserNode);
	free(pUserNode);

end:
	return pNew;
}
/////////////////////////////// TagUserListNode End //////////////////////////////////////
/////////////////////////////// TagThreadNode Begin //////////////////////////////////////

TagThreadNode *ThreadAdd(TzhNetList *pThreadList)
{
	TagThreadNode *pThreadNode=NULL;

	pThreadNode = (TagThreadNode *)calloc(1, sizeof(TagThreadNode));

	if(pThreadNode == NULL) {
		printf("TagUserListNode: error to get memory\n");
		goto end;
	}
	pThreadNode->bThread=true;
	pThreadNode->dwSleepTime=zhPlatGetTime();
	
	zhNetListAdd(pThreadList, (TzhNetNode *)pThreadNode);

end:
	return pThreadNode;
}
TagThreadNode *ThreadDelete(TzhNetList *pThreadList,TagThreadNode*thread_node)
{
	TagThreadNode *pNew=0;
	pNew=(TagThreadNode *)zhNetListNext((TzhNetNode *)thread_node);
	zhNetListDelete(pThreadList, (TzhNetNode *)thread_node);
	free(thread_node);
	return pNew;
}

/////////////////////////////// TagThreadNode End //////////////////////////////////////
