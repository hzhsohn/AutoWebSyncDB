
// assemblyDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "assembly.h"
#include "assemblyDlg.h"
#include "afxdialogex.h"
#include "StringUtil.h"
#include "cJSON\cJSON.h"
#include "ClientNet.h"
#include "md5\md5.h"
#include <mmSystem.h>
#include "getwork.h"
#include "urlcode\encoding.h"

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
	DDX_Control(pDX, IDC_EDIT5, _txtJsonType);
	DDX_Control(pDX, IDC_SGUN_STATE2, _CacheConnectStatus);
	DDX_Control(pDX, IDC_CHECK1, isAppendInfo);
	DDX_Control(pDX, IDC_JPLACE, _LbJPlace);
	DDX_Control(pDX, IDC_ADR, _LbJAdr);
	DDX_Control(pDX, IDC_PCHI, _LbPChi);
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
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_CHECK1, &CassemblyDlg::OnBnClickedCheck1)
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
	str.Format(_T("%s,�ύ����ɹ�!"),_S2WS_CSTR(key));
	setListAdd(str);	
	PlaySound(okWavPath, NULL ,SND_FILENAME | SND_ASYNC); 
	return TRUE;
}
afx_msg LONG CassemblyDlg::msgCacheDataFail(WPARAM wParam,LPARAM lParam)
{
	char*key=(char*)wParam;
	CString str;
	str.Format(_T("%s,�ύ����ʧ��."),_S2WS_CSTR(key));
	setListAdd(str);
	return TRUE;
}
afx_msg LONG CassemblyDlg::msgCacheUploadResult(WPARAM wParam,LPARAM lParam)
{
	char*key=(char*)wParam;
	int ret=(int)lParam;
	CString str;
	switch(ret)
	{
		case 1:
		{
			str.Format(_T("%s,�ϴ��������ɹ�!"),_S2WS_CSTR(key));
		}
			break;
		case 2:
		{
			str.Format(_T("%s,�ϴ�������ʧ��."),_S2WS_CSTR(key));
		}
			break;
		case 3:
		{
			str.Format(_T("%s,��¼�Ѿ�����,�����ظ����"),_S2WS_CSTR(key));
		}
			break;
		case 4:
		{
			str.Format(_T("%s,�ύ������JSON��ʽ"),_S2WS_CSTR(key));
		}
			break;
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
	_CacheConnectStatus.ModifyStyle(0xF,SS_BITMAP|SS_CENTERIMAGE);//���þ�̬�ؼ�����ʽ��ʹ��������ʹ��λͼ
	_CacheConnectStatus.SetBitmap((HBITMAP)BmpA);
	_CacheConnectStatus.ShowWindow(TRUE);
}

void CassemblyDlg::setScanGunOK(BOOL b)
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

// CassemblyDlg ��Ϣ�������
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
	ScanGunContentLen=0;
	memset(strScanGunCache,0,sizeof(strScanGunCache));
	memset(strScanContent,0,sizeof(strScanContent));
	//
	CTime time;
	time = CTime::GetCurrentTime();
	CString curdata = time.Format("%Y-%m-%d");
	_txtJsonTime.SetWindowText(curdata);
	//
	char bufpath[512]={0};
	getCurrentPath("ok.wav",bufpath,512);
	okWavPath=bufpath;
	//-------------------------------------------
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
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
	setScanGunOK(TRUE);
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CassemblyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CassemblyDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
void CassemblyDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		_txtJsonType.SetReadOnly(TRUE);
		isAppendInfo.EnableWindow(FALSE);
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
		_txtJsonType.SetReadOnly(FALSE);
		isAppendInfo.EnableWindow(TRUE);

		CString str;
		str.Format(_T("��������"));
		setListAdd(str);
		
		_strJSON="";
		TRACE("JSON set null\n");
	}
}

BOOL CassemblyDlg::genJsonUpdate(const char*scanContent)
{
	//----------
	//�ж�ɨ�����ݵĸ�ʽ�Ƿ���ȷ
	if(memcmp(scanContent,"http",4))//ͷ���ݲ�Ϊhttp
	{
		setListAdd(_T("[����]----ɨ�����ݸ�ʽ����,��Ϊhttp��https����"));
		return FALSE;
	}
	if(!strchr(scanContent,'?'))//�������ʺ�
	{
		setListAdd(_T("[����]----ɨ�����ݸ�ʽ����,������Ψһ��ʶ����"));
		return FALSE;
	}
	//
	if(_isLockParameter)
	{		
		CString str0;
		CString str1;
		CString str2;
		CString str3;
		CString str4;
		_txtJsonOperater.GetWindowText(str0);
		_txtJsonTime.GetWindowText(str1);
		_txtJsonBatch.GetWindowText(str2);
		_txtJsonPlace.GetWindowText(str3);
		_txtJsonType.GetWindowText(str4);

		//
		char *out1;
		cJSON *root;
		root=cJSON_CreateObject();
		//
		//
		cJSON_AddStringToObject(root,"_scan",scanContent);
		cJSON_AddStringToObject(root,"_act","update"); //�������ģʽ,׷�ӻ���� append or update
		cJSON_AddStringToObject(root,"operater",_WS2S_CSTR(str0.GetBuffer()));
		cJSON_AddStringToObject(root,"time",_WS2S_CSTR(str1.GetBuffer()));
		cJSON_AddStringToObject(root,"batch",_WS2S_CSTR(str2.GetBuffer()));
		cJSON_AddStringToObject(root,"place",_WS2S_CSTR(str3.GetBuffer()));
		cJSON_AddStringToObject(root,"type",_WS2S_CSTR(str4.GetBuffer()));
		out1=cJSON_PrintUnformatted(root);
				
		//
		
		char *utf8c;
		utf8c=(char *)malloc(strlen(out1)*5);
		//����ת��
		Gb2312ToUtf8(out1,strlen(out1),utf8c);

		_strJSON=utf8c;
		TRACE("JSON=%s\n",utf8c);
		//-------------------------------
		cJSON_Delete(root);
		free(out1);
		free(utf8c);
		//
		return TRUE;
	}
	return FALSE;
}


BOOL CassemblyDlg::genJsonAppend(const char*scanContent)
{
	//----------
	//�ж�ɨ�����ݵĸ�ʽ�Ƿ���ȷ
	if(memcmp(scanContent,"http",4))//ͷ���ݲ�Ϊhttp
	{
		setListAdd(_T("[����]----ɨ�����ݸ�ʽ����,��Ϊhttp��https����"));
		return FALSE;
	}
	if(!strchr(scanContent,'?'))//�������ʺ�
	{
		setListAdd(_T("[����]----ɨ�����ݸ�ʽ����,������Ψһ��ʶ����"));
		return FALSE;
	}
	//
	if(_isLockParameter)
	{		
		CString str0;
		CString str1;
		CString str2;
		_txtJsonOperater.GetWindowText(str0);
		_txtJsonTime.GetWindowText(str1);
		_txtJsonBatch.GetWindowText(str2);

		//
		char *out1;
		cJSON *root;
		root=cJSON_CreateObject();
		//
		char *utf8c, *utf8c2;
		utf8c=(char *)malloc(strlen(_WS2S_CSTR(str0.GetBuffer()))*3);
		utf8c2=(char *)malloc(strlen(_WS2S_CSTR(str2.GetBuffer()))*3);
		//����ת��
		Gb2312ToUtf8(_WS2S_CSTR(str0.GetBuffer()),strlen(_WS2S_CSTR(str0.GetBuffer())),utf8c);
		Gb2312ToUtf8(_WS2S_CSTR(str2.GetBuffer()),strlen(_WS2S_CSTR(str2.GetBuffer())),utf8c2);
		//
		cJSON_AddStringToObject(root,"_scan",scanContent);
		cJSON_AddStringToObject(root,"_act","append"); //�������ģʽ,׷�ӻ���� append or update
		//cJSON_AddStringToObject(root,"operater",_WS2S_CSTR(str0.GetBuffer()));
		cJSON_AddStringToObject(root,"operater",utf8c);
		cJSON_AddStringToObject(root,"time",_WS2S_CSTR(str1.GetBuffer()));
		cJSON_AddStringToObject(root,"mark",utf8c2);
		out1=cJSON_PrintUnformatted(root);
				
		//
		_strJSON=out1;
		TRACE("JSON=%s\n",out1);
		//-------------------------------
		cJSON_Delete(root);
		free(out1);
		free(utf8c);
		free(utf8c2);
		//
		return TRUE;
	}
	return FALSE;
}


//����ǹ¼��
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
			//100�������¼����Ϊ���
			if(tmpT-dwScanGunTime>100)
			{
				ScanGunContentLen=0;
				dwScanGunTime=tmpT;
				//100������¼�����5���ַ�.����ɨ��ǹ,������Ϊ��ͨ����
				if(strlen(strScanGunCache)>5)
				{
					if(strcmp(strScanContent,strScanGunCache))
					{
						dwScanGunClearTime=tmpT;
						strcpy(strScanContent,strScanGunCache);
						TRACE("ɨ��ǹ����=%s\n",strScanGunCache);
						//�ύ�����������
						if(_isLockParameter)
						{
							//����JSON���ύ�����������
							BOOL b=isAppendInfo.GetCheck();
							BOOL sRet=FALSE;
							if(b)
							{
								sRet=genJsonAppend(strScanGunCache);
							}
							else
							{
								sRet=genJsonUpdate(strScanGunCache);
							}

							if(sRet)
							{
								char *json=(char*)_strJSON.c_str();
								char strMD5[40]={0};
								MDString(json,strMD5);
								MDStk(strMD5,strMD5);
								ClientNetSend(strMD5,json);
								//								
								CString str;
								str.Format(_T("�ύ %s %s"),_S2WS_CSTR(strMD5),_S2WS_CSTR(json));
								setListAdd(str);
							}
						}
						else
						{
							setListAdd(_T("[����]----�ύʧ��,����δ����"));
						}
					}
					else
					{
						TRACE("�ظ�¼��=%s\n",strScanGunCache);
					}
				}
				else
				{
					TRACE("ɨ��ǹ��������\n");
				}
			}
		}
		else
		{
			//2���ڲ���¼���ظ���ֵ
			if(tmpT-dwScanGunClearTime>2000)
			{
				strcpy(strScanContent,"");
				dwScanGunClearTime=tmpT;
			}
		}
	return __super::PreTranslateMessage(pMsg);
}

void CassemblyDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	if(nState==WA_INACTIVE)
    {
		setScanGunOK(FALSE);
    } 
    else
    {       
		setScanGunOK(TRUE);
    }

}


void CassemblyDlg::OnBnClickedCheck1()
{
	BOOL b=isAppendInfo.GetCheck();
	if(b)
	{
		_txtJsonType.ShowWindow(0);
		_txtJsonPlace.ShowWindow(0);
		_LbJPlace.ShowWindow(0);
		_LbJAdr.ShowWindow(0);
		_LbPChi.SetWindowText(_T("��ע: "));
	}
	else
	{
		_txtJsonType.ShowWindow(1);
		_txtJsonPlace.ShowWindow(1);
		_LbJPlace.ShowWindow(1);
		_LbJAdr.ShowWindow(1);
		_LbPChi.SetWindowText(_T("����: "));
	}
}
