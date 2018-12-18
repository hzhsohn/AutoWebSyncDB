
// assemblyDlg.h : ͷ�ļ�
//

#pragma once

#include "afxwin.h"
#include <string>

using namespace std;

// CassemblyDlg �Ի���
class CassemblyDlg : public CDialogEx
{
// ����
public:
	CassemblyDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ASSEMBLY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	//��ʱ��
	void OnTimer(UINT nIDEvent);

	//��ʼ��
	CButton _btn1;
	CListBox _lstBox;

	//ɨ��ǹ
	BOOL _isGunOK;
	CStatic _ScanGunStatus;
	
	//JSON�����������
	CEdit _txtJsonOperater;
	CEdit _txtJsonTime;
	CEdit _txtJsonBatch;
	CEdit _txtJsonPlace;
	//��������
	BOOL _isLockParameter;
	string _strJSON;
	
	//�¼�
	afx_msg void OnBnClickedButton1();
	
	//ɨ��ǹ���봦��
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	time_t dwScanGunTime;
	time_t dwScanGunClearTime;
	int ScanGunContentLen;
	char strScanGunCache[512];
	char strScanContent[512];

	//��������
	afx_msg LONG CassemblyDlg::msgConnectCacheServerOK(WPARAM wParam,LPARAM lParam);
	afx_msg LONG CassemblyDlg::msgConnectCacheServerFail(WPARAM wParam,LPARAM lParam);
	void CassemblyDlg::setConnectCacheServerOK(BOOL b);
	void CassemblyDlg::setListAdd(CString str);
};
