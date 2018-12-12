
// CntrItem.h : CQRCodeClientCntrItem ��Ľӿ�
//

#pragma once

class CQRCodeClientDoc;
class CQRCodeClientView;

class CQRCodeClientCntrItem : public COleClientItem
{
	DECLARE_SERIAL(CQRCodeClientCntrItem)

// ���캯��
public:
	CQRCodeClientCntrItem(CQRCodeClientDoc* pContainer = NULL);
		// ע��: ���� pContainer Ϊ NULL ������ IMPLEMENT_SERIALIZE
		//  IMPLEMENT_SERIALIZE Ҫ������д���
		//  �����Ĺ��캯����OLE ��ͨ������
		//  �� NULL �ĵ�ָ�빹���

// ����
public:
	CQRCodeClientDoc* GetDocument()
		{ return reinterpret_cast<CQRCodeClientDoc*>(COleClientItem::GetDocument()); }
	CQRCodeClientView* GetActiveView()
		{ return reinterpret_cast<CQRCodeClientView*>(COleClientItem::GetActiveView()); }

public:
	virtual void OnChange(OLE_NOTIFICATION wNotification, DWORD dwParam);
	virtual void OnActivate();

protected:
	virtual void OnGetItemPosition(CRect& rPosition);
	virtual void OnDeactivateUI(BOOL bUndoable);
	virtual BOOL OnChangeItemPosition(const CRect& rectPos);
	virtual BOOL OnShowControlBars(CFrameWnd* pFrameWnd, BOOL bShow);

// ʵ��
public:
	~CQRCodeClientCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void Serialize(CArchive& ar);
};

