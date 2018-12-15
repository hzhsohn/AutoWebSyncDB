//////////////////////////////////////////////////////////////////////////////////////////////////

//WINDOWS×¨ÓÃ
#pragma once

#ifndef __StringUtil_H_

#include "stdafx.h"
#include <string>

using namespace std;


class StringUtil
{
public:
    static string ws2s(const wstring& ws)
    {
        const wchar_t* _Source = ws.c_str();
        size_t _Dsize = WideCharToMultiByte(CP_ACP, 0, _Source, -1, NULL, 0, NULL, NULL);
        char *_Dest = new char[_Dsize];
        WideCharToMultiByte(CP_ACP, 0, _Source, -1, _Dest, _Dsize, NULL, NULL);
        string result = _Dest;
        delete []_Dest;
        return result;
    }
    static wstring s2ws(const string& s)
    {
        const char* _Source = s.c_str();
        size_t _Dsize = MultiByteToWideChar(CP_ACP, 0, _Source, -1, NULL, 0);
        wchar_t *_Dest = new wchar_t[_Dsize];
        MultiByteToWideChar(CP_ACP, 0, _Source, -1, _Dest, _Dsize);
        wstring result = _Dest;
        delete []_Dest;
        return result;
    }
    static string ws2utf8(const wstring& ws)
    {
        const wchar_t* _Source = ws.c_str();
        size_t _Dsize = WideCharToMultiByte(CP_UTF8, 0, _Source, -1, NULL, 0, NULL, NULL);
        char *_Dest = new char[_Dsize];
        WideCharToMultiByte(CP_UTF8, 0, _Source, -1, _Dest, _Dsize, NULL, NULL);
        string result = _Dest;
        delete []_Dest;
        return result;
    }
    static wstring utf82ws(const string& s)
    {
        const char* _Source = s.c_str();
        size_t _Dsize = MultiByteToWideChar(CP_UTF8, 0, _Source, -1, NULL, 0);
        wchar_t *_Dest = new wchar_t[_Dsize];
        MultiByteToWideChar(CP_UTF8, 0, _Source, -1, _Dest, _Dsize);
        wstring result = _Dest;
        delete []_Dest;
        return result;
    }
};

#ifdef _UNICODE
 #define _WS2S_CSTR(str)  StringUtil::ws2s(str).c_str()
 #define _S2WS_CSTR(str)  StringUtil::s2ws(str).c_str()
#else
 #define _WS2S_CSTR(str)  str 
 #define _S2WS_CSTR(str)  str 
#endif

#define __StringUtil_H_
#endif
