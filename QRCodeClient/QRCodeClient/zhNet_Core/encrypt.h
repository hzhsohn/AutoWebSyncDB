/* 
  encrypt.h - Data Security, custom message-digest algorithm
*/

/* 
  Copyright (C) 2008-2, Han.zhihong, Developer. Created 2008. 
  All rights reserved.

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

#pragma once
#include "platform.h"
#include "gcdef.h"

#ifndef __zh_encrypt_h__

#ifdef __cplusplus
extern "C"{
#endif

//
//Initial start random key
//
//初始始化随机钥匙
//
unsigned int zhNetGetRandEncryptKey();

//
//sub function,using in this moudle
//
bool zhNetEncrypt(bool isNeedEncrypt,char* buf, int len,unsigned int nEncryptKey);
bool zhNetDecrypt(bool isNeedEncrypt,char* buf, int len,unsigned int nEncryptKey);

#ifdef __cplusplus
}
#endif

#define __zh_encrypt_h__
#endif
