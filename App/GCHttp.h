#pragma once

#ifndef _CHttpGetFile_H
#include <windows.h>
#include <tchar.h> 
#include <iostream> 
#include <winInet.h>
#include <winsock.h> 

using namespace std;
#pragma comment(lib,"Ws2_32.lib") 
#pragma comment(lib,"winInet.lib")

//�ٽ���
#define TYPE_CS			CRITICAL_SECTION 
#define LOCK_CS(p)		EnterCriticalSection(p)	
#define UNLOCK_CS(p)		LeaveCriticalSection(p)
#define INIT_CS(p)		InitializeCriticalSection(p)
#define DELETE_CS(p)		DeleteCriticalSection(p)


class GCHttp
{
private:
	HINTERNET m_open;
	HINTERNET m_connect;


public:
	GCHttp();
	virtual ~GCHttp(void);


	/*
		InitConnect��������˵��
	*/
	bool InitConnect();

	/*
		GetFileBuf�����ķ���ֵ
		0=success
		1=connect err
		2=recv err
		3=cancel download
		//BeginByte������Ƿ�����Ч���ÿ��������Ƿ�֧���������
	*/
	int GetFileBuf(TCHAR*szUrl,BYTE*szdestbuf,DWORD dwbufsize,DWORD BeginByte=0);
};

#define _CHttpGetFile_H
#endif
