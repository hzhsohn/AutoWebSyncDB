
// assemblyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "assembly.h"
#include "assemblyDlg.h"
#include "afxdialogex.h"
#include "StringUtil.h"
#include "cJSON\cJSON.h"
#include "ClientNet.h"
#include "md5\md5.h"

//shd的定时器通讯循环
#define SHD_TIMER	1

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CassemblyDlg 对话框

//主界面的句柄
HWND g_hwndMainForm;

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
	DDX_Control(pDX, IDC_EDIT4, _txtJsonOperater);
}

BEGIN_MESSAGE_MAP(CassemblyDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CassemblyDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CassemblyDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CassemblyDlg::OnBnClickedButton1)
	ON_MESSAGE(WM_USER+1000,msgConnectCacheServerOK)
	ON_MESSAGE(WM_USER+1001,msgConnectCacheServerFail)
	ON_MESSAGE(WM_USER+1002,msgConnectedKeep)
	ON_MESSAGE(WM_USER+1003,msgCacheDataSuccess)
	ON_MESSAGE(WM_USER+1004,msgCacheDataFail)
	ON_MESSAGE(WM_USER+1005,msgCacheUploadResult)
END_MESSAGE_MAP()


afx_msg LONG CassemblyDlg::msgConnectCacheServerOK(WPARAM wParam,LPARAM lParam)
{
	setConnectCacheServerOK(TRUE);
	return TRUE;
}
afx_msg LONG CassemblyDlg::msgConnectCacheServerFail(WPARAM wParam,LPARAM lParam)
{
	setConnectCacheServerOK(FALSE);
	return TRUE;
}
afx_msg LONG CassemblyDlg::msgConnectedKeep(WPARAM wParam,LPARAM lParam)
{
	setConnectCacheServerOK(TRUE);
	return TRUE;
}
afx_msg LONG CassemblyDlg::msgCacheDataSuccess(WPARAM wParam,LPARAM lParam)
{
	char*key=(char*)wParam;
	CString str;
	str.Format(_T("%s,提交缓存成功!"),_S2WS_CSTR(key));
	setListAdd(str);
	return TRUE;
}
afx_msg LONG CassemblyDlg::msgCacheDataFail(WPARAM wParam,LPARAM lParam)
{
	char*key=(char*)wParam;
	CString str;
	str.Format(_T("%s,提交缓存失败."),_S2WS_CSTR(key));
	setListAdd(str);
	return TRUE;
}
afx_msg LONG CassemblyDlg::msgCacheUploadResult(WPARAM wParam,LPARAM lParam)
{
	char*key=(char*)wParam;
	bool isok=(bool)lParam;
	CString str;
	if(isok)
	{
		str.Format(_T("%s,上传服务器成功!"),_S2WS_CSTR(key));
	}
	else
	{
		str.Format(_T("%s,上传服务器失败."),_S2WS_CSTR(key));
	}
	setListAdd(str);
	return TRUE;
}

void CassemblyDlg::setConnectCacheServerOK(BOOL b)
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

void CassemblyDlg::setListAdd(CString str)
{
	//最大显示数据行数
	if(_lstBox.GetCount()>200)
	{
		_lstBox.DeleteString(0);
	}
	_lstBox.AddString(str);
	_lstBox.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
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
	g_hwndMainForm=this->GetSafeHwnd();
	_isGunOK=FALSE;
	_isLockParameter=FALSE;
	//打开定时器1 
	ClientNetInit();
	SetTimer(SHD_TIMER,1,NULL);
	ScanGunContentLen=0;
	memset(strScanGunCache,0,sizeof(strScanGunCache));
	memset(strScanContent,0,sizeof(strScanContent));
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
	setConnectCacheServerOK(FALSE);
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
	//联网处理
	ClientNetLoop();
	CWnd::OnTimer(nIDEvent);
}

void CassemblyDlg::OnBnClickedButton1()
{
	_isLockParameter=!_isLockParameter;
	if(_isLockParameter)
	{
		CString str;
		str.Format(_T("已锁定参数"));
		setListAdd(str);
		
		_txtJsonOperater.SetReadOnly(TRUE);
		_txtJsonTime.SetReadOnly(TRUE);
		_txtJsonBatch.SetReadOnly(TRUE);
		_txtJsonPlace.SetReadOnly(TRUE);
		//
		_strJSON="";
		_lstBox.SetFocus();
	}
	else
	{
		_txtJsonOperater.SetReadOnly(FALSE);
		_txtJsonTime.SetReadOnly(FALSE);
		_txtJsonBatch.SetReadOnly(FALSE);
		_txtJsonPlace.SetReadOnly(FALSE);

		CString str;
		str.Format(_T("解锁参数"));
		setListAdd(str);
		
		_strJSON="";
		TRACE("JSON set null\n");

	}
}

BOOL CassemblyDlg::genJson(const char*scanContent)
{
	//----------
	//判断扫描内容的格式是否正确
	if(memcmp(scanContent,"http",4))//头内容不为http
	{
		setListAdd(_T("[错误]----扫描内容格式错误,不为http或https连接"));
		return FALSE;
	}
	if(!strchr(scanContent,'?'))//不存在问号
	{
		setListAdd(_T("[错误]----扫描内容格式错误,不存在唯一标识参数"));
		return FALSE;
	}
	//
	if(_isLockParameter)
	{		
		CString str0;
		CString str1;
		CString str2;
		CString str3;
		_txtJsonOperater.GetWindowText(str0);
		_txtJsonTime.GetWindowText(str1);
		_txtJsonBatch.GetWindowText(str2);
		_txtJsonPlace.GetWindowText(str3);

		char *out1;
		cJSON *root;
		//
		root=cJSON_CreateObject();
		cJSON_AddStringToObject(root,"_scan",scanContent);
		cJSON_AddStringToObject(root,"_act","update"); //内容填充模式,追加或更新
		cJSON_AddStringToObject(root,"operater",_WS2S_CSTR(str0.GetBuffer()));
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
		//
		return TRUE;
	}
	return FALSE;
}

//搜索枪录入
BOOL CassemblyDlg::PreTranslateMessage(MSG * pMsg)
{
		time_t tmpT=zhPlatGetTime();

		if (pMsg->message == WM_CHAR)
		{	
			if((char)pMsg->wParam!='\r' && (char)pMsg->wParam!='\n')
			{
				dwScanGunTime=tmpT;
				strScanGunCache[ScanGunContentLen]=(char)pMsg->wParam;
				ScanGunContentLen++;
				strScanGunCache[ScanGunContentLen]=0;
			}
		}

		if(ScanGunContentLen>0)
		{				
			//100毫秒后不再录入视为完毕
			if(tmpT-dwScanGunTime>100)
			{
				ScanGunContentLen=0;
				dwScanGunTime=tmpT;
				//100毫秒内录入大于5个字符.就是扫描枪,否则视为普通输入
				if(strlen(strScanGunCache)>5)
				{
					if(strcmp(strScanContent,strScanGunCache))
					{
						dwScanGunClearTime=tmpT;
						strcpy(strScanContent,strScanGunCache);
						TRACE("扫码枪内容=%s\n",strScanGunCache);
						//提交到缓存服务器
						if(_isLockParameter)
						{
							//生成JSON并提交到缓存服务器
							if(genJson(strScanGunCache))
							{
								char *json=(char*)_strJSON.c_str();
								char strMD5[40]={0};
								MDString(json,strMD5);
								MDStk(strMD5,strMD5);
								ClientNetSend(strMD5,json);
								//								
								CString str;
								str.Format(_T("提交 md5=%s json=%s"),_S2WS_CSTR(strMD5),_S2WS_CSTR(json));
								setListAdd(str);
							}
						}
						else
						{
							setListAdd(_T("[错误]----提交失败,参数未锁定"));
						}
					}
					else
					{
						TRACE("重复录入=%s\n",strScanGunCache);
					}
				}
				else
				{
					TRACE("扫码枪字数不足\n");
				}
			}
		}
		else
		{
			//2秒内不能录入重复数值
			if(tmpT-dwScanGunClearTime>2000)
			{
				strcpy(strScanContent,"");
				dwScanGunClearTime=tmpT;
			}
		}
	return __super::PreTranslateMessage(pMsg);
}