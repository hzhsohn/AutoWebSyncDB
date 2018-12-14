
// assemblyDlg.h : ͷ�ļ�
//

#pragma once

#include "afxwin.h"

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
	CStatic _ScanGunStatus;
	
	//JSON�����������
	CEdit _txtJsonTime;
	CEdit _txtJsonBatch;
	CEdit _txtJsonPlace;
	//��������
	BOOL _isLockParameter;
	CString _strJSON;
	
	//�¼�
	afx_msg void OnBnClickedButton1();
	
	//��������
	void CassemblyDlg::setScanGun(BOOL b);
};
