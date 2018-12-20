#include "Proc.h"
#include "Database.h"
#include "urlcode\encoding.h"
#include "c-json\js_parser.h"
#include "Net.h"
#include "zhlist\zhlist_exp.h"

//��ʼ��
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
	//ģ���������У�10���Զ��˳���Ӧ��ʱ����Ҫ������ڴ˼���
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

		//Ĭ��������ֵ
		_tcscpy(accessAddr.url,_T("http://wl.hx-kong.com/post.i.php"));
		accessAddr.interval_second=20; //Ĭ��20��
		//����Ĭ��ֵ
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
		//����Ĭ��ֵ
		tcpPort=7666;
		_itot(tcpPort,buf1,10);
		WritePrivateProfileString(_T("TCP"),_T("tcp_port"),buf1,ini_file);
	}
	if(0==_tcslen(db_file))
	{
		//����Ĭ��ֵ
		assist::getCurrentPath(_T("AutoWebSyncDB.db"),db_file);		
		WritePrivateProfileString(_T("Database"),_T("path"),db_file,ini_file); 
	}
	if(0!=_taccess(db_file,0))
	{
		//����INI
		WritePrivateProfileString(_T("Database"),_T("path"),db_file,ini_file); 
	}
	//�����������ݿ�
	Database::genDB();

	//��ʼ�����
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
	//��������
	http.InitConnect();
	//����ͨѶ	
	zhInitTcpProc(tcpPort,true,zhAccept,zhDisconnect,zhRecv,zhError,zhRealTime);
	CREATE_THREAD(threadLoopDatabase,NULL);
}
//------------------------------------------------------------
BOOL Proc::Loop()
{
	//��JSON����¼�뵽���ݿ�
	TzhListElementNode *eleNode;
	TzhList*lstJSONData;

	//----------------------
	//��������
	zhTcpServ_loop();
	
	//--------------------
	//��������
	lstJSONData=getJsonDataList();
    for(eleNode = (TzhListElementNode *)zhListFirst(lstJSONData);
          eleNode != NULL;)
    {
			TzhJSONData* p=(TzhJSONData*)eleNode->pElement;
			Database::writeDB(p->key,p->json);
			//ɾ������ڵ�
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
		//��ʱ��ȡ���ݿ�
		if(time(NULL) - accessAddr.dwOldTime >= accessAddr.interval_second)
		{
			char gkeyName[100]={0};
			accessAddr.dwOldTime=time(NULL);
		
	_redonnc:
			Sleep(1);//������ʱ
			string json=Database::readUnUpdateTop1(gkeyName);
			if(json.length()>0)
			{
				char *utf8c;
				char *enjson;int k;
				char*szb=(char*)json.c_str();
				int nnlen=strlen(szb)*5;
				utf8c=(char *)malloc(nnlen);
				memset(utf8c,0,nnlen);
				//����ת��
				Gb2312ToUtf8(szb,strlen(szb),utf8c);
				//����
				enjson=urlencode(utf8c,strlen(utf8c),&k);
				free(utf8c);

				//ѭ����ȡ���ݿ���δ�ύ������
				memset(szHttpRebackBuf,0,128);
				_stprintf(doUrl,_T("%s?%s"),accessAddr.url,_S2WS_CSTR(enjson));

				//�ύ��������
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
					//��������
					Sleep(2000);
					goto _redonnc;
				}
				//-----------------------
				//���ͷ�����������
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
									netSendCacheAllUserUploadResult(gkeyName,1);//�ɹ�
									//��������
									goto _redonnc;
								}
								else if(0==strncmp("invalid", str->str_data, str->str_len))
								{
									printf("upload invalid content=%s\n",json.c_str());
									Database::setDBUpdateInvalidKey(gkeyName);
									netSendCacheAllUserUploadResult(gkeyName,2);//��Чscan_key
									//��������
									goto _redonnc;
								}
								else if(0==strncmp("repeat", str->str_data, str->str_len))
								{
									printf("upload repeat content=%s\n",json.c_str());
									Database::setDBUpdateSuccess(gkeyName);
									netSendCacheAllUserUploadResult(gkeyName,3);//���������ظ�
									//��������
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
									//��������
									goto _redonnc;
								}
								
								else //������OK��Ϊʧ��
								{
									Database::setDBUpdateExceptionKey(gkeyName);
									printf("Error:: data error.submit fail content=%s\n",json.c_str());
									//�ظ������쳣
									//MessageBox(0,_S2WS_CSTR((char*)szHttpRebackBuf),_T("���������쳣,����ϵ�������Ա"),0);
									netSendCacheAllUserUploadResult(gkeyName,0);
									//��������
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