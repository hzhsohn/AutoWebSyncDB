#include "StdAfx.h"
#include "assist.h"


assist::assist(void)
{
}


assist::~assist(void)
{
}


TCHAR* assist::getCurrentPath(TCHAR*filename,TCHAR*dst_buf)
{
  TCHAR ssspath[1024]={0};
 #ifdef _WIN32
  GetModuleFileName(NULL,ssspath,sizeof(ssspath)); 
  (_tcsrchr(ssspath, '\\'))[1] = 0; //ɾ���ļ�����ֻ���·��
 #else
  //ͷ�ļ���direct.h��TC2.0��Ϊdir.h��
  getcwd(ssspath, sizeof(ssspath));
  strcat(ssspath,"/");
 #endif

 if(filename)
 {
	 _tcsset(dst_buf,0);
	 _stprintf(dst_buf,_T("%s%s"),ssspath,filename); 
 }
 return dst_buf;
}
