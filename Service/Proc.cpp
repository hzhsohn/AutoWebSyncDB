#include "Proc.h"
#include "Database.h"
#include "urlcode\encoding.h"
#include "c-json\js_parser.h"
#include "Net.h"
#include "zhlist\zhlist_exp.h"

//初始化
TzhAutoWebAddr Proc::accessAddr;
GCHttp Proc::http;
TCHAR  Proc::ini_file[1024]={0};
TCHAR  Proc::db_file[1024]={0};
TCHAR Proc::doUrl[2048]={0};
int Proc::tcpPort=0;
BYTE Proc::szHttpRebackBuf[512]={0};
BOOL Proc::isThreadRuning=FALSE;
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
		accessAddr.interval_second=20; //默认20秒
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
		//更新INI
		WritePrivateProfileString(_T("Database"),_T("path"),db_file,ini_file); 
	}
	//检测或生成数据库
	Database::genDB();

	//初始化检测
	if(0==_tcslen(accessAddr.url))
	{
		cout << "err addrcess.url is null !" << endl;
		cout <<  "exit(0) program ..." << endl;
		exit(0);
	}
	if(0==accessAddr.interval_second)
	{
		cout << "err addrcess.interval_second ! not allow is 0" << endl;
		cout <<  "exit(0) program ..." << endl;
		exit(0);
	}

	cout<<"URL=\""<<_WS2S_CSTR(accessAddr.url)<<"\""<<endl;
	cout<<"DB=\""<<_WS2S_CSTR(Proc::db_file)<<"\""<<endl;
	isThreadRuning=TRUE;
	_tcscpy(Database::db_file , Proc::db_file);
	//访问网络
	http.InitConnect();
	//数据通讯	
	zhInitTcpProc(tcpPort,true,zhAccept,zhDisconnect,zhRecv,zhError,zhRealTime);
	CREATE_THREAD(threadLoopDatabase,NULL);
}
//------------------------------------------------------------
BOOL Proc::Loop()
{
	//将JSON数据录入到数据库
	TzhListElementNode *eleNode;
	TzhList*lstJSONData;

	//----------------------
	//网络数据
	zhTcpServ_loop();
	
	//--------------------
	//链表数据
	lstJSONData=getJsonDataList();
    for(eleNode = (TzhListElementNode *)zhListFirst(lstJSONData);
          eleNode != NULL;)
    {
			TzhJSONData* p=(TzhJSONData*)eleNode->pElement;
			Database::writeDB(p->key,p->json);
			//删除链表节点
			eleNode=zhListNodeFreeAndDelete(lstJSONData,eleNode);
    }
	Sleep(1);
	return TRUE;
}
void Proc::End()
{
	isThreadRuning=FALSE;
	zhTcpServ_free();
}

void  Proc::threadLoopDatabase()
{
	while(isThreadRuning)
	{
		//--------------------
		//定时读取数据库
		if(time(NULL) - accessAddr.dwOldTime >= accessAddr.interval_second)
		{
			char gkeyName[100]={0};
			accessAddr.dwOldTime=time(NULL);
		
	_redonnc:
			Sleep(1);//防错延时
			string json=Database::readUnUpdateTop1(gkeyName);
			if(json.length()>0)
			{
				char *utf8c;
				char *enjson;int k;
				char*szb=(char*)json.c_str();
				int nnlen=strlen(szb)*5;
				utf8c=(char *)malloc(nnlen);
				memset(utf8c,0,nnlen);
				//编码转换
				Gb2312ToUtf8(szb,strlen(szb),utf8c);
				//加码
				enjson=urlencode(utf8c,strlen(utf8c),&k);
				free(utf8c);

				//循环读取数据库内未提交的数据
				memset(szHttpRebackBuf,0,128);
				_stprintf(doUrl,_T("%s?%s"),accessAddr.url,_S2WS_CSTR(enjson));

				//提交到服务器
				if(0!=http.GetFileBuf(doUrl,szHttpRebackBuf,512,0))
				{
					printf("[Error] -----------------------------------------------\r\n");
					printf("[Error] ------------- server access fail --------------\r\n");
					printf("[Error] ------------- server access fail --------------\r\n");
					printf("[Error] %s\r\n",_WS2S_CSTR(doUrl));
					printf("[Error] ------------- server access fail --------------\r\n");
					printf("[Error] ------------- server access fail --------------\r\n");
					printf("[Error] -----------------------------------------------\r\n");
					printf("Error:: Server access exception. json=%s\n",json.c_str());
					//upload reback json data error
					//继续处理
					Sleep(2000);
					goto _redonnc;
				}
				//-----------------------
				//解释返回来网络结果
				PJOBJECT root = NULL;
				int i = 0;
				PJVALUE jsonval; PJSTRING str;
				int pret=Js_parser_object((void**)&root, (char*)szHttpRebackBuf, &i);
				if(pret!=-1)
				{
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
									netSendCacheAllUserUploadResult(gkeyName,1);//成功
									//继续处理
									goto _redonnc;
								}
								else if(0==strncmp("invalid", str->str_data, str->str_len))
								{
									printf("upload invalid content=%s\n",json.c_str());
									Database::setDBUpdateInvalidKey(gkeyName);
									netSendCacheAllUserUploadResult(gkeyName,2);//无效scan_key
									//继续处理
									goto _redonnc;
								}
								else if(0==strncmp("repeat", str->str_data, str->str_len))
								{
									printf("upload repeat content=%s\n",json.c_str());
									Database::setDBUpdateSuccess(gkeyName);
									netSendCacheAllUserUploadResult(gkeyName,3);//数据内容重复
									//继续处理
									goto _redonnc;
								}
								else if(0==strncmp("json_error", str->str_data, str->str_len))
								{
									printf("[Error] -----------------------------------------------\r\n");
									printf("[Error] -----------------------------------------------\r\n");
									printf("[Error] upload json_error content=%s\n",json.c_str());
									Database::setDBUpdateExceptionKey(gkeyName);
									netSendCacheAllUserUploadResult(gkeyName,4);
									printf("[Error] -----------------------------------------------\r\n");
									printf("[Error] -----------------------------------------------\r\n");
									//继续处理
									goto _redonnc;
								}
								
								else //不等于OK即为失败
								{
									Database::setDBUpdateExceptionKey(gkeyName);
									printf("Error:: data error.submit fail content=%s\n",json.c_str());
									//回复数据异常
									//MessageBox(0,_S2WS_CSTR((char*)szHttpRebackBuf),_T("网络数据异常,请联系软件管理员"),0);
									netSendCacheAllUserUploadResult(gkeyName,0);
									//继续处理
									goto _redonnc;
								}
							}
							Js_parser_object_free(root);
						}
						//--------------------------			
						free(enjson);
						enjson=NULL;
				}
				else
				{
					printf("[Error] -----------------------------------------------\r\n");
					printf("[Error] -----------------------------------------------\r\n");
					printf("Error:: Server exception. json=%s\n",json.c_str());
					printf("Error:: Server exception. szHttpRebackBuf=%s\n",szHttpRebackBuf);
					printf("[Error] -----------------------------------------------\r\n");
					printf("[Error] -----------------------------------------------\r\n");
				}
			}
		}
	}
}