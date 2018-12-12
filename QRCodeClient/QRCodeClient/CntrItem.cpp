
// CntrItem.cpp : CQRCodeClientCntrItem ���ʵ��
//

#include "stdafx.h"
#include "QRCodeClient.h"

#include "QRCodeClientDoc.h"
#include "QRCodeClientView.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CQRCodeClientCntrItem ��ʵ��

IMPLEMENT_SERIAL(CQRCodeClientCntrItem, COleClientItem, 0)

CQRCodeClientCntrItem::CQRCodeClientCntrItem(CQRCodeClientDoc* pContainer)
	: COleClientItem(pContainer)
{
	// TODO: �ڴ����һ���Թ������
}

CQRCodeClientCntrItem::~CQRCodeClientCntrItem()
{
	// TODO: �ڴ˴�����������
}

void CQRCodeClientCntrItem::OnChange(OLE_NOTIFICATION nCode, DWORD dwParam)
{
	ASSERT_VALID(this);

	COleClientItem::OnChange(nCode, dwParam);

	// �ڱ༭ĳ��ʱ(�͵ر༭����ȫ�򿪽��б༭)��
	//  �����������״̬�ĸ���
	//  �������ݵĿ������״̬�ĸ��ķ��� OnChange ֪ͨ��

	// TODO: ͨ������ UpdateAllViews ʹ������Ч
	//  (�����ʺ�����Ӧ�ó������ʾ)

	GetDocument()->UpdateAllViews(NULL);
		// ����ֻ����������ͼ/����ʾ
}

BOOL CQRCodeClientCntrItem::OnChangeItemPosition(const CRect& rectPos)
{
	ASSERT_VALID(this);

	// �͵ؼ����ڼ䣬���������� CQRCodeClientCntrItem::OnChangeItemPosition
	//  �Ը��ľ͵ش��ڵ�λ�á�
	//  ��ͨ�������ڷ�����
	//  �ĵ��е����ݸ��Ķ����·�Χ�ı����
	//  �͵ص�����С������ġ�
	//
	// �˴���Ĭ�����õ��û��࣬�û��ཫ����
	//  COleClientItem::SetItemRects �Խ�����
	//  �ƶ����µ�λ�á�

	if (!COleClientItem::OnChangeItemPosition(rectPos))
		return FALSE;

	// TODO: ���¸���ľ���/��Χ���ܴ��ڵ��κλ���

	return TRUE;
}

BOOL CQRCodeClientCntrItem::OnShowControlBars(CFrameWnd* pFrameWnd, BOOL bShow)
{
	CFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CFrameWndEx, pFrameWnd);

	if (pMainFrame != NULL)
	{
		ASSERT_VALID(pMainFrame);
		return pMainFrame->OnShowPanes(bShow);
	}

	return FALSE;
}

void CQRCodeClientCntrItem::OnGetItemPosition(CRect& rPosition)
{
	ASSERT_VALID(this);

	// �͵ؼ����ڼ䣬������ CQRCodeClientCntrItem::OnGetItemPosition
	//  �Ծ��������λ�á�ͨ����
	//  �þ��λᷴӳ�뼤��������ͼ 
	//  �����ĵ�ǰλ�á����� 
	//  CQRCodeClientCntrItem::GetActiveView �ɵõ���ͼ��

	// TODO: ���� rPosition �е���ȷ����(�����ر�ʾ)

	CSize size;
	rPosition.SetRectEmpty();
	if (GetExtent(&size, m_nDrawAspect))
	{
		CQRCodeClientView* pView = GetActiveView();
		ASSERT_VALID(pView);
		if (!pView)
			return;
		CDC *pDC = pView->GetDC();
		ASSERT(pDC);
		if (!pDC)
			return;
		pDC->HIMETRICtoLP(&size);
		rPosition.SetRect(10, 10, size.cx + 10, size.cy + 10);
	}
	else
		rPosition.SetRect(10, 10, 210, 210);
}

void CQRCodeClientCntrItem::OnActivate()
{
    // ÿ�����ֻ����һ���͵ؼ�����
    CQRCodeClientView* pView = GetActiveView();
    ASSERT_VALID(pView);
	if (!pView)
		return;
    COleClientItem* pItem = GetDocument()->GetInPlaceActiveItem(pView);
    if (pItem != NULL && pItem != this)
        pItem->Close();
    
    COleClientItem::OnActivate();
}

void CQRCodeClientCntrItem::OnDeactivateUI(BOOL bUndoable)
{
	COleClientItem::OnDeactivateUI(bUndoable);

    DWORD dwMisc = 0;
    m_lpObject->GetMiscStatus(GetDrawAspect(), &dwMisc);
    if (dwMisc & OLEMISC_INSIDEOUT)
        DoVerb(OLEIVERB_HIDE, NULL);
}

void CQRCodeClientCntrItem::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	// ���ȵ��û����Զ�ȡ COleClientItem ���ݡ�
	// ��Ϊ���������ô� CQRCodeClientCntrItem::GetDocument ���ص� m_pDocument ָ�룬
	//  ��������ȵ���
	//  ���� Serialize��
	COleClientItem::Serialize(ar);

	// ���ڴ洢/�����ض��� CQRCodeClientCntrItem ������
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CQRCodeClientCntrItem ���

#ifdef _DEBUG
void CQRCodeClientCntrItem::AssertValid() const
{
	COleClientItem::AssertValid();
}

void CQRCodeClientCntrItem::Dump(CDumpContext& dc) const
{
	COleClientItem::Dump(dc);
}
#endif

