
// CntrItem.h : CQRCodeClientCntrItem 类的接口
//

#pragma once

class CQRCodeClientDoc;
class CQRCodeClientView;

class CQRCodeClientCntrItem : public COleClientItem
{
	DECLARE_SERIAL(CQRCodeClientCntrItem)

// 构造函数
public:
	CQRCodeClientCntrItem(CQRCodeClientDoc* pContainer = NULL);
		// 注意: 允许 pContainer 为 NULL 以启用 IMPLEMENT_SERIALIZE
		//  IMPLEMENT_SERIALIZE 要求类具有带零
		//  参数的构造函数。OLE 项通常是用
		//  非 NULL 文档指针构造的

// 特性
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

// 实现
public:
	~CQRCodeClientCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void Serialize(CArchive& ar);
};

