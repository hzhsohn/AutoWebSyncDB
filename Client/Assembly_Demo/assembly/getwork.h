#ifndef __zh_getpath_func_h__

#ifdef __cplusplus
extern "C"{
#endif

/*

//���蹤��Ŀ¼c:work
char buf[256];
getCurrentPath("fuckyou.txt",buf,sizeof(buf));   //��� c:workfuckyou.txt
getWorkPath("../","yeah","fuckyou.txt",buf,sizeof(buf));    //��� c:work../yeahfuckyou.txt
getWorkPath("/etc","yeah","fuckyou.txt",buf,sizeof(buf));    //��� /etc/yeah/fuckyou.txt

*/
static char*getCurrentPath(char*filename,char*dst_buf,int dst_buf_len)
{

 #ifdef _WIN32
  GetModuleFileNameA(NULL,dst_buf,dst_buf_len); 
  (strrchr(dst_buf, '\\'))[1] = 0; //ɾ���ļ�����ֻ���·��
 #else
  //ͷ�ļ���direct.h��TC2.0��Ϊdir.h��
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
