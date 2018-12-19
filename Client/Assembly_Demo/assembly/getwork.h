#ifndef __zh_getpath_func_h__

#ifdef __cplusplus
extern "C"{
#endif

/*

//假设工作目录c:work
char buf[256];
getCurrentPath("fuckyou.txt",buf,sizeof(buf));   //输出 c:workfuckyou.txt
getWorkPath("../","yeah","fuckyou.txt",buf,sizeof(buf));    //输出 c:work../yeahfuckyou.txt
getWorkPath("/etc","yeah","fuckyou.txt",buf,sizeof(buf));    //输出 /etc/yeah/fuckyou.txt

*/
static char*getCurrentPath(char*filename,char*dst_buf,int dst_buf_len)
{

 #ifdef _WIN32
  GetModuleFileNameA(NULL,dst_buf,dst_buf_len); 
  (strrchr(dst_buf, '\\'))[1] = 0; //删除文件名，只获得路径
 #else
  //头文件：direct.h（TC2.0下为dir.h）
  getcwd(dst_buf, dst_buf_len);
  strcat(dst_buf,"/");
 #endif

 if(filename)
 { strcat_s(dst_buf,dst_buf_len,filename); }
 return dst_buf;
}

#ifdef __cplusplus
}
#endif

#define __zh_getpath_func_h__
#endif
