// StringUtil.cpp : 字符串工具类的实现文件
// 

#include "StdAfx.h"
#include "StringUtil.h"
#include "stdafx.h"
#include <string>
#include <iostream>
#include "windows.h"
#include <tchar.h>
void StringUtility::MyPrint(wchar_t * format, ...) {
    va_list args;
    va_start(args, format);
    wchar_t buf[512];
    _vswprintf(buf, format, args);
    OutputDebugString(buf);
    va_end(args);
}
wchar_t *StringUtility::HW_Utf8ToUnicodeW(const char *strSrc) {
    wchar_t  *strRtn = NULL;
    int   lLen = 0;
    if (strSrc == NULL) {
        return NULL;
    }
    for (;;) {
        lLen = MultiByteToWideChar(CP_UTF8, NULL, strSrc, (int)strlen(strSrc),
            NULL, NULL);
        strRtn = (wchar_t *)malloc(((unsigned int)lLen + 1) * sizeof(wchar_t));
        if (NULL == strRtn) {
            break;
        }
        memset(strRtn, 0, ((unsigned int)lLen + 1) * sizeof(wchar_t));
        if (0 != MultiByteToWideChar(CP_UTF8, NULL, strSrc, (int)strlen(strSrc),
            strRtn, lLen)) {
            strRtn[lLen] = L'\0';
        }
        break;
    }
    return strRtn;
}

char *StringUtility::HW_UnicodeToLocale(const wchar_t *strSrc) {
    char *strRtn = NULL;
    int lLen = 0;
    if (strSrc == NULL) {
        return NULL;
    }
    for (;;) {
        lLen = WideCharToMultiByte(CP_ACP, 0, strSrc, (int)wcslen(strSrc),
            NULL, 0, NULL, NULL);
        strRtn = (char *)malloc((unsigned int)lLen + 1);
        if (NULL == strRtn) {
            break;
        }
        memset(strRtn, 0, (unsigned int)lLen + 1);

        if (0 != WideCharToMultiByte(CP_ACP, 0, strSrc, (int)wcslen(strSrc),
            strRtn, lLen, NULL, NULL)) {
            strRtn[lLen] = '\0';
        }
        break;
    }
    return strRtn;
}


char *StringUtility::HW_Utf8ToLocale(const char *strSrc) {
    wchar_t *strRtn1 = NULL;
    char  *strRtn2 = NULL;
    if (strSrc == NULL) {
        return NULL;
    }
    strRtn1 = HW_Utf8ToUnicodeW(strSrc);
    strRtn2 = HW_UnicodeToLocale(strRtn1);
    free(strRtn1);
    strRtn1 = NULL;
    return strRtn2;
}

char *StringUtility::HW_UnicodeToUtf8(const wchar_t *strSrc) {
    char *strRtn = NULL;
    int lLen = 0;
    if (strSrc == NULL) {
        return NULL;
    }
    for (;;) {
        lLen = WideCharToMultiByte(CP_UTF8, NULL, strSrc, (int)wcslen(strSrc),
            NULL, 0, NULL, NULL);
        strRtn = (char *)malloc((unsigned int)lLen + 1);
        if (NULL == strRtn) {
            break;
        }
        (void)memset(strRtn, 0, (unsigned int)lLen + 1);
        if (0 != WideCharToMultiByte(CP_UTF8, NULL, strSrc, (int)wcslen(strSrc),
            strRtn, lLen, NULL, NULL)) {
            strRtn[lLen] = '\0';
        }
        break;
    }
    return strRtn;
}
wchar_t *StringUtility::HW_LocaleToUnicodeW(const char *strSrc) {
    wchar_t *strRtn = NULL;
    int  lLen = 0;
    if (strSrc == NULL) {
        return NULL;
    }
    (void)GetACP();
    for (;;) {
        lLen = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, strSrc, (int)strlen(strSrc),
            NULL, NULL);
        strRtn = (wchar_t *)malloc(((unsigned int)lLen + 1) * sizeof(wchar_t));
        if (NULL == strRtn) {
            break;
        }
        (void)memset(strRtn, 0, ((unsigned int)lLen + 1) * sizeof(wchar_t));
        if (0 != MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, strSrc, (int)strlen(strSrc),
            strRtn, lLen)) {
            strRtn[lLen] = L'\0';
        }
        break;
    }
    return strRtn;
}
char *StringUtility::HW_LocaleToUtf8(const char *strSrc) {
    wchar_t *strRtn1 = NULL;
    char  *strRtn2 = NULL;
    if (strSrc == NULL) {
        return NULL;
    }
    strRtn1 = HW_LocaleToUnicodeW(strSrc);
    strRtn2 = HW_UnicodeToUtf8(strRtn1);
    free(strRtn1);
    strRtn1 = NULL;
    return strRtn2;
}
