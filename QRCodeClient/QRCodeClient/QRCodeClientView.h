
// QRCodeClientView.h : CQRCodeClientView ��Ľӿ�
//

#pragma once

class CQRCodeClientCntrItem;

class CQRCodeClientView : public CView
{
protected: // �������л�����
	CQRCodeClientView();
	DECLARE_DYNCREATE(CQRCodeClientView)

// ����
public:
	CQRCodeClientDoc* GetDocument() const;
	// m_pSelection ����ѡ���ݱ����ڵ�ǰ�� CQRCodeClientCntrItem �С�
	// �������Ӧ�ó������ֳ�Ա����������
	//  ��ʾĳ��ѡ�������ڲ����� CQRCodeClientCntrItem �Ķ�����
	//  ѡ����һ�����������ṩ����ѡ��
	//  ���Ƶ�Ŀ��ֻ�ǰ���������

	// TODO: ���ʺ�Ӧ�ó����ѡ������滻��ѡ�����
	CQRCodeClientCntrItem* m_pSelection;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���
	virtual BOOL IsSelected(const CObject* pDocItem) const;// ����֧��

// ʵ��
public:
	virtual ~CQRCodeClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnInsertObject();
	afx_msg void OnCancelEditCntr();
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // QRCodeClientView.cpp �еĵ��԰汾
inline CQRCodeClientDoc* CQRCodeClientView::GetDocument() const
   { return reinterpret_cast<CQRCodeClientDoc*>(m_pDocument); }
#endif

