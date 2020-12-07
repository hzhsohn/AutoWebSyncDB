#pragma once

#include "StdAfx.h"
#include <stdio.h>
#include <string>
#include "sqlite/sqlite3.h"
#include "GCHttp.h"
#include "StringUtil.h"
#include "assist.h"
#include <time.h>

class Database
{
public:
	Database(void);
	~Database(void);
		
	static TCHAR db_file[1024];
	
	//
	static void genDB();
	static BOOL writeDB(char*keyName,char*value);
	static void readDB(int start,int count);
	static string readUnUpdateTop1(char* getkeyName);
	static BOOL setDBUpdateInvalidKey(char* keyName);
	static BOOL setDBUpdateSuccess(char* keyName);
	static BOOL setDBUpdateExceptionKey(char* keyName);
};

