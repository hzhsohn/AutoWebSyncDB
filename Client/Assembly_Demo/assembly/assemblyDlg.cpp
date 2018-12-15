
// assemblyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "assembly.h"
#include "assemblyDlg.h"
#include "afxdialogex.h"
#include "StringUtil.h"
#include "cJSON\cJSON.h"

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
		BmpA.LoadBitmap(IDB_SGAN_YES);
	}
	else
	{
		BmpA.LoadBitmap(IDB_SGAN_NO);
	}	

	_ScanGunStatus.ModifyStyle(0xF,SS_BITMAP|SS_CENTERIMAGE);//设置静态控件的样式，使得它可以使用位图
	_ScanGunStatus.SetBitmap((HBITMAP)BmpA);
	_ScanGunStatus.ShowWindow(TRUE);
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
	//程序启动初始化
	_isGunOK=FALSE;
	_isLockParameter=FALSE;
	//打开定时器1 
	SetTimer(SHD_TIMER,1000,NULL);
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

	//--------------------------------------------
	//初始化界面素材
	setScanGun(FALSE);
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
	//扫描枪状态
	setScanGun(_isGunOK);
	CWnd::OnTimer(nIDEvent);
}


void CassemblyDlg::OnBnClickedButton1()
{
	_isLockParameter=!_isLockParameter;
	if(_isLockParameter)
	{
		CString str;
		str.Format(_T("已锁定参数"));
		_lstBox.AddString(str);
		
		_txtJsonTime.SetReadOnly(TRUE);
		_txtJsonBatch.SetReadOnly(TRUE);
		_txtJsonPlace.SetReadOnly(TRUE);

		CString str1;
		CString str2;
		CString str3;
		_txtJsonTime.GetWindowText(str1);
		_txtJsonBatch.GetWindowText(str2);
		_txtJsonPlace.GetWindowText(str3);

		char *out1;
		cJSON *root;
		//
		root=cJSON_CreateObject();
		cJSON_AddStringToObject(root,"time",_WS2S_CSTR(str1.GetBuffer()));
		cJSON_AddStringToObject(root,"batch",_WS2S_CSTR(str2.GetBuffer()));
		cJSON_AddStringToObject(root,"place",_WS2S_CSTR(str3.GetBuffer()));
		//
		out1=cJSON_PrintUnformatted(root);
		//	
		_strJSON=out1;
		TRACE("JSON=%s\n",out1);
		//-------------------------------
		cJSON_Delete(root);
		free(out1);

	}
	else
	{
		_txtJsonTime.SetReadOnly(FALSE);
		_txtJsonBatch.SetReadOnly(FALSE);
		_txtJsonPlace.SetReadOnly(FALSE);

		CString str;
		str.Format(_T("解锁参数"));
		_lstBox.AddString(str);
		
		_strJSON="";
		TRACE("JSON set null\n");

	}
}
