
// DownloaderDlg.cpp: ʵ���ļ�
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

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
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


// CDownloaderDlg �Ի���

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


// CDownloaderDlg ��Ϣ�������

BOOL CDownloaderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	this->SetWindowText(c_strTitle);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDownloaderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
