#ifndef __USERINFO_H__
#ifdef __cplusplus
extern "C"{
#endif

#include "platform.h"

typedef struct _TagUserInfo{
	time_t dwKeepTime;
	bool bLogin;
	char szAccount[32];
}TagUserInfo;

static void initizalUser(TagUserInfo*info)
{
	info->dwKeepTime=zhPlatGetTime();
	info->bLogin=false;
	memset(info->szAccount,0,sizeof(info->szAccount));
}

#ifdef __cplusplus
}
#endif
#define __USERINFO_H__
#endif

