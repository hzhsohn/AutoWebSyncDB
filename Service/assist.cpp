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
  (_tcsrchr(ssspath, '\\'))[1] = 0; //删除文件名，只获得路径
 #else
  //头文件：direct.h（TC2.0下为dir.h）
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
