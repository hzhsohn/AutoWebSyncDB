#ifndef __USERINFO_H__
#ifdef __cplusplus
extern "C"{
#endif

#include "platform.h"

typedef struct _TagUserInfo{
	time_t dwKeepTime;
}TagUserInfo;

static void initizalUser(TagUserInfo*info)
{
	info->dwKeepTime=zhPlatGetTime();
}

#ifdef __cplusplus
}
#endif
#define __USERINFO_H__
#endif

