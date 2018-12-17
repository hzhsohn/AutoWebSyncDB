
// assemblyDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "assembly.h"
#include "assemblyDlg.h"
#include "afxdialogex.h"
#include "StringUtil.h"
#include "cJSON\cJSON.h"
#include "ClientNet.h"

//shd�Ķ�ʱ��ͨѶѭ��
#define SHD_TIMER	1

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CassemblyDlg �Ի���

//������ľ��
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

	_ScanGunStatus.ModifyStyle(0xF,SS_BITMAP|SS_CENTERIMAGE);//���þ�̬�ؼ�����ʽ��ʹ��������ʹ��λͼ
	_ScanGunStatus.SetBitmap((HBITMAP)BmpA);
	_ScanGunStatus.ShowWindow(TRUE);
}

void CassemblyDlg::setListAdd(CString str)
{
	//�����ʾ��������
	if(_lstBox.GetCount()>200)
	{
		_lstBox.DeleteString(0);
	}
	_lstBox.AddString(str);
	_lstBox.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
}

// CassemblyDlg ��Ϣ��������
BOOL CassemblyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//-------------------------------------------
	//����������ʼ��
	g_hwndMainForm=this->GetSafeHwnd();
	_isGunOK=FALSE;
	_isLockParameter=FALSE;
	//�򿪶�ʱ��1 
	ClientNetInit();
	SetTimer(SHD_TIMER,1,NULL);
	//-------------------------------------------
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի���������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CassemblyDlg::OnPaint()
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

	//--------------------------------------------
	//��ʼ�������ز�
	setConnectCacheServerOK(FALSE);
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CassemblyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CassemblyDlg::OnBnClickedOk()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
}
void CassemblyDlg::OnBnClickedCancel()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	CassemblyDlg::OnCancel();
}

//----------------------------------------------------------
void CassemblyDlg::OnTimer(UINT nIDEvent)
{
	//��������
	ClientNetLoop();
	CWnd::OnTimer(nIDEvent);
}

void CassemblyDlg::OnBnClickedButton1()
{
	_isLockParameter=!_isLockParameter;
	if(_isLockParameter)
	{
		CString str;
		str.Format(_T("����������"));
		setListAdd(str);
		
		_txtJsonOperater.SetReadOnly(TRUE);
		_txtJsonTime.SetReadOnly(TRUE);
		_txtJsonBatch.SetReadOnly(TRUE);
		_txtJsonPlace.SetReadOnly(TRUE);
				
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

		ClientNetSend(_strJSON.c_str());
	}
	else
	{
		_txtJsonOperater.SetReadOnly(FALSE);
		_txtJsonTime.SetReadOnly(FALSE);
		_txtJsonBatch.SetReadOnly(FALSE);
		_txtJsonPlace.SetReadOnly(FALSE);

		CString str;
		str.Format(_T("��������"));
		setListAdd(str);
		
		_strJSON="";
		TRACE("JSON set null\n");

	}
}