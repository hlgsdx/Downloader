#include "stdafx.h"
#include "DlHelper.h"

CDlHelper::CDlHelper(CString& Url /*��ԴURL*/, CString& FileName /*�ļ���*/)
{
	auto pwszUrl = Url.GetBuffer(Url.GetLength());
	auto pwszFileName = FileName.GetBuffer(FileName.GetLength());
	wcscpy_s(m_wszUrl, 256, pwszUrl);
	wcscpy_s(m_wszFileName, 256, pwszFileName);
	Url.ReleaseBuffer();
	FileName.ReleaseBuffer();
}

CDlHelper::~CDlHelper()
{
}

void CDlHelper::download(DownLoadCallback Func)
{

	URL_INFO url_info = { 0 };
	URL_COMPONENTSW lpUrlComponents = { 0 };
	lpUrlComponents.dwStructSize = sizeof(lpUrlComponents);
	lpUrlComponents.lpszExtraInfo = url_info.szExtraInfo;
	lpUrlComponents.lpszHostName = url_info.szHostName;
	lpUrlComponents.lpszPassword = url_info.szPassword;
	lpUrlComponents.lpszScheme = url_info.szScheme;
	lpUrlComponents.lpszUrlPath = url_info.szUrlPath;
	lpUrlComponents.lpszUserName = url_info.szUserName;

	lpUrlComponents.dwExtraInfoLength =
		lpUrlComponents.dwHostNameLength =
		lpUrlComponents.dwPasswordLength =
		lpUrlComponents.dwSchemeLength =
		lpUrlComponents.dwUrlPathLength =
		lpUrlComponents.dwUserNameLength = 512;
	//��URL��ַ������URL_COMPONENTS��
	WinHttpCrackUrl(m_wszUrl, 0, ICU_ESCAPE, &lpUrlComponents);

	// ����һ���Ự
	HINTERNET hSession = WinHttpOpen(NULL, WINHTTP_ACCESS_TYPE_NO_PROXY, NULL, NULL, 0);
	if(!hSession) {TRACE("!hSession");return;}
	DWORD dwReadBytes, dwSizeDW = sizeof(dwSizeDW), dwContentSize, dwIndex = 0;
	// ����һ������
	HINTERNET hConnect = WinHttpConnect(hSession, lpUrlComponents.lpszHostName, lpUrlComponents.nPort, 0);
	if(!hConnect) {TRACE("!hConnect");return;}
	// ����һ�������Ȳ�ѯ���ݵĴ�С
	HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"HEAD", lpUrlComponents.lpszUrlPath, L"HTTP/1.1", WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_REFRESH);
	if(!hRequest) {TRACE("!hRequest");return;}
	BOOL bSendRet = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
	if(!bSendRet) {TRACE("!bSendRet");return;}
	BOOL bRecvRet = WinHttpReceiveResponse(hRequest, 0);
	if(!bRecvRet) {
		DWORD ecode = GetLastError();
		TRACE("!bRecvRet:%d\n",ecode);
		return;
		}
	WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_CONTENT_LENGTH | WINHTTP_QUERY_FLAG_NUMBER, NULL, &dwContentSize, &dwSizeDW, &dwIndex);
	WinHttpCloseHandle(hRequest);

	// ����һ�����󣬻�ȡ����
	hRequest = WinHttpOpenRequest(hConnect, L"GET", lpUrlComponents.lpszUrlPath, L"HTTP/1.1", WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_REFRESH);
	WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
	WinHttpReceiveResponse(hRequest, 0);

	// �ֶλص���ʾ����
	DWORD BUF_LEN = 1024, ReadedLen = 0;
	BYTE *pBuffer = NULL;
	pBuffer = new BYTE[BUF_LEN];

	HANDLE hFile = CreateFileW(m_wszFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	while (dwContentSize > ReadedLen)
	{
		ZeroMemory(pBuffer, BUF_LEN);
		WinHttpReadData(hRequest, pBuffer, BUF_LEN, &dwReadBytes);
		ReadedLen += dwReadBytes;

		// д���ļ�
		WriteFile(hFile, pBuffer, dwReadBytes, &dwReadBytes, NULL);
		// ���Ȼص�
		Func(dwContentSize, ReadedLen);

	}

	CloseHandle(hFile);
	delete pBuffer;


	/*
	// һ����д�������ļ�
	BYTE *pBuffer = NULL;

	pBuffer = new BYTE[dwContentSize];
	ZeroMemory(pBuffer, dwContentSize);
	//do{
	WinHttpReadData(hRequest, pBuffer, dwContentSize, &dwReadBytes);
	Func(dwContentSize, dwReadBytes);
	//} while (dwReadBytes == 0);


	HANDLE hFile = CreateFileW(FileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(hFile, pBuffer, dwContentSize, &dwReadBytes, NULL);
	CloseHandle(hFile);

	delete pBuffer;
	*/
	WinHttpCloseHandle(hRequest);
	WinHttpCloseHandle(hConnect);
	WinHttpCloseHandle(hSession);
}