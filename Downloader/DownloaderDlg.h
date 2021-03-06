
// DownloaderDlg.h : 头文件
//

#pragma once
#include "MyThreadPool.h"

// CDownloaderDlg 对话框
class CDownloaderDlg : public CDialogEx
{
// 构造
public:
	CDownloaderDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DOWNLOADER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	static CDownloaderDlg *p_gThis;
	CString m_strURL;
	afx_msg void OnBnClickedBtnGo();
	CString m_strFilename;
	CProgressCtrl m_pgbar;
	static void UpdatePgBar(int ContentSize, int CUR_LEN);
	afx_msg void OnBnClickedButtonPath();
	CMyThreadPool m_Thpool;
	afx_msg void OnBnClickedBtnStop();
};
