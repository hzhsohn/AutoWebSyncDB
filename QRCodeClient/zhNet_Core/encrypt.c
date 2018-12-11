/* encrypt.c - Data Security, custom message-digest algorithm
*/

/* Copyright (C) 2008-2, Han.zhihong, Developer. Created 2008. All
rights reserved.

  License to copy and use this software is granted provided that it
  is identified as the "Han.zhihong. Message-Digest
  Algorithm" in all material mentioning or referencing this software
  or this function.
  
  Han.zhihong. makes no representations concerning either
  the merchantability of this software or the suitability of this
  software for any particular purpose. It is provided "as is"
  without express or implied warranty of any kind.
	  
  These notices must be retained in any copies of any part of this
  documentation and/or software.
*/

#include "encrypt.h"

//
//Self-defined changes in the encrypted sequence
//
//自定义加密的变化序列
//
#define GCHMAC_KEY_CHANGE		37


//
//Initial start random key
//
//初始始化随机钥匙
//
unsigned int zhNetGetRandEncryptKey()
{
	time_t dw;
	int ret;
	dw=zhPlatGetTime();
	srand((unsigned int)dw);
	ret=rand();
	while(ret == 0)
	{
		dw=zhPlatGetTime();
		srand((unsigned int)dw);
		ret = rand();
	}
	return ret;
}

//
//sub function,using in this moudle
//function for encrypt data
//
bool zhNetEncrypt(bool isNeedEncrypt,char* buf, int len,unsigned int nEncryptKey)
{
	if(isNeedEncrypt)
	{
		int i,j;
		unsigned char yar[4];

		for(i=0; i<len; i++)
		{
			nEncryptKey += i*GCHMAC_KEY_CHANGE;
			memcpy(yar, &nEncryptKey, sizeof(int));
			for(j=0; j<4; j++)
			{
				buf[i] = buf[i]^yar[j];
			}
		}
	}
	return true;
}

//
//sub function,using in this moudle
//decrypt data
//
bool zhNetDecrypt(bool isNeedEncrypt,char* buf, int len,unsigned int nEncryptKey)
{
	if(isNeedEncrypt)
	{
		int i,j;
		unsigned char yar[4];

		for(i=0; i<len; i++)
		{
			nEncryptKey += i*GCHMAC_KEY_CHANGE;
			memcpy(yar, &nEncryptKey, sizeof(int));	
			for(j=3; j>=0; j--)
			{
				buf[i] = buf[i]^yar[j];
			}
		}
	}
	return true;
}
