#pragma once
#include <stdio.h>
#include <Windows.h>
#include <Winhttp.h>
#pragma comment(lib,"Winhttp.lib")
//�ο�����https://www.cnblogs.com/jkcx/p/6374026.html

typedef struct _URL_INFO
{
	WCHAR szScheme[512];
	WCHAR szHostName[512];
	WCHAR szUserName[512];
	WCHAR szPassword[512];
	WCHAR szUrlPath[512];
	WCHAR szExtraInfo[512];
}URL_INFO, *PURL_INFO;
typedef void(*DownLoadCallback)(int ContentSize, int CUR_LEN);
class CDlHelper
{
public:
	CDlHelper(CString& Url /*��ԴURL*/, CString& FileName /*�ļ���*/);
	~CDlHelper();

public:
	wchar_t m_wszUrl[256];
	wchar_t m_wszFileName[256];
	void download(DownLoadCallback Func);
};

