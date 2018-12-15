#include "Proc.h"
#include "Database.h"
#include "urlcode\encoding.h"
#include "c-json\js_parser.h"
#include "Net.h"

//初始化
TzhAutoWebAddr Proc::accessAddr;
GCHttp Proc::http;
TCHAR  Proc::ini_file[1024]={0};
TCHAR  Proc::db_file[1024]={0};
TCHAR Proc::doUrl[2048]={0};
int Proc::tcpPort=0;
BYTE Proc::szHttpRebackBuf[128]={0};

//--------------------------------------------
Proc::Proc(void)
{
}

Proc::~Proc(void)
{
}
 
void Proc::Init()
{
	//--------------------------------------------------------------
	//模拟服务的运行，10后自动退出。应用时将主要任务放于此即可
	assist::getCurrentPath(_T("AutoWebSyncDB.ini"),ini_file);
	
	if(0==_taccess(ini_file,0))
	{
		GetPrivateProfileString(_T("Address"),_T("url"),_T(""),accessAddr.url,1024,ini_file);
		accessAddr.interval_second=GetPrivateProfileInt(_T("Address"),_T("interval_second"),30,ini_file);
		GetPrivateProfileString(_T("Database"),_T("path"),_T(""),db_file,1024,ini_file);	
		tcpPort=GetPrivateProfileInt(_T("TCP"),_T("tcp_port"),0,ini_file);
	}
	else
	{
		TCHAR buf1[32];

		//默认生成数值
		_tcscpy(accessAddr.url,_T("http://wl.hx-kong.com/post.i.php"));
		accessAddr.interval_second=10;
		//生成默认值
		assist::getCurrentPath(_T("AutoWebSyncDB.db"),db_file);		
		WritePrivateProfileString(_T("Database"),_T("path"),db_file,ini_file); 
		//
		_itot(accessAddr.interval_second,buf1,10);
		WritePrivateProfileString(_T("Address"),_T("url"),accessAddr.url,ini_file);
		WritePrivateProfileString(_T("Address"),_T("interval_second"),buf1,ini_file);
		//
		tcpPort=7666;
		_itot(tcpPort,buf1,10);
		WritePrivateProfileString(_T("TCP"),_T("tcp_port"),buf1,ini_file);
	}
	if(0==tcpPort)
	{
		TCHAR buf1[32];
		//生成默认值
		tcpPort=7666;
		_itot(tcpPort,buf1,10);
		WritePrivateProfileString(_T("TCP"),_T("tcp_port"),buf1,ini_file);
	}
	if(0==_tcslen(db_file))
	{
		//生成默认值
		assist::getCurrentPath(_T("AutoWebSyncDB.db"),db_file);		
		WritePrivateProfileString(_T("Database"),_T("path"),db_file,ini_file); 
	}
	if(0!=_taccess(db_file,0))
	{
		sqlite3 *db;
		char *zErrMsg = 0;
		int  rc;
		int key=0;
		const char *sql ;
		string temp;
		
		//更新INI
		WritePrivateProfileString(_T("Database"),_T("path"),db_file,ini_file); 

		/* Open database */
		rc = sqlite3_open(_WS2S_CSTR(db_file), &db);
		if (rc){
			cout <<  "Can't open database "<<db_file << endl;
			sqlite3_free(zErrMsg);
		}
		else{
			cout << "open the database successful!" << endl;
		}
 
		// Create table statement	
		sql = "CREATE TABLE table1(autoid integer PRIMARY KEY autoincrement,key varchar(128) NOT NULL,value TEXT NOT NULL,is_sync tinyint default 0);";
		rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
		if (rc != SQLITE_OK){
			cout << zErrMsg << endl;
			sqlite3_free(zErrMsg);
		}
		else{
			cout << "create table successful!" << endl;
		}
		sqlite3_close(db);
	}

	//初始化检测
	if(0==_tcslen(accessAddr.url))
	{
		cout << "err addrcess.url !!!" << endl;
		exit(0);
	}
	if(0==accessAddr.interval_second)
	{
		cout << "err addrcess.interval_second !!! not allow is 0" << endl;
		exit(0);
	}
	_tcscpy(Database::db_file , Proc::db_file);
	//访问网络
	http.InitConnect();
	//数据通讯	
	zhInitTcpProc(tcpPort,true,zhAccept,zhDisconnect,zhRecv,zhError,zhRealTime);
}
//------------------------------------------------------------

BOOL Proc::Loop()
{
	//定时读取数据库
	if(time(NULL) - accessAddr.dwOldTime >= accessAddr.interval_second)
	{
		char gkeyName[100]={0};
		accessAddr.dwOldTime=time(NULL);
		
_redonnc:
		string json=Database::readUnUpdateTop1(gkeyName);
		if(json.length()>0)
		{
			char *enjson;int k;
			char*szb=(char*)json.c_str();
			//加码
			enjson=urlencode(szb,strlen(szb),&k);

			//循环读取数据库内未提交的数据
			memset(szHttpRebackBuf,0,128);
			_stprintf(doUrl,_T("%s?%s"),accessAddr.url,_S2WS_CSTR(enjson));

			//提交到服务器
			http.GetFileBuf(doUrl,szHttpRebackBuf,128,0);
			
			//-----------------------
			//解释返回来网络结果
			PJOBJECT root = NULL;
			int i = 0;
			PJVALUE jsonval, value2; PJSTRING str;
			Js_parser_object((void**)&root, (char*)szHttpRebackBuf, &i);

			jsonval = (PJVALUE)Js_object_get_value(root, "ret");
			if(jsonval)
			{
				if(JS_STRING==jsonval->value_type)
				{
					str = (PJSTRING)jsonval->value_data;	//array1
					if(0==strncmp("ok", str->str_data, str->str_len))
					{
						printf("upload success content=%s\n",json.c_str());
						Database::setDBUpdateSuccess(gkeyName);
						goto _redonnc;
					}
					if(0==strncmp("invalid", str->str_data, str->str_len))
					{
						Database::setDBUpdateInvalidKey(gkeyName);
					}
					else //不等于OK即为失败
					{
						printf("submit fail content=%s\n",json.c_str());
					}
				}			
				Js_parser_object_free(root);
			}
			else
			{
				MessageBox(0,_T("upload reback json data error"),_T("error"),0);
			}
			//--------------------------			
			free(enjson);
			enjson=NULL;

		}
	}

	Sleep(1);
	return TRUE;
}
void Proc::End()
{
	
}