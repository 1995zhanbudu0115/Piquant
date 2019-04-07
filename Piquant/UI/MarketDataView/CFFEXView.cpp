// MultiTraderView.cpp : CMultiTraderView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "../App.h"
#endif

#include "../Doc.h"
#include "CFFEXView.h"
#include "..\GridCtrl\GridCtrl.h"
#include <atlconv.h>//�����ַ�ת��
#include "../../ServiceLayer/CtpMsgDistributor.h"
#include "../../ServiceLayer/CtpService.h"
#include "../../ServiceLayer/DataTypes.h"
#include "../GlobalMgr.h" 
#include <Windows.h> //��ʱ����Ҫ��ͷ�ļ�
#include <time.h>
#include <set>
#include <float.h>
#include "../../ServiceLayer/CtpMsg.h"
#include <map>
#include <utility>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMultiTraderView

IMPLEMENT_DYNCREATE(CCFFEXView, CFormView)

BEGIN_MESSAGE_MAP(CCFFEXView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_MESSAGE(WM_UPDATE_GRIDCTRL,&CCFFEXView::OnUpdateGridCtrl)//����Ϣ����Ϣӳ�亯�����
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CMultiTraderView ����/����


CCFFEXView::CCFFEXView()
	: CFormView(CCFFEXView::IDD)
{
	// TODO:  �ڴ˴���ӹ������
	m_pGridCtrl = NULL;
}

CCFFEXView::~CCFFEXView()
{
	if (NULL != m_pGridCtrl)
	{
		delete m_pGridCtrl;		//�ͷ�ָ��ָ��Ķ���
		m_pGridCtrl = NULL;	//����Ϊ��ָ�룬�ܺõ����ϰ��
	}
}

void CCFFEXView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CCFFEXView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	return CFormView::PreCreateWindow(cs);
}

void CCFFEXView::SetGridCtrlColumns(const vector<ColumnInfo>& header_columns)
{
	if (header_columns.size() <= 0)
	{//�����쳣�жϴ�����ʱֱ��return��
		return;
	}
	//������ͷ��
	m_pGridCtrl->DeleteAllItems();
	m_pGridCtrl->SetRowCount(1);//����
	m_pGridCtrl->SetColumnCount(header_columns.size());//����
	m_pGridCtrl->SetFixedRowCount(1);
	m_pGridCtrl->SetFixedColumnCount(1);
	USES_CONVERSION;
	int row = 0;//ѡ������һ�У�Ҳ������ͷ��
	for (int col = 0; col < m_pGridCtrl->GetColumnCount(); col++)//������ͷ���е�ÿ�����ֶ�
	{
		GV_ITEM Item;
		Item.mask = GVIF_TEXT | GVIF_FORMAT | GVIF_PARAM;
		Item.row = row;
		Item.col = col;
		Item.lParam = header_columns[col].column_id;
		Item.nFormat = DT_LEFT | DT_WORDBREAK;
		Item.strText.Format(_T("%s"), A2T(header_columns[col].column_name.c_str()));//�����е���ͷ�������
		m_pGridCtrl->SetItem(&Item);
		m_pGridCtrl->SetColumnWidth(col, header_columns[col].column_width);//����ÿ�еĿ��
	}
	//m_pGridCtrl->AutoSize();//��Ҫ�Զ����
}

void CCFFEXView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	CString title = _T("�н�");
	GetParent()->SetWindowTextW(title);
	CFormView::OnInitialUpdate();
	ResizeParentToFit();
	get_CtpMsgDistributor()->AddReceiver(this);
	if (m_pGridCtrl == NULL)
	{
		// Create the Gridctrl object
		m_pGridCtrl = new CMarketDataGridCtrl();
		if (!m_pGridCtrl) return;
		// Create the Gridctrl window
		CRect rect;
		GetClientRect(rect);
		m_pGridCtrl->Create(rect, this, 100);
		// fill it up with stuff
		m_pGridCtrl->SetEditable(FALSE);//���ò��ɱ༭
		m_pGridCtrl->EnableDragAndDrop(FALSE);//���ò�����ק
	}
	ConfigReader& cfg_reader = get_config_reader();
	cfg_reader.load("brokers.xml", "config.xml");//��xml�ļ��м��ؾ�������Ϣ����ͷ��Ϣ�����ĺ�Լ��Ϣ�ȣ����ص�������ĳ�Ա�����У������������
	vector<ColumnInfo> header_colums = cfg_reader.GetHeaderColumn("ListCtrl���۱�");
	SetGridCtrlColumns(header_colums);   
}

void CCFFEXView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CCFFEXView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


//���

#ifdef _DEBUG
void CCFFEXView::AssertValid() const
{
	CFormView::AssertValid();
}

void CCFFEXView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CDoc* CCFFEXView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDoc)));
	return (CDoc*)m_pDocument;
}
#endif //_DEBUG


//��Ϣ�������
void CCFFEXView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if (m_pGridCtrl->GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(rect);
		m_pGridCtrl->MoveWindow(rect);
	}
}

BOOL CCFFEXView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_pGridCtrl && IsWindow(m_pGridCtrl->m_hWnd))
		if (m_pGridCtrl->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;
	return CFormView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CCFFEXView::OnReceiveData(WPARAM wParam, LPARAM lParam)
{
	if (wParam == ctp_msg::RspQryInstrument)
	{
		PostMessage(WM_UPDATE_GRIDCTRL, wParam, lParam);//��ʼ���������
	}
}

LRESULT CCFFEXView::OnUpdateGridCtrl(WPARAM wParam, LPARAM lParam)//�����б���
{
	//CFFEX
	if (wParam == RspQryInstrument)
	{//�����Ϻ�����Դ�������ĺ�Լ���ݣ�����������
		vector<BaseData>* instruments_list = reinterpret_cast<vector<BaseData>*>(lParam);
		vector<BaseData> shfe_ine_instrumentlist;
		vector<BaseData> shfe_ine_resort_instrumentlist; //���žۺϺõ�
		set<string> productID_set;
		if (instruments_list != NULL)
		{
			for (int i = 0; i < instruments_list->size(); ++i)
			{//ֻ�����Ϻ�����Դ�������ĺ�Լ����
				BaseData base_data = (*instruments_list)[i];
				CString Exchange_id(base_data.ExchangeID);//��ȡ������
				if (Exchange_id == _T("CFFEX"))
				{
					shfe_ine_instrumentlist.push_back(base_data);
				}
			}
			for (int j = 0;  j < shfe_ine_instrumentlist.size(); ++j)
			{//��ȡ��Ʒ���ͼ���
				productID_set.insert(shfe_ine_instrumentlist[j].ProductID);
			}
			string productID;
			vector<BaseData> instrument_buffer;//��Լ��д���������仺�����һ��Ʒ�����͵ĺ�Լ����
			for (set<string>::iterator it = productID_set.begin(); it != productID_set.end(); ++it)
			{
				productID = *it;
				if (instrument_buffer.size() != 0)
				{//�����
					instrument_buffer.clear();
				}
				for (vector<BaseData>::iterator it = shfe_ine_instrumentlist.begin(); it != shfe_ine_instrumentlist.end(); ++it)
				{//���žۺ�
					string productID_Instrument = "";
					for (int strindex = 0; strindex < strlen((*it).code); ++strindex)
					{
						char a = ((*it).code)[strindex];
						if (isalpha(((*it).code)[strindex]))//�Ƿ�����ĸ
						{
							productID_Instrument += ((*it).code)[strindex];
						}
					}
					if (strcmp(productID_Instrument.c_str(), productID.c_str()) == 0) //��Ʒ������ͬ
					{
						instrument_buffer.push_back(*it); //��ʱ��ѹ�뻺����
					}
				}
				int Min_DeliveryMonth=13;//12�º��1�����������·����޷��ﵽ�����ֵ
				int Min_DeliveryYear = 9999;
				BaseData instrument_timesort;
				int resort_num = instrument_buffer.size();
				for (int k = 0; k < resort_num; ++k)
				{//�ж��ٸ�Ԫ�ؾͱ���Ƚ�������ٴ�
					Min_DeliveryMonth = 13;//����
					Min_DeliveryYear = 9999;
					for (vector<BaseData>::iterator it = instrument_buffer.begin(); it != instrument_buffer.end(); ++it)
					{//����ݼ��·�����
						if (it->DeliveryYear < Min_DeliveryYear)
						{
							Min_DeliveryYear = it->DeliveryYear;
							Min_DeliveryMonth = it->DeliveryMonth;
							instrument_timesort = *it;
						}
						else if (it->DeliveryYear == Min_DeliveryYear)
						{
							if (it->DeliveryMonth < Min_DeliveryMonth)
							{
								Min_DeliveryYear = it->DeliveryYear;
								Min_DeliveryMonth = it->DeliveryMonth;
								instrument_timesort = *it;
							}
						}
						else
						{//���ڵģ�ֱ��continue
							continue;
						}
					}
					shfe_ine_resort_instrumentlist.push_back(instrument_timesort);
					for (vector<BaseData>::iterator it = instrument_buffer.begin(); it != instrument_buffer.end(); )
					{//���Ѿ�ѹ��shfe_ine_resort_instrumentlist�У�������Ƚϵĺ�Լ���ݴӻ�������ɾ��
						if (strcmp(it->code, instrument_timesort.code) == 0)
						{
							it = instrument_buffer.erase(it);
						}
						else
						{
							it++;
						}
					}
				}
			}
		}

		if (shfe_ine_resort_instrumentlist.size()!=0)
		{
			for (int i = 0; i < shfe_ine_resort_instrumentlist.size(); ++i)
			{
				BaseData base_data = shfe_ine_resort_instrumentlist[i];
				CString Instrument_id(base_data.code);//��ȡ��ͷ����ԼID
				int row = m_pGridCtrl->GetRowCount();//Ĭ�ϱ�β���������
				bool bFined = false;
				for (; row <= m_pGridCtrl->GetRowCount(); row++)//�ӵ�һ�п�ʼȥ�ң���Ϊ��0������ͷ��
				{
					CString row_data = m_pGridCtrl->GetItemText(row, 0);//�Ƚϸ�����ͷ��Instrument_id�Ƿ�һ��
					CString row_Instrument_id(row_data);
					if (row_Instrument_id == Instrument_id)
					{
						bFined = true;
						break;//ƥ��ɹ���break��ȥ��˵����ʱ�Ѿ������˸ú�Լ��Ϣ��
					}
				}
				//�����ֶ�
				CString disp_str;
				for (int col = 0; col < m_pGridCtrl->GetColumnCount(); col++)
				{
					GV_ITEM Item;
					Item.mask = GVIF_TEXT | GVIF_FORMAT;
					Item.row = row;//ͨ����һ��ѭ�������ҵ���row�Ѿ�����,����δ�ҵ������ĩβ�е���һ��
					Item.col = col;//��ͬ�����������к�,ָ��item��������
					//Item.lParam = (LPARAM)CString(base_data.code).GetBuffer();

					if (m_pGridCtrl->GetItemText(0, col)==_T("��Լ"))
					{
						disp_str = base_data.code;
					}
					else if (m_pGridCtrl->GetItemText(0, col) == _T("��Լ��"))
					{
						disp_str = base_data.InstrumentName;
					}
					else if (m_pGridCtrl->GetItemText(0, col) == _T("������"))
					{
						disp_str = base_data.ExchangeID;
					}
					else
					{//�����ֶβ������︳ֵ����ֱ��continue
						continue;
					}
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
				}
				m_pGridCtrl->Refresh();
			}
		}
		
	}
	return 0;
}

void CCFFEXView::OnDraw(CDC* pDC)
{
	CDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
	{
		return;
	}
}


void CCFFEX_ChildView::UpdateMarketDataView(LPARAM lParam)
{
	MarketData* pMarketData = reinterpret_cast<MarketData*>(lParam);
	MarketData marketData = *pMarketData;
	CString marketData_instrumentID(marketData.code);
	int matchrow = -1;//��¼ƥ����к�
	for (int row = 1; row <= m_pGridCtrl->GetRowCount(); row++)//�ӵ�һ�п�ʼȥ�ң���Ϊ��0������ͷ��
	{
		CString row_data = m_pGridCtrl->GetItemText(row, 0);//�Ƚϸ�����ͷ��Instrument_id�Ƿ�һ��
		CString row_Instrument_id(row_data);
		if (row_Instrument_id == marketData_instrumentID)
		{
			matchrow = row;
			break;//ƥ��ɹ���break��ȥ��˵����ʱ�Ѿ������˸ú�Լ��Ϣ��
		}
	}
	if (matchrow == -1) //����δ֪��ĳ���쳣,��Ȼ�޷�ƥ��ɹ�
	{
		return;
	}
	CString disp_str;
	for (int col = 0; col < m_pGridCtrl->GetColumnCount(); col++)
	{
		GV_ITEM Item;
		Item.mask = GVIF_TEXT | GVIF_FORMAT;
		Item.row = matchrow;
		Item.col = col;
		if (m_pGridCtrl->GetItemText(0, col) == _T("���¼�"))
		{
			if (marketData.LastPrice == DBL_MAX)
			{
				disp_str = _T("��");
			}
			else
			{
				char lastprice_cstr[100];
				sprintf(lastprice_cstr, "%0.2f", marketData.LastPrice);
				disp_str = lastprice_cstr;
				m_pGridCtrl->SetItemBkColour(matchrow, col, RGB(255, 152, 49));//�ȵ׺���
			}
		}
		else if (m_pGridCtrl->GetItemText(0, col) == _T("��һ��"))
		{
			if (marketData.AskPrice == DBL_MAX)
			{
				disp_str = _T("��");
			}
			else
			{
				char askprice_cstr[100];
				sprintf(askprice_cstr, "%0.2f", marketData.AskPrice);
				disp_str = askprice_cstr;
			}
		}
		else if (m_pGridCtrl->GetItemText(0, col) == _T("��һ��"))
		{
			char askvolume_cstr[100];
			sprintf(askvolume_cstr, "%d", marketData.AskVolume);
			disp_str = askvolume_cstr;
		}
		else if (m_pGridCtrl->GetItemText(0, col) == _T("��һ��"))
		{
			if (marketData.BidPrice == DBL_MAX)
			{
				disp_str = _T("��");
			}
			else
			{
				char bidprice_cstr[100];
				sprintf(bidprice_cstr, "%0.2f", marketData.BidPrice);
				disp_str = bidprice_cstr;
			}
		}
		else if (m_pGridCtrl->GetItemText(0, col) == _T("��һ��"))
		{
			char bidvolume_cstr[100];
			sprintf(bidvolume_cstr, "%d", marketData.BidVolume);
			disp_str = bidvolume_cstr;
		}
		else if (m_pGridCtrl->GetItemText(0, col) == _T("��ͣ��"))
		{
			if (marketData.UpperLimitPrice == DBL_MAX)
			{
				disp_str = _T("��");
			}
			else
			{
				char UpperLimitPrice_cstr[100];
				sprintf(UpperLimitPrice_cstr, "%0.2f", marketData.UpperLimitPrice);
				disp_str = UpperLimitPrice_cstr;
				m_pGridCtrl->SetItemBkColour(matchrow, col, RGB(255, 255, 0));
				m_pGridCtrl->SetItemFgColour(matchrow, col, RGB(255, 0, 0));
			}
		}
		else if (m_pGridCtrl->GetItemText(0, col) == _T("��ͣ��"))
		{
			if (marketData.LowerLimitPrice == DBL_MAX)
			{
				disp_str = _T("��");
			}
			else
			{
				char LowerLimitPrice_cstr[100];
				sprintf(LowerLimitPrice_cstr, "%0.2f", marketData.LowerLimitPrice);
				disp_str = LowerLimitPrice_cstr;
				m_pGridCtrl->SetItemBkColour(matchrow, col, RGB(255, 255, 0));
				m_pGridCtrl->SetItemFgColour(matchrow, col, RGB(0, 128, 128));
			}
		}
		else if (m_pGridCtrl->GetItemText(0, col) == _T("�ɽ���"))
		{
			char Volume[100];
			sprintf(Volume, "%d", marketData.Volume);
			disp_str = Volume;
		}
		else if (m_pGridCtrl->GetItemText(0, col) == _T("������"))
		{
			if (marketData.PreClosePrice == DBL_MAX)
			{
				disp_str = _T("��");
			}
			else
			{
				char PreClosePrice[100];
				sprintf(PreClosePrice, "%0.2f", marketData.PreClosePrice);
				disp_str = PreClosePrice;
			}
		}
		else if (m_pGridCtrl->GetItemText(0, col) == _T("����"))
		{
			if (marketData.OpenPrice == DBL_MAX)
			{
				disp_str = _T("��");
			}
			else
			{
				char OpenPrice[100];
				sprintf(OpenPrice, "%0.2f", marketData.OpenPrice);
				disp_str = OpenPrice;
			}
		}
		else if (m_pGridCtrl->GetItemText(0, col) == _T("��߼�"))
		{
			if (marketData.HighestPrice == DBL_MAX)
			{
				disp_str = _T("��");
			}
			else
			{
				char HighestPrice[100];
				sprintf(HighestPrice, "%0.2f", marketData.HighestPrice);
				disp_str = HighestPrice;
			}
		}
		else if (m_pGridCtrl->GetItemText(0, col) == _T("��ͼ�"))
		{
			if (marketData.LowestPrice == DBL_MAX)
			{
				disp_str = _T("��");
			}
			else
			{
				char LowestPrice[100];
				sprintf(LowestPrice, "%0.2f", marketData.LowestPrice);
				disp_str = LowestPrice;
			}
		}
		else if (m_pGridCtrl->GetItemText(0, col) == _T("������"))
		{//����֪ͨ�У��������ۡ����̼ۡ������۳���double����ֵ�����ʾ���ֶ�û��ֵ��������ͣ���ʱ����Ϊû�����ۣ������һ��double����ֵ��ͬʱ����Ϊ0��
			if (marketData.ClosePrice == DBL_MAX)
			{
				disp_str = _T("��");
			}
			else
			{
				char ClosePrice[100];
				sprintf(ClosePrice, "%0.2f", marketData.ClosePrice);
				disp_str = ClosePrice;
			}
		}
		else if (m_pGridCtrl->GetItemText(0, col) == _T("�����"))
		{
			if (marketData.PreSettlementPrice == DBL_MAX)
			{
				disp_str = _T("��");
			}
			else
			{
				char PreSettlementPrice[100];
				sprintf(PreSettlementPrice, "%0.2f", marketData.PreSettlementPrice);
				disp_str = PreSettlementPrice;
			}
		}
		else if (m_pGridCtrl->GetItemText(0, col) == _T("�����"))
		{//����֪ͨ�У��������ۡ����̼ۡ������۳���double����ֵ�����ʾ���ֶ�û��ֵ��������ͣ���ʱ����Ϊû�����ۣ������һ��double����ֵ��ͬʱ����Ϊ0��
			if (marketData.SettlementPrice == DBL_MAX)
			{
				disp_str = _T("��");
			}
			else
			{
				char SettlementPrice[100];
				sprintf(SettlementPrice, "%0.2f", marketData.SettlementPrice);
				disp_str = SettlementPrice;
			}
		}
		else if (m_pGridCtrl->GetItemText(0, col) == _T("�������ʱ��"))
		{
			disp_str = marketData.UpdateTime;
		}
		else if (m_pGridCtrl->GetItemText(0, col) == _T("�ǵ�"))
		{
			if (marketData.LastPrice == DBL_MAX || marketData.PreClosePrice==DBL_MAX)
			{
				disp_str = _T("��");
			}
			else
			{
				double range = marketData.LastPrice - marketData.PreClosePrice;
				char range_cstr[100];
				sprintf(range_cstr, "%0.2f", range);
				disp_str = range_cstr;
				if (range >= 0)
				{
					m_pGridCtrl->SetItemBkColour(matchrow, col, RGB(255, 0, 0));
				}
				else
				{
					m_pGridCtrl->SetItemBkColour(matchrow, col, RGB(0, 255, 0));
				}
			}
		}
		else if (m_pGridCtrl->GetItemText(0, col) == _T("�ǵ���"))
		{
			if (marketData.LastPrice == DBL_MAX || marketData.PreClosePrice == DBL_MAX)
			{
				disp_str = _T("��");
			}
			else
			{
				double range = 0;
				if (marketData.PreClosePrice != 0)
				{
					range = (marketData.LastPrice - marketData.PreClosePrice) / marketData.PreClosePrice * 100;
					char range_cstr[100];
					sprintf(range_cstr, "%0.2f%%", range);
					disp_str = range_cstr;
				}
				else
				{
					disp_str = _T("---");
				}
				if (range >= 0)
				{
					m_pGridCtrl->SetItemBkColour(matchrow, col, RGB(255, 0, 0));
				}
				else
				{
					m_pGridCtrl->SetItemBkColour(matchrow, col, RGB(0, 255, 0));
				}
			}
		}
		else
		{//�����ֶβ������︳ֵ����ֱ��continue
			continue;
		}
		Item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;
		Item.strText.Format(_T("%s"), disp_str);
		m_pGridCtrl->SetItem(&Item);
	}
	m_pGridCtrl->Refresh();
}


void CCFFEXView::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	get_CtpMsgDistributor()->DelReceiver(this);
	__super::OnClose();
}