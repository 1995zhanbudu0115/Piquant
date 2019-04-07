
// ShowInstrumentsView.cpp : implementation of the CShowInstrumentsView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "../App.h"//�����
#endif

#include "ShowInstrumentsView.h"
#include "../Doc.h"
#include "../../ServiceLayer/CtpMsgDistributor.h"
#include "../../ServiceLayer/DataTypes.h"
#include "../../ServiceLayer/CtpService.h"
#include "../../ServiceLayer/CtpMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CShowInstrumentsView

IMPLEMENT_DYNCREATE(CShowInstrumentsView, CView)

BEGIN_MESSAGE_MAP(CShowInstrumentsView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CShowInstrumentsView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_MESSAGE(WM_UPDATE_GRIDCTRL, &CShowInstrumentsView::OnUpdateGridCtrl)//����Ϣ����Ϣӳ�亯�����
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CShowInstrumentsView construction/destruction

CShowInstrumentsView::CShowInstrumentsView()
{
	// TODO: add construction code here
	m_pGridCtrl = NULL;

}

CShowInstrumentsView::~CShowInstrumentsView()
{
	if (NULL != m_pGridCtrl)
	{
		delete m_pGridCtrl;//�ͷ�ָ��ָ��Ķ���
		m_pGridCtrl = NULL;//����Ϊ��ָ�룬�ܺõ����ϰ��
	}
}

BOOL CShowInstrumentsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CShowInstrumentsView drawing

void CShowInstrumentsView::OnDraw(CDC* /*pDC*/)
{
	CDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CShowInstrumentsView printing


void CShowInstrumentsView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CShowInstrumentsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CShowInstrumentsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CShowInstrumentsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CShowInstrumentsView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CShowInstrumentsView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CShowInstrumentsView diagnostics

#ifdef _DEBUG
void CShowInstrumentsView::AssertValid() const
{
	CView::AssertValid();
}

void CShowInstrumentsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDoc* CShowInstrumentsView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDoc)));
	return (CDoc*)m_pDocument;
}

void CShowInstrumentsView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	CString title = _T("��Լ��Ϣ");
	GetParent()->SetWindowTextW(title); 
	get_CtpMsgDistributor()->AddReceiver(this);//��CShowInstrumentsView��ʵ����ӽ�������������
	//�����¿�ʼд���Ի�����:
	if (m_pGridCtrl == NULL)
	{
		// Create the Gridctrl object
		m_pGridCtrl = new CGridCtrl;
		if (!m_pGridCtrl) return;
		// Create the Gridctrl window
		CRect rect;
		GetClientRect(rect);
		m_pGridCtrl->Create(rect, this, 100);
		// fill it up with stuff
		m_pGridCtrl->SetEditable(FALSE);//���ÿɱ༭
		m_pGridCtrl->EnableDragAndDrop(FALSE);//���ÿ���ק

		ConfigReader& cfg_reader = get_config_reader();
		cfg_reader.load("brokers.xml", "config.xml");//��xml�ļ��м��ؾ�������Ϣ����ͷ��Ϣ�����ĺ�Լ��Ϣ�ȣ����ص�������ĳ�Ա�����У������������
		vector<ColumnInfo> header_colums = cfg_reader.GetHeaderColumn("ListCtrl��Լ��Ϣ");//���ݱ���,��ȡ���ڸ�����ͷ�ṹ��ColumnInfo,�ϼ�Ϊvector
		if (header_colums.size() <= 0)
		{//�����쳣�жϴ�������xml�����ļ������ԭ�򣬶�ȡ������Ϣ����ʱֱ��return��
			return;
		}
		try {
			m_pGridCtrl->SetRowCount(1);//����
			m_pGridCtrl->SetColumnCount(header_colums.size());//����
			m_pGridCtrl->SetFixedRowCount(1);
			m_pGridCtrl->SetFixedColumnCount(1);
		}
		catch (CMemoryException* e)
		{
			e->ReportError();
			e->Delete();
			return;
		}
		USES_CONVERSION;
		// ��ʼ����ʱ���γ���ͷ��
		int row = 0;//ѡ������һ�У�Ҳ������ͷ��
		for (int col = 0; col < m_pGridCtrl->GetColumnCount(); col++)//������ͷ���е�ÿ�����ֶ�
		{
			GV_ITEM Item;
			Item.mask = GVIF_TEXT | GVIF_FORMAT | GVIF_PARAM;
			Item.row = row;
			Item.col = col;
			Item.lParam = header_colums[col].column_id;
			Item.nFormat = DT_LEFT | DT_WORDBREAK;
			Item.strText.Format(_T("%s"), A2T(header_colums[col].column_name.c_str()));//�����е���ͷ�������
			m_pGridCtrl->SetItem(&Item);
			m_pGridCtrl->SetColumnWidth(col, header_colums[col].column_width);//����ÿ�еĿ��
		}
		//m_pGridCtrl->AutoSize();//��Ҫ�Զ����
	}
}

LRESULT CShowInstrumentsView::OnUpdateGridCtrl(WPARAM wParam, LPARAM lParam)
{
	if (wParam == ctp_msg::RspQryInstrument)
	{
		vector<BaseData>* instruments_list = reinterpret_cast<vector<BaseData>*>(lParam);
		if (instruments_list != NULL)
		{
			for (int i = 0; i < instruments_list->size(); ++i)
			{
				BaseData base_data = (*instruments_list)[i];
				CString Instrument_id(base_data.code);//��ȡ��ͷ
				// fill rows/cols with text
				int row = 1;
				bool bFined = false;
				for (; row < m_pGridCtrl->GetRowCount(); row++)//�ӵ�һ�п�ʼȥ�ң���Ϊ��0������ͷ��
				{
					LPARAM row_data = m_pGridCtrl->GetItemData(row, 0);//�Ƚϸ�����ͷ��Instrument_id�Ƿ�һ��
					CString row_Instrument_id((TCHAR*)row_data);
					if (row_Instrument_id == Instrument_id)
					{
						bFined = true;
						break;//ƥ��ɹ���break��ȥ��˵����ʱ�Ѿ������˸ú�Լ��Ϣ��
					}
				}
				//ÿһ�ж�Ӧһ���ֶΣ���ʼ�������û���������Ϣ
				for (int col = 0; col < m_pGridCtrl->GetColumnCount(); col++)
				{
					GV_ITEM Item;
					Item.mask = GVIF_TEXT | GVIF_FORMAT;
					Item.row = row;//ͨ����һ��ѭ�������ҵ���row�Ѿ�����,����δ�ҵ������ĩβ�е���һ��,ָ��item��������
					Item.col = col;//��ͬ�����������к�,ָ��item��������
					//Item.lParam = (LPARAM)CString(base_data.code).GetBuffer();
					int field_id = m_pGridCtrl->GetItemData(0, col);//�ӱ�ͷ�ĸ��л�ȡfield_id,����Ϊ��0��
					CString disp_str(GetTextById(base_data, field_id).c_str());//����field_id��ȡҪ���ֵ
					Item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;
					Item.strText.Format(_T("%s"), disp_str);
					if (bFined)//���ӱ��п����ҵ��Ѵ��иú�Լ����Ϣ����ֻ��ˢ��
					{
						m_pGridCtrl->SetItem(&Item);
					}
					else
					{
						row = m_pGridCtrl->InsertRow(Instrument_id);//����һ��,��һ�е����ݾ��Ǻ�Լ��
						bFined = true;
					}
					if (FIELD_Code==field_id)//����ʱ��ˢ����ͷ��Լ������
					{
						m_pGridCtrl->SetItemBkColour(row, col, RGB(255, 154, 53)); //���ñ���ɫ
						m_pGridCtrl->SetItemFgColour(row, col, RGB(0, 0, 255)); //����ǰ��ɫ
					}
				}
				m_pGridCtrl->Refresh();
			}
		}
	}
	return 0;
}

void CShowInstrumentsView::OnReceiveData(WPARAM wParam, LPARAM lParam)
{
	if (wParam == ctp_msg::RspQryInstrument)
	{//�˴�������Ϣ��ֻ����ͼ����
		PostMessage(WM_UPDATE_GRIDCTRL, wParam, lParam);//WM_UPDATE_GRIDCTRL��Ϣ���ͺ��������Ϣ������OnUpdateGridCtrl�Ը�����ͼ����
	}
}

#endif //_DEBUG


// CShowInstrumentsView message handlers


void CShowInstrumentsView::OnSize(UINT nType, int cx, int cy)
{

	__super::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (m_pGridCtrl->GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(rect);
		m_pGridCtrl->MoveWindow(rect);
	}
}
