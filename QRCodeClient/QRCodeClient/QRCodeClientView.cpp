
// QRCodeClientView.cpp : CQRCodeClientView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "QRCodeClient.h"
#endif

#include "QRCodeClientDoc.h"
#include "CntrItem.h"
#include "resource.h"
#include "QRCodeClientView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CQRCodeClientView

IMPLEMENT_DYNCREATE(CQRCodeClientView, CView)

BEGIN_MESSAGE_MAP(CQRCodeClientView, CView)
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_COMMAND(ID_OLE_INSERT_NEW, &CQRCodeClientView::OnInsertObject)
	ON_COMMAND(ID_CANCEL_EDIT_CNTR, &CQRCodeClientView::OnCancelEditCntr)
	ON_COMMAND(ID_FILE_PRINT, &CQRCodeClientView::OnFilePrint)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CQRCodeClientView ����/����

CQRCodeClientView::CQRCodeClientView()
{
	m_pSelection = NULL;
	// TODO: �ڴ˴���ӹ������

}

CQRCodeClientView::~CQRCodeClientView()
{
}

BOOL CQRCodeClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CQRCodeClientView ����

void CQRCodeClientView::OnDraw(CDC* pDC)
{
	if (!pDC)
		return;

	CQRCodeClientDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	// TODO: ͬʱ�����ĵ��е����� OLE ��

	// ������λ�û���ѡ���һ��
	//  ʵ���������Ļ��ƴ����Ӧ�Ƴ��˴��롣��λ��
	//  �� CQRCodeClientCntrItem ���صľ�����ȫ��Ӧ��
	//  �Ӷ������͵ر༭��Ч����

	// TODO: ���ջ��ƴ�����ɺ��Ƴ��˴��롣
	if (m_pSelection != NULL)
	{
		CSize size;
		CRect rect(10, 10, 210, 210);
		
		if (m_pSelection->GetExtent(&size, m_pSelection->m_nDrawAspect))
		{
			pDC->HIMETRICtoLP(&size);
			rect.right = size.cx + 10;
			rect.bottom = size.cy + 10;
		}
		m_pSelection->Draw(pDC, rect);
	}
}

void CQRCodeClientView::OnInitialUpdate()
{
	CView::OnInitialUpdate();


	// TODO: д������ѡ��ģʽ����֮���Ƴ��˴���
	m_pSelection = NULL;    // ��ʼ����ѡ����

}

void CQRCodeClientView::OnDestroy()
{
	// ����ʱͣ�ô������
	// ʹ�ò����ͼʱ�ǳ���Ҫ 
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
   }
   CView::OnDestroy();
}



// OLE �ͻ���֧�ֺ�����

BOOL CQRCodeClientView::IsSelected(const CObject* pDocItem) const
{
	// �����ѡ����ֻ����
	//  CQRCodeClientCntrItem ����������ʵ�־��㹻�ˡ���Ҫ��������ѡ����ƣ�
	//  ��Ӧ�滻�˴���ʵ��

	// TODO: ʵ�ֶ���ѡ OLE �ͻ�������в��Եĺ���

	return pDocItem == m_pSelection;
}

void CQRCodeClientView::OnInsertObject()
{
	// ���ñ�׼�ġ�������󡱶Ի����Ի���й�
	//  �� CQRCodeClientCntrItem �������Ϣ
	COleInsertDialog dlg;
	if (dlg.DoModal() != IDOK)
		return;

	BeginWaitCursor();

	CQRCodeClientCntrItem* pItem = NULL;
	TRY
	{
		// ��������ĵ������ӵ�����
		CQRCodeClientDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		pItem = new CQRCodeClientCntrItem(pDoc);
		ASSERT_VALID(pItem);

		// ͨ���Ի������ݳ�ʼ������
		if (!dlg.CreateItem(pItem))
			AfxThrowMemoryException();  // �κ��쳣�������¸ý��
		ASSERT_VALID(pItem);
		
        if (dlg.GetSelectionType() == COleInsertDialog::createNewItem)
			pItem->DoVerb(OLEIVERB_SHOW, this);

		ASSERT_VALID(pItem);
		// ��Ϊ�����û�������ƣ���Ὣ��ѡ����
		//  ����Ϊ��������һ��

		// TODO: ����ʵ����ѡ���ݣ�ʹ���ʺ�������Ӧ�ó���
		m_pSelection = pItem;   // ����ѡ��������Ϊ��������һ��
		pDoc->UpdateAllViews(NULL);
	}
	CATCH(CException, e)
	{
		if (pItem != NULL)
		{
			ASSERT_VALID(pItem);
			pItem->Delete();
		}
		AfxMessageBox(IDP_FAILED_TO_CREATE);
	}
	END_CATCH

	EndWaitCursor();
}

// �������������ṩ�˱�׼����
//  �û�������ȡ���͵ر༭�Ự���˴���
//  ����(�����Ƿ�����)������ͣ��
void CQRCodeClientView::OnCancelEditCntr()
{
	// �رմ���ͼ�е��κξ͵ػ�
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL)
	{
		pActiveItem->Close();
	}
	ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
}

// �ھ͵ر༭һ������ʱ��������Ҫ�� OnSetFocus �� OnSize 
//  �������⴦��
void CQRCodeClientView::OnSetFocus(CWnd* pOldWnd)
{
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL &&
		pActiveItem->GetItemState() == COleClientItem::activeUIState)
	{
		// ��������ͬһ��ͼ�У�����Ҫ���������õ�����
		CWnd* pWnd = pActiveItem->GetInPlaceWindow();
		if (pWnd != NULL)
		{
			pWnd->SetFocus();   // ��Ҫ���û���
			return;
		}
	}

	CView::OnSetFocus(pOldWnd);
}

void CQRCodeClientView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL)
		pActiveItem->SetItemRects();
}

void CQRCodeClientView::OnFilePrint()
{
	//Ĭ������£�Ҫ���ĵ�ʹ�� IOleCommandTarget ��ӡ����
	//�������Ҫ����Ϊ��
	//���Ƴ��� COleDocObjectItem::DoDefaultPrinting ���á�
	//�������ĳ��ԭ�����ʧ�ܣ��򽫳���ʹ�� IPrint �ӿڴ�ӡ
	//���ĵ�����
	CPrintInfo printInfo;
	ASSERT(printInfo.m_pPD != NULL); 
	if (S_OK == COleDocObjectItem::DoDefaultPrinting(this, &printInfo))
		return;
	
	CView::OnFilePrint();

}


void CQRCodeClientView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CQRCodeClientView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CQRCodeClientView ���

#ifdef _DEBUG
void CQRCodeClientView::AssertValid() const
{
	CView::AssertValid();
}

void CQRCodeClientView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CQRCodeClientDoc* CQRCodeClientView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CQRCodeClientDoc)));
	return (CQRCodeClientDoc*)m_pDocument;
}
#endif //_DEBUG


// CQRCodeClientView ��Ϣ�������
