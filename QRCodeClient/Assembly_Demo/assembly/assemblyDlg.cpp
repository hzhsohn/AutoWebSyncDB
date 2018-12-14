
// assemblyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "assembly.h"
#include "assemblyDlg.h"
#include "afxdialogex.h"
#include "StringUtil.h"

//shd的定时器通讯循环
#define SHD_TIMER	1

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CassemblyDlg 对话框

CassemblyDlg::CassemblyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CassemblyDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CassemblyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, _lstBox);
	DDX_Control(pDX, IDC_SGUN_STATE, _ScanGunStatus);
	DDX_Control(pDX, IDC_EDIT1, _txtJsonTime);
	DDX_Control(pDX, IDC_EDIT2, _txtJsonBatch);
	DDX_Control(pDX, IDC_EDIT3, _txtJsonPlace);
}

BEGIN_MESSAGE_MAP(CassemblyDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CassemblyDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CassemblyDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CassemblyDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


void CassemblyDlg::setScanGun(BOOL b)
{
	CBitmap BmpA;
	if(b)
	{
		BmpA.LoadBitmap(IDB_SGAN_YES); //载入图A
	}
	else
	{
		BmpA.LoadBitmap(IDB_SGAN_NO); //载入图A
	}	
	_ScanGunStatus.SetBitmap(BmpA);
}

// CassemblyDlg 消息处理程序
BOOL CassemblyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//-------------------------------------------
	//初始化界面素材
	_isLockParameter=FALSE;

	//-------------------------------------------
	//程序启动入口

	//-------------------------------------------
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CassemblyDlg::OnPaint()
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
HCURSOR CassemblyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CassemblyDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
}
void CassemblyDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CassemblyDlg::OnCancel();
}

//----------------------------------------------------------
void CassemblyDlg::OnTimer(UINT nIDEvent)
{
	
	CWnd::OnTimer(nIDEvent);
}


void CassemblyDlg::OnBnClickedButton1()
{
	_strJSON

	CString str;
	str.Format(_T("已锁定参数"));
	_lstBox.AddString(str);
}
