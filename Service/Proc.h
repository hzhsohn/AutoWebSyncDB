#pragma once
#include "StdAfx.h"
#include <stdio.h>
#include <string>
#include "sqlite/sqlite3.h"
#include "GCHttp.h"
#include "StringUtil.h"
#include "assist.h"
#include <time.h>
#include <vector>

using namespace std;

struct TzhAutoWebAddr
{
	TCHAR url[512];
	int interval_second;
	time_t dwOldTime;
};

class Proc
{
	static TzhAutoWebAddr accessAddr;
	static GCHttp http;
	static TCHAR ini_file[1024];
	static TCHAR doUrl[2048];
	static int tcpPort;
	static BYTE szHttpRebackBuf[512];
	//
	static BOOL isThreadRuning;
public:
	Proc(void);
	~Proc(void);

	//
	static void Init();
	static BOOL Loop();
	static void End();

	static void threadLoopDatabase();
};

