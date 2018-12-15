#include "StdAfx.h"
#include "GCHttp.h"

GCHttp::GCHttp()
{
}

GCHttp::~GCHttp(void)
{
}

bool GCHttp::InitConnect()
{
	DWORD dwFlags;
	TCHAR strAgent[64];

	InternetGetConnectedState(&dwFlags,0);

	_stprintf(strAgent,_T("Agent%ld"),GetTickCount());
	if(!(dwFlags&INTERNET_CONNECTION_PROXY))
	{
		m_open=InternetOpen(strAgent,INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY,NULL,NULL,0);
	}
	else
	{
		m_open=InternetOpen(strAgent,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
	}
	if(!m_open)
		return false;
	return true;
}

/*
	0=success
	1=connect err
	2=recv err
	3=cancel download
*/
int GCHttp::GetFileBuf(TCHAR*szUrl,BYTE*szdestbuf,DWORD dwbufsize,DWORD BeginByte)
{
	int ret;
	TCHAR szHead[512];

	ret=0;
	memset(szdestbuf,0,dwbufsize);
	memset(szHead,0,sizeof(szHead));
	_stprintf(szHead,_T("Accept: */*\r\n\r\nRange:bytes=%u-\r\n\r\n"),BeginByte);

	if(!(m_connect=InternetOpenUrl(m_open,szUrl,szHead,lstrlen(szHead),INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_PRAGMA_NOCACHE|INTERNET_FLAG_RELOAD,0)))
	{
		return 1;//连接出错
	}

	DWORD dwByteToRead=0;
	DWORD dwSizeOfRq=4;
	//查询远程文件长度
	if(!HttpQueryInfo(m_connect,HTTP_QUERY_CONTENT_LENGTH|HTTP_QUERY_FLAG_NUMBER,(LPVOID)&dwByteToRead,&dwSizeOfRq,NULL))
	{
		dwByteToRead=0;
	}

	
	// 开始下载文件
	BYTE strBuffer[2048];
	DWORD dwSize,dwTmpSize;
	DWORD recvSize=0;
	do
	{
		memset(strBuffer,0,sizeof(strBuffer));
		if(!InternetReadFile(m_connect,strBuffer,sizeof(strBuffer),&dwSize))
		{
			ret=2;
			break;//下载出错
		}

		if(0==dwSize) //下载完成
		{
			break;
		}
		else
		{
			if(recvSize>=dwbufsize)break;
			dwTmpSize=dwbufsize-recvSize;
			if(dwTmpSize<dwSize)
			{
				for(int i=0;i<dwTmpSize;i++)
				{
					szdestbuf[recvSize]=strBuffer[i];
					recvSize++;
				}
			}
			else
			{
				memcpy(&szdestbuf[recvSize],strBuffer,dwSize);
				recvSize+=dwSize;
			}
		}
	}while(true);

	InternetCloseHandle(m_connect);

    return ret;
}
