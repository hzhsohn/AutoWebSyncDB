#ifndef __assist_H__

#pragma once


#include "tchar.h"


class assist
{
public:
	assist(void);
	~assist(void);

	static TCHAR* assist::getCurrentPath(TCHAR*filename,TCHAR*dst_buf);
};

#define __assist_H__
#endif