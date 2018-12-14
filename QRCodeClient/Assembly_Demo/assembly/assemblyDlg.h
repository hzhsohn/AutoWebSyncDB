
// assemblyDlg.h : 头文件
//

#pragma once

#include "afxwin.h"

// CassemblyDlg 对话框
class CassemblyDlg : public CDialogEx
{
// 构造
public:
	CassemblyDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ASSEMBLY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	//定时器
	void OnTimer(UINT nIDEvent);

	//初始化
	CButton _btn1;
	CListBox _lstBox;
	CStatic _ScanGunStatus;
	
	//JSON的内容输入框
	CEdit _txtJsonTime;
	CEdit _txtJsonBatch;
	CEdit _txtJsonPlace;
	//生成内容
	BOOL _isLockParameter;
	CString _strJSON;
	
	//事件
	afx_msg void OnBnClickedButton1();
	
	//辅助函数
	void CassemblyDlg::setScanGun(BOOL b);
};
