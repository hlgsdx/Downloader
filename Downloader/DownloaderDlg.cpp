
// DownloaderDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Downloader.h"
#include "DownloaderDlg.h"
#include "afxdialogex.h"
#include "DlHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const auto c_strTitle = _T("Simple Downloader");

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDownloaderDlg 对话框

CDownloaderDlg *CDownloaderDlg::p_gThis = NULL;

CDownloaderDlg::CDownloaderDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DOWNLOADER_DIALOG, pParent)
	, m_strURL(_T("http://www.baidu.com/img/bd_logo1.png"))
	, m_strFilename(_T("./File.png"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	p_gThis = this;
}

void CDownloaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_URL, m_strURL);
	DDX_Text(pDX, IDC_EDIT_FILE, m_strFilename);
	DDX_Control(pDX, IDC_PGBAR, m_pgbar);
}

BEGIN_MESSAGE_MAP(CDownloaderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_GO, &CDownloaderDlg::OnBnClickedBtnGo)
	ON_BN_CLICKED(IDC_BTN_SETPATH, &CDownloaderDlg::OnBnClickedButtonPath)
	ON_BN_CLICKED(IDC_BTN_STOP, &CDownloaderDlg::OnBnClickedBtnStop)
END_MESSAGE_MAP()


// CDownloaderDlg 消息处理程序

BOOL CDownloaderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	this->SetWindowText(c_strTitle);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDownloaderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDownloaderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDownloaderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDownloaderDlg::OnBnClickedBtnGo()
{
	UpdateData();
// 	CDlHelper cdl(m_strURL, m_strFilename);
// 	cdl.download(&CDownloaderDlg::UpdatePgBar);
	DlTask *ptask = new DlTask(m_strURL, m_strFilename,&CDownloaderDlg::UpdatePgBar);
	m_Thpool.CreateThreadPool(1,1);
	m_Thpool.Push(ptask);
}

void CDownloaderDlg::UpdatePgBar(int ContentSize, int CUR_LEN)
{
	p_gThis->m_pgbar.SetRange32(0,ContentSize);
	p_gThis->m_pgbar.SetPos(CUR_LEN);
}



void CDownloaderDlg::OnBnClickedButtonPath()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(FALSE);
	if (dlg.DoModal() == IDOK)
	{
		CString strPath = dlg.GetPathName();
		m_strFilename = strPath;
	}

	UpdateData(FALSE);
}


void CDownloaderDlg::OnBnClickedBtnStop()
{
	m_Thpool.DestroyThreadPool();
}
