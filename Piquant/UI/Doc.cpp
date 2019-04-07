
// Doc.cpp : CDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "App.h"
#endif

#include "Doc.h"
#include "MarketDataView/CFFEXView.h"
#include "MarketDataView/SHFE_INEView.h"
#include "MarketDataView/DCEView.h"
#include "MarketDataView/CZCEView.h"
#include "../ServiceLayer/CtpMsg.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMultiTraderDoc

IMPLEMENT_DYNCREATE(CDoc, CDocument)

BEGIN_MESSAGE_MAP(CDoc, CDocument)
END_MESSAGE_MAP()


// CMultiTraderDoc ����/����

CDoc::CDoc()
{
	// TODO:  �ڴ����һ���Թ������
	get_CtpMsgDistributor()->AddReceiver(this);
	hMonitorThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MonitorThreadFunc,this, 0, &MonitorThreadID); //�ڹ��캯���д��������߳�, ���Բ��ϼ������ݶ���
}

CDoc::~CDoc()
{
	get_CtpMsgDistributor()->DelReceiver(this);
}

BOOL CDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	SetTitle(L"Piquant"); //�޸�mfc�����exe����
	// TODO:  �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)
	CApp*p_app = (CApp*)AfxGetApp();
	CreateNewWindow(p_app->pDocTemplate_ShowInstrument, this);
	CreateNewWindow(p_app->pDocTemplate_CZCEView, this);
	CreateNewWindow(p_app->pDocTemplate_DCEView, this);
	CreateNewWindow(p_app->pDocTemplate_SHFEINEView, this);
	return TRUE;
}




// CMultiTraderDoc ���л�

void CDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO:  �ڴ���Ӵ洢����
	}
	else
	{
		// TODO:  �ڴ���Ӽ��ش���
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:     strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMultiTraderDoc ���

#ifdef _DEBUG
void CDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}

CFrameWnd* CDoc::CreateNewWindow(CDocTemplate* pTemplate, CDocument* pDocument)
{
	ASSERT_VALID(pTemplate);
	ASSERT_VALID(pDocument);
	CFrameWnd* pFrame =pTemplate->CreateNewFrame(pDocument, NULL);
	CRect rect;
	GetClientRect(pFrame->m_hWnd, &rect);
	pFrame->MoveWindow(100, 150, rect.Width(), rect.Height());
	if (pFrame == NULL)
	{
		TRACE0("Warning: failed to create new frame.\n");
		return NULL;
	}
	ASSERT_KINDOF(CFrameWnd, pFrame);
	pTemplate->InitialUpdateFrame(pFrame, pDocument);
	return pFrame;
}

void CDoc::OnReceiveData(WPARAM wParam, LPARAM lParam)
{
	if (wParam==ctp_msg::RtnMarketData)
	{
		MarketData* pMarketData = reinterpret_cast<MarketData*>(lParam);
		MarketData marketData = *pMarketData;
		DataQueue.Push(marketData);//�������ݽ���ȫ�����ݶ���
	}
}



//ȫ�ֵ��̼߳�������
void MonitorThreadFunc(CDoc* pDoc)
{
	vector<BaseData> instruments;
	if (NULL != get_global_mgr().GetService())
	{
		while (instruments.size()==0)
		{//����ȡ���ĺ�Լ��Ϣ��Ϊ��,��ȴ��������ȡ���ɵõ�Ϊֹ
			Sleep(2000);
			get_global_mgr().GetService()->get_basic_data(instruments);
		}
	}
	else
	{//��ȫ�ֹ���������ȡ����,ֱ��return
		return;
	}
	static MarketData marketData;
	string marketData_instrumentID;

	while (TRUE)
	{
		memset(&marketData, 0, sizeof(marketData));//����
		if (pDoc->DataQueue.Pop(marketData) == TRUE)
		{//������ݳɹ�
			marketData_instrumentID = marketData.code;
			for (vector<BaseData>::iterator it = instruments.begin(); it != instruments.end(); ++it)
			{
				if (strcmp(marketData_instrumentID.c_str(), it->code) == 0)
				{//ƥ���ԼID
					//��ΪCTP���ص������������ExchangeIDΪ��, �˴������������
					strcpy_s(marketData.ExchangeID, it->ExchangeID);
				}
			}
			if (strcmp(marketData.ExchangeID, "CFFEX") == 0)
			{//�н������ݴ����н���������view
				POSITION pos = pDoc->GetFirstViewPosition();
				while (pos != NULL)
				{
					CView* pView = pDoc->GetNextView(pos);
					if (pView->IsKindOf(RUNTIME_CLASS(CCFFEXView)))
					{
						CCFFEX_ChildView *CffexView = (CCFFEX_ChildView*)pView;
						if (CffexView->m_pGridCtrl->GetRowCount() != 0)
						{//�����Ѿ���ʼ���������Ŵ��������������ˢ��
							CffexView->UpdateMarketDataView((LPARAM)&marketData);
						}
					}
				}
			}
			else if (strcmp(marketData.ExchangeID, "SHFE") == 0 || strcmp(marketData.ExchangeID, "INE") == 0)
			{
				POSITION pos = pDoc->GetFirstViewPosition();
				while (pos != NULL)
				{
					CView* pView = pDoc->GetNextView(pos);
					if (pView->IsKindOf(RUNTIME_CLASS(CSHFE_INEView)))
					{
						CSHFE_INE_ChildView *CShfe_Ine_View = (CSHFE_INE_ChildView*)pView;
						if (CShfe_Ine_View->m_pGridCtrl->GetRowCount() != 0)
						{//�����Ѿ���ʼ���������Ŵ��������������ˢ��
							CShfe_Ine_View->UpdateMarketDataView((LPARAM)&marketData);
						}
					}
				}
			}
			else if (strcmp(marketData.ExchangeID, "DCE") == 0)
			{
				POSITION pos = pDoc->GetFirstViewPosition();
				while (pos != NULL)
				{
					CView* pView = pDoc->GetNextView(pos);
					if (pView->IsKindOf(RUNTIME_CLASS(CDCEView)))
					{
						CDCE_ChildView *CDce_View = (CDCE_ChildView*)pView;
						if (CDce_View->m_pGridCtrl->GetRowCount() != 0)
						{//�����Ѿ���ʼ���������Ŵ��������������ˢ��
							CDce_View->UpdateMarketDataView((LPARAM)&marketData);
						}
					}
				}
			}
			else if (strcmp(marketData.ExchangeID, "CZCE") == 0)
			{
				POSITION pos = pDoc->GetFirstViewPosition();
				while (pos != NULL)
				{
					CView* pView = pDoc->GetNextView(pos);
					if (pView->IsKindOf(RUNTIME_CLASS(CCZCEView)))
					{
						CCZCE_ChildView *CCZCE_View = (CCZCE_ChildView*)pView;
						if (CCZCE_View->m_pGridCtrl->GetRowCount() != 0)
						{//�����Ѿ���ʼ���������Ŵ��������������ˢ��
							CCZCE_View->UpdateMarketDataView((LPARAM)&marketData);
						}
					}
				}
			}
			else
			{
				//δ֪�쳣,�ݲ�������
			}
		}
	}
}

#endif //_DEBUG


// CDoc ����
