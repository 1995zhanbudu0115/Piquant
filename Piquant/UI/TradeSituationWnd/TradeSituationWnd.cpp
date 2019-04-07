
#include "stdafx.h"

#include "TradeSituationWnd.h"
#include "../Resource.h"
#include "../MainFrm.h"
#include "../../ServiceLayer/DataTypes.h"
#include <atlconv.h>//�����ַ�ת��
#include "../GlobalMgr.h"
#include "../../ServiceLayer/CtpMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

COutputWnd::COutputWnd()
{
}

COutputWnd::~COutputWnd()
{
}

BEGIN_MESSAGE_MAP(COutputWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(WM_UPDATE_GRIDCTRL, &COutputWnd::OnUpdateGridCtrl)
END_MESSAGE_MAP()

int COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	get_CtpMsgDistributor()->AddReceiver(this);//��COutputWnd�������ӽ�������������

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// ����ѡ�����: 
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("δ�ܴ����������\n");
		return -1;      // δ�ܴ���
	}

	// ����3�����������: 
	const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;
	if (!m_wndOrderList.Create(rectDummy, &m_wndTabs, ID_WNDORDERLIST, dwStyle) ||
		!m_wndPosition.Create(rectDummy, &m_wndTabs, ID_WNDPOSITION, dwStyle) ||
		!m_wndTrade.Create(rectDummy, &m_wndTabs, ID_WNDTRADE, dwStyle)||
		!m_wndUnfilledOrderList.Create(rectDummy, &m_wndTabs, ID_WNDUNFILLEDORDERLIST, dwStyle)||
		!m_wndPositionDetail.Create(rectDummy, &m_wndTabs, ID_WNDPOSITIONDETAIL, dwStyle)||
		!m_wndCloseTrade.Create(rectDummy, &m_wndTabs, ID_WNDCLOSETRADE, dwStyle) ||
		!m_wndParkedOrderList.Create(rectDummy, &m_wndTabs, ID_WNDPARKEDORDERLIST, dwStyle) ||
		!m_wndParkedOrderActionList.Create(rectDummy, &m_wndTabs, ID_WNDPARKEDORDERACTIONLIST, dwStyle) ||
		!m_wndConditionOrderList.Create(rectDummy, &m_wndTabs, ID_WNDCONDITIONORDER, dwStyle))
	{//1999��ӦResource.h�е���Դ��ID_WNDPOSITION�� 1998��ӦID_WNDTRADE��1997��ӦID_WNDUNFILLEDORDERLIST   
		TRACE0("δ�ܴ����������\n");
		return -1;      // δ�ܴ���
	}

	ConfigReader& cfg_reader = get_config_reader();
	cfg_reader.load("brokers.xml", "config.xml");
	vector<ColumnInfo> header_colums = cfg_reader.GetHeaderColumn("ListCtrl����ί�е�");
	m_wndOrderList.LoadColumns(header_colums);
	m_wndUnfilledOrderList.LoadColumns(header_colums);
	header_colums = cfg_reader.GetHeaderColumn("ListCtrl�ֲ�");
	m_wndPosition.LoadColumns(header_colums);
	header_colums = cfg_reader.GetHeaderColumn("ListCtrl�ɽ���¼");
	m_wndTrade.LoadColumns(header_colums);
	header_colums = cfg_reader.GetHeaderColumn("ListCtrl�ֲ���ϸ");
	m_wndPositionDetail.LoadColumns(header_colums);
	header_colums = cfg_reader.GetHeaderColumn("ListCtrlƽ���б�");
	m_wndCloseTrade.LoadColumns(header_colums);
	header_colums = cfg_reader.GetHeaderColumn("ListCtrlԤ���б�");
	m_wndParkedOrderList.LoadColumns(header_colums);
	header_colums = cfg_reader.GetHeaderColumn("ListCtrlԤ�񳷵��б�");
	m_wndParkedOrderActionList.LoadColumns(header_colums);
	header_colums = cfg_reader.GetHeaderColumn("ListCtrl�������б�");
	m_wndConditionOrderList.LoadColumns(header_colums);
	UpdateFonts();

	CString strTabName;
	BOOL bNameValid;

	// ���б��ڸ��ӵ�ѡ�: 
	bNameValid = strTabName.LoadString(IDS_POSITION_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndPosition, strTabName, (UINT)1);

	bNameValid = strTabName.LoadString(IDS_POSITIONDETAIL_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndPositionDetail, strTabName, (UINT)4);

	bNameValid = strTabName.LoadString(IDS_ORDER_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOrderList, strTabName, (UINT)0);

	bNameValid = strTabName.LoadString(IDS_UNFILLEDORDER_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndUnfilledOrderList, strTabName, (UINT)3);

	bNameValid = strTabName.LoadString(IDS_TRADED_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndTrade, strTabName, (UINT)2);

	bNameValid = strTabName.LoadString(IDS_CLOSETRADED_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndCloseTrade, strTabName, (UINT)5);

	bNameValid = strTabName.LoadString(IDS_CONDITIONORDER_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndConditionOrderList, strTabName, (UINT)8);

	bNameValid = strTabName.LoadString(IDS_PARKEDORDER_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndParkedOrderList, strTabName, (UINT)6);

	bNameValid = strTabName.LoadString(IDS_PARKEDORDERACTION_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndParkedOrderActionList, strTabName, (UINT)7);

	// ʹ��һЩ�����ı���д���ѡ�(���踴������)
	FillBuildWindow();
	FillDebugWindow();
	FillFindWindow();

	return 0;
}

void COutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// ѡ��ؼ�Ӧ��������������: 
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void COutputWnd::AdjustHorzScroll(CListBox& wndListBox)
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);

	int cxExtentMax = 0;

	for (int i = 0; i < wndListBox.GetCount(); i ++)
	{
		CString strItem;
		wndListBox.GetText(i, strItem);

		cxExtentMax = max(cxExtentMax, (int)dc.GetTextExtent(strItem).cx);
	}

	wndListBox.SetHorizontalExtent(cxExtentMax);
	dc.SelectObject(pOldFont);
}

void COutputWnd::FillBuildWindow()
{
	//m_wndOutputBuild.AddString(_T("�����������ʾ�ڴ˴���"));
	//m_wndOutputBuild.AddString(_T("�������ʾ���б���ͼ������"));
	//m_wndOutputBuild.AddString(_T("�������Ը�����Ҫ��������ʾ��ʽ..."));
}

void COutputWnd::FillDebugWindow()
{
	//m_wndOutputDebug.AddString(_T("�����������ʾ�ڴ˴���"));
	//m_wndOutputDebug.AddString(_T("�������ʾ���б���ͼ������"));
	//m_wndOutputDebug.AddString(_T("�������Ը�����Ҫ��������ʾ��ʽ..."));
}

void COutputWnd::FillFindWindow()
{
	//m_wndOutputFind.AddString(_T("�����������ʾ�ڴ˴���"));
	//m_wndOutputFind.AddString(_T("�������ʾ���б���ͼ������"));
	//m_wndOutputFind.AddString(_T("�������Ը�����Ҫ��������ʾ��ʽ..."));
}

void COutputWnd::UpdateFonts()
{
	m_wndOrderList.SetFont(&afxGlobalData.fontRegular);
	m_wndPosition.SetFont(&afxGlobalData.fontRegular);
	m_wndTrade.SetFont(&afxGlobalData.fontRegular);
	m_wndUnfilledOrderList.SetFont(&afxGlobalData.fontRegular);
	m_wndPositionDetail.SetFont(&afxGlobalData.fontRegular);
	m_wndCloseTrade.SetFont(&afxGlobalData.fontRegular);
	m_wndParkedOrderList.SetFont(&afxGlobalData.fontRegular);
	m_wndParkedOrderActionList.SetFont(&afxGlobalData.fontRegular);
	m_wndConditionOrderList.SetFont(&afxGlobalData.fontRegular);
}

void COutputWnd::OnReceiveData(WPARAM wParam, LPARAM lParam)
{
	if (wParam == ctp_msg::RspQryInvestorPosition)
	{//���յ��ֲֻ�������Ϣ��ȥˢ����Ӧ�ĳֲֽ���
		SendMessage(WM_UPDATE_GRIDCTRL, wParam, lParam);
	}
	if (wParam==ctp_msg::RspQryOrder)
	{//���յ�ί�л�������Ϣ��ȥˢ����Ӧ�ĳֲֽ���
		SendMessage(WM_UPDATE_GRIDCTRL, wParam, lParam);
	}
	if (wParam == ctp_msg::RtnOrder)
	{//���յ�ί�л�������Ϣ��ȥˢ����Ӧ�ĳֲֽ���
		SendMessage(WM_UPDATE_GRIDCTRL, wParam, lParam);
	}
	if (wParam == ctp_msg::RspQryTrade)
	{//���յ�ί�л�������Ϣ��ȥˢ����Ӧ�ĳֲֽ���
		SendMessage(WM_UPDATE_GRIDCTRL, wParam, lParam);
	}
	if (wParam == ctp_msg::RtnTrade)
	{
		SendMessage(WM_UPDATE_GRIDCTRL, wParam, lParam);
	}
	if (wParam==ctp_msg::RspQryInvestorPositionDetail)
	{
		SendMessage(WM_UPDATE_GRIDCTRL, wParam, lParam);
	}
	if (wParam == ctp_msg::RspQryParkedOrder)
	{
		SendMessage(WM_UPDATE_GRIDCTRL, wParam, lParam);
	}
	if (wParam == ctp_msg::RspParkedOrderInsert)
	{
		SendMessage(WM_UPDATE_GRIDCTRL, wParam, lParam);
	}
	if (wParam == ctp_msg::RspRemoveParkedOrder)
	{
		SendMessage(WM_UPDATE_GRIDCTRL, wParam, lParam);
	}
	if (wParam == ctp_msg::RspQryParkedOrderAction)
	{
		SendMessage(WM_UPDATE_GRIDCTRL, wParam, lParam);
	}
	if (wParam == ctp_msg::RspParkedOrderAction)
	{
		SendMessage(WM_UPDATE_GRIDCTRL, wParam, lParam);
	}
	if (wParam == ctp_msg::RspRemoveParkedOrderAction)
	{
		SendMessage(WM_UPDATE_GRIDCTRL, wParam, lParam);
	}
}

LRESULT COutputWnd::OnUpdateGridCtrl(WPARAM wParam, LPARAM lParam)
{
	//�ֲ�ͬ��Ϣȥ���´���
	if (wParam == ctp_msg::RspQryInvestorPosition)
	{
		vector<InvestorPosition>* InvestorPositions_list = reinterpret_cast<vector<InvestorPosition>*>(lParam);
		vector<InvestorPosition> InvestorPositions_list_merged; //�����ϲ�����ĳֲ�vector
		CString Instrument_id_mergedvector;
		CString direction_mergedvector;
		InvestorPosition position_merged;
		if (InvestorPositions_list!=NULL)
		{
			for (int i = 0; i < InvestorPositions_list->size(); ++i)
			{
				InvestorPosition Position = (*InvestorPositions_list)[i];
				CString Instrument_id(Position.InstrumentID.c_str());//��ȡ��ͷ
				CString direction(Position.PosiDirection);//��char��cstring���Զ�ת��
				position_merged = Position;
				vector<InvestorPosition>::iterator it;
				for ( it = InvestorPositions_list_merged.begin(); it != InvestorPositions_list_merged.end(); )
				{
					Instrument_id_mergedvector = (*it).InstrumentID.c_str();//��ȡ��ͷ
					direction_mergedvector = (*it).PosiDirection;                //��char��cstring���Զ�ת��
					if (Instrument_id == Instrument_id_mergedvector && direction == direction_mergedvector)
					{
						//�ȴ�����ô������, ��Щ������ϲ�,��Щ���ݲ���ϲ�,���ڿ�����
						position_merged.PositionProfit += (*it).PositionProfit;
						position_merged.Position += (*it).Position;
						position_merged.YdPosition += (*it).YdPosition;
						position_merged.TodayPosition += (*it).TodayPosition;
						position_merged.UseMargin += (*it).UseMargin;
						position_merged.OpenCost += (*it).OpenCost;
						//�ϲ����,��ԭ�е�ɾ����
						it=InvestorPositions_list_merged.erase(it);
					}
					//��ֹ����iterator not incrementable�Ľ���취
					else
					{
						++it;
					}
				}
				InvestorPositions_list_merged.push_back(position_merged); //�������ϲ��ĳֲ�ѹ��
			}
		}

		if (InvestorPositions_list_merged.size()!=0)
		{
			for (int i = 0; i < InvestorPositions_list_merged.size(); ++i)
			{
				InvestorPosition Position = InvestorPositions_list_merged[i];
				CString Instrument_id(Position.InstrumentID.c_str());//��ȡ��ͷ
				CString direction(Position.PosiDirection);//��char��cstring���Զ�ת��
				USES_CONVERSION;
				string direction_stringtype = CT2A(direction.GetBuffer()); //��cstringת��Ϊstring
				// fill rows/cols with text
				int row = 1;
				bool bFined = false;

				for (; row <= m_wndPosition.GetRowCount(); row++)//�ӵ�һ�п�ʼȥ�ң���Ϊ��0������ͷ��
				{
					CString row_data = m_wndPosition.GetItemText(row, 0);//�Ƚϸ�����ͷ��Instrumentid�Ƿ�һ��
					CString row_Instrument_id(row_data);

					row_data = m_wndPosition.GetItemText(row, 2);//�˴�Ҫע���2��λ�ñ�������������
					CString row_direction(row_data);

					USES_CONVERSION;
					string row_direction_stringtype = CT2A(row_direction.GetBuffer()); //��cstringת��Ϊstring

					if (row_Instrument_id == Instrument_id)
					{
						if ((row_direction_stringtype == "��" && direction_stringtype == "3") || (row_direction_stringtype == "��" && direction_stringtype == "2") || (row_direction_stringtype == "��" && direction_stringtype == "1"))
						{
							bFined = true;
							break;//ƥ��ɹ���break��ȥ����ʱ��row��������Ҫ�ҵ��û�����Ϣ
						}
					}
				}
				//ÿһ�ж�Ӧһ���ֶΣ���ʼ�������û���������Ϣ
				for (int col = 0; col < m_wndPosition.GetColumnCount(); col++)
				{
					GV_ITEM Item;
					Item.mask = GVIF_TEXT | GVIF_FORMAT;
					Item.row = row;//ͨ����һ��ѭ�������ҵ���row�Ѿ�����,����δ�ҵ������ĩβ�е���һ��,ָ��item��������
					Item.col = col;//��ͬ�����������к�,ָ��item��������
					//Item.lParam = (LPARAM)CString(base_data.code).GetBuffer();
					int field_id = m_wndPosition.GetItemData(0, col);//�ӱ�ͷ�ĸ��л�ȡfield_id,����Ϊ��0��
					CString disp_str(GetTextById(Position, field_id).c_str());//����field_id��ȡҪ���ֵ
					Item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;
					Item.strText.Format(_T("%s"), disp_str);
					if (bFined)//���ӱ��п����ҵ��Ѵ��иú�Լ����Ϣ����ֻ��ˢ��
					{
						m_wndPosition.SetItem(&Item);
					}
					else
					{
						row = m_wndPosition.InsertRow(Instrument_id);//����һ��,��һ�е����ݾ��Ǻ�Լ��
						bFined = true;
					}
					if (FIELD_ID_InvestorPosition::FIELD_FloatProfit_Position == field_id)//����ʱ��ˢ����ͷ��Լ������
					{
						string floatprofit_str = CT2A(m_wndPosition.GetItemText(row, col).GetBuffer());
						double floatprofit = stod(floatprofit_str);
						if (floatprofit <= 0)
						{
							m_wndPosition.SetItemBkColour(row, col, RGB(0, 255, 0)); //���ñ���ɫ
							m_wndPosition.SetItemFgColour(row, col, RGB(0, 0, 0)); //����ǰ��ɫ
						}
						if (floatprofit > 0)
						{
							m_wndPosition.SetItemBkColour(row, col, RGB(255, 0, 0)); //���ñ���ɫ
							m_wndPosition.SetItemFgColour(row, col, RGB(0, 0, 0)); //����ǰ��ɫ
						}
					}
				}
				m_wndPosition.Refresh();
				//�����ֲ��б����Ѿ��ֲ�Ϊ�յĸ���ɾ����
				if (Position.Position == 0)
				{
					for (int rowcnt = 1; rowcnt < m_wndPosition.GetRowCount(); rowcnt++)
					{
						CString PositionValue = m_wndPosition.GetItemText(rowcnt, 6); //Ҫȷ����6�д�һ���ǳֲ��ֶ�
						USES_CONVERSION;
						string PositionValue_stringtype = CT2A(PositionValue.GetBuffer()); //��cstringת��Ϊstring
						if (PositionValue_stringtype == "0")
						{
							m_wndPosition.DeleteRow(rowcnt);
							break;
						}
					}
					continue;
				}
			}
		}
		return 1;
	}

	
	if (wParam == ctp_msg::RspQryOrder)
	{
		vector<Order>* Orders_list = reinterpret_cast<vector<Order>*>(lParam);
		if (Orders_list != NULL)
		{
			for (int i = 0; i < Orders_list->size(); ++i) //ѭ��ȡ���еĸ���Ԫ��
			{
				Order order = (*Orders_list)[i];
				CString Instrument_id(order.InstrumentID);//��ȡ��ͷ

				{//��������ί�в���
					if (  !(order.OrderStatus == THOST_FTDC_OST_NotTouched || order.OrderStatus == THOST_FTDC_OST_Touched)   )
					{//���������������˳�ȥ���Ѿ�������������岿�ִ���
						if (order.ContingentCondition == THOST_FTDC_CC_Immediately )
						{//Ϊ�˹��˵�������������������������Ҳ�ǳ���״̬�����Ǵ������Ͳ�������
							bool bFined = false;
							int row = m_wndOrderList.GetRowCount(); //��ȡ��ʼ��
							for (int col = 0; col < m_wndOrderList.GetColumnCount(); col++)
							{//ÿһ�ж�Ӧһ���ֶΣ���ʼ�������û���������Ϣ
								GV_ITEM Item;
								Item.mask = GVIF_TEXT | GVIF_FORMAT;
								Item.row = row;
								Item.col = col;//��ͬ�����������к�,ָ��item��������
								int field_id = m_wndOrderList.GetItemData(0, col);//�ӱ�ͷ�ĸ��л�ȡfield_id,����Ϊ��0��
								CString disp_str(GetTextById(order, field_id).c_str());//����field_id��ȡҪ���ֵ
								Item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;
								Item.strText.Format(_T("%s"), disp_str);
								if (bFined)//���ӱ��п����ҵ��Ѵ��иú�Լ����Ϣ����ֻ��ˢ��
								{
									m_wndOrderList.SetItem(&Item);
								}
								else
								{
									row = m_wndOrderList.InsertRow(Instrument_id);//����һ��,��һ�е����ݾ��Ǻ�Լ��
									bFined = true;
								}
							}
							m_wndOrderList.Refresh();
						}
					}
				}
				
				{//����δ��ί�в���
					if ( !(order.OrderStatus == THOST_FTDC_OST_AllTraded || 
						order.OrderStatus == THOST_FTDC_OST_Canceled || 
						order.OrderStatus == THOST_FTDC_OST_Unknown || 
						order.OrderStatus == THOST_FTDC_OST_Touched ||
						order.OrderStatus == THOST_FTDC_OST_NotTouched || 
						order.OrderStatus == THOST_FTDC_OST_Touched) )
					{//��������δ��ί�е�continue��ȥ
						bool bFined = false;
						int row = m_wndUnfilledOrderList.GetRowCount(); //��ȡ��ʼ��
						for (int col = 0; col < m_wndUnfilledOrderList.GetColumnCount(); col++)
						{//ÿһ�ж�Ӧһ���ֶΣ���ʼ�������û���������Ϣ
							GV_ITEM Item;
							Item.mask = GVIF_TEXT | GVIF_FORMAT;
							Item.row = row;
							Item.col = col;//��ͬ�����������к�,ָ��item��������
							int field_id = m_wndUnfilledOrderList.GetItemData(0, col);//�ӱ�ͷ�ĸ��л�ȡfield_id,����Ϊ��0��
							CString disp_str(GetTextById(order, field_id).c_str());//����field_id��ȡҪ���ֵ
							Item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;
							Item.strText.Format(_T("%s"), disp_str);
							if (bFined)//���ӱ��п����ҵ��Ѵ��иú�Լ����Ϣ����ֻ��ˢ��
							{
								m_wndUnfilledOrderList.SetItem(&Item);
							}
							else
							{
								row = m_wndUnfilledOrderList.InsertRow(Instrument_id);//����һ��,��һ�е����ݾ��Ǻ�Լ��
								bFined = true;
							}
						}
						m_wndUnfilledOrderList.Refresh();
					}
				}

				{//��������������
					if (  order.ContingentCondition == THOST_FTDC_CC_Immediately  )
					{//�������Ĵ������������������������Գ��������������͵ģ���������������Ϣ��Ӧ���մ���
						continue;
					}
					bool bFined = false;
					int row = m_wndConditionOrderList.GetRowCount(); //��ȡ��ʼ��
					for (int col = 0; col < m_wndConditionOrderList.GetColumnCount(); col++)
					{//ÿһ�ж�Ӧһ���ֶΣ���ʼ�������û���������Ϣ
						GV_ITEM Item;
						Item.mask = GVIF_TEXT | GVIF_FORMAT;
						Item.row = row;
						Item.col = col;//��ͬ�����������к�,ָ��item��������
						int field_id = m_wndConditionOrderList.GetItemData(0, col);//�ӱ�ͷ�ĸ��л�ȡfield_id,����Ϊ��0��
						CString disp_str(GetTextById(order, field_id).c_str());//����field_id��ȡҪ���ֵ
						Item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;
						Item.strText.Format(_T("%s"), disp_str);
						if (bFined)//���ӱ��п����ҵ��Ѵ��иú�Լ����Ϣ����ֻ��ˢ��
						{
							m_wndConditionOrderList.SetItem(&Item);
						}
						else
						{
							row = m_wndConditionOrderList.InsertRow(Instrument_id);//����һ��,��һ�е����ݾ��Ǻ�Լ��
							bFined = true;
						}
					}
					m_wndConditionOrderList.Refresh();
				}
			}
		}
		return 1;
	}

	if (wParam == ctp_msg::RtnOrder)
	{
		Order* porder = reinterpret_cast<Order*>(lParam);
		if (porder != NULL)
		{
			Order order = *porder;
			if (order.OrderStatus == THOST_FTDC_OST_Unknown) //����������ύ���򱨵�״̬Ϊδ֪�����ǲ���Ҫ�����ݺ�������Ϣ��Ϊ��ֱ�ӹ��˵�
			{
				return 1;
			}
			CString Instrument_id(order.InstrumentID);//��ȡ��ͷ

			{//����ί�в���
				if (!(order.OrderStatus == THOST_FTDC_OST_NotTouched || order.OrderStatus == THOST_FTDC_OST_Touched))
				{//�Ǵ������������Ѵ���������������ί�в��ִ���
					if (order.ContingentCondition == THOST_FTDC_CC_Immediately)
					{//Ϊ�˹��˵�������������������������Ҳ�ǳ���״̬�����Ǵ������Ͳ�������
						for (int j = 1; j <= m_wndOrderList.GetRowCount() - 1; ++j)
						{//row==1����ʼ��������, m_wndOrderList.GetRowCount()-1Ϊ����������, �б��0��ʼ,��0��Ϊ��ͷ��
							CString orderSysID = m_wndOrderList.GetItemText(j, m_wndOrderList.GetColumnCount() - 1);//Ҫȷ�����һ��Ϊ�������
							if (orderSysID == order.OrderSysID) //Ҫ��֤ί���б��б�����ŵ�Ψһ��
							{
								if (orderSysID != _T(""))
								{//�ڷǽ���ʱ�εı�����û�б�����ŵģ���ʱ��Ҫ����
									m_wndOrderList.DeleteRow(j);
								}
							}
						}
						bool bFined = false; //��δ�������
						int row = m_wndOrderList.GetRowCount(); //��ȡ��ʼ��ID
						for (int col = 0; col < m_wndOrderList.GetColumnCount(); col++)
						{//ÿһ�ж�Ӧһ���ֶΣ���ʼ����������Ϣ
							GV_ITEM Item;
							Item.mask = GVIF_TEXT | GVIF_FORMAT;
							Item.row = row;
							Item.col = col;//��ͬ�����������к�,ָ��item��������
							int field_id = m_wndOrderList.GetItemData(0, col);//�ӱ�ͷ�ĸ��л�ȡfield_id,����Ϊ��0��
							CString disp_str(GetTextById(order, field_id).c_str());//����field_id��ȡҪ���ֵ
							Item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;
							Item.strText.Format(_T("%s"), disp_str);
							if (bFined)//���Ѳ�����У���ֻ��ˢ��
							{
								m_wndOrderList.SetItem(&Item);
							}
							else
							{
								row = m_wndOrderList.InsertRow(Instrument_id);//����һ��,��һ�е����ݾ��Ǻ�Լ��, Ҫ���뵽��һ����
								bFined = true;
							}
						}
						m_wndOrderList.Refresh();
					}
				}
			}

			{//����δ��ί�в���
				if ( !(order.OrderStatus == THOST_FTDC_OST_NotTouched || order.OrderStatus == THOST_FTDC_OST_Touched) )
				{//�Ǵ������������Ѵ���������������δ��ί�в��ִ���
					if (order.ContingentCondition == THOST_FTDC_CC_Immediately)
					{//Ϊ�˹��˵�������������������������Ҳ�ǳ���״̬�����Ǵ������Ͳ�������
						if (order.OrderStatus == THOST_FTDC_OST_AllTraded || order.OrderStatus == THOST_FTDC_OST_Canceled || order.OrderStatus == THOST_FTDC_OST_Touched)
						{//Ҫ����Ƿ�������ί�е���ȫ���ɽ�������,  ����,  ��Ҫ��δ��ί���б���ɾ����Ӧί����
							for (int j = 1; j <= m_wndUnfilledOrderList.GetRowCount() - 1; ++j)
							{
								CString orderSysID = m_wndUnfilledOrderList.GetItemText(j, m_wndUnfilledOrderList.GetColumnCount() - 1);//Ҫȷ�����һ��Ϊ�������
								if (orderSysID == order.OrderSysID) //Ҫ��֤ί���б��б�����ŵ�Ψһ��
								{
									m_wndUnfilledOrderList.DeleteRow(j);
									m_wndUnfilledOrderList.Refresh();
									return 1;//ɾ����к�,ֱ��return��ȥ
								}
							}
							return 1; //��ʹ��δ��ί�����Ҳ�����Ӧδ��ί��,ҲӦ��return��ȥ,��ֹ���
						}
						for (int j = 1; j <= m_wndUnfilledOrderList.GetRowCount() - 1; ++j)
						{//row==1����ʼ��������, m_wndUnfilledOrderList.GetRowCount()-1Ϊ����������, �б��0��ʼ,��0��Ϊ��ͷ��
							CString orderSysID = m_wndUnfilledOrderList.GetItemText(j, m_wndUnfilledOrderList.GetColumnCount() - 1);//Ҫȷ�����һ��Ϊ�������
							if (orderSysID == order.OrderSysID) //Ҫ��֤ί���б��б�����ŵ�Ψһ��
							{
								if (orderSysID != _T(""))
								{//�ڷǽ���ʱ�εı�����û�б�����ŵģ���ʱ��Ҫ����
									m_wndUnfilledOrderList.DeleteRow(j);
								}
							}
						}
						bool bFined = false; //��δ�������
						int row = m_wndUnfilledOrderList.GetRowCount(); //��ȡ��ʼ��ID
						for (int col = 0; col < m_wndUnfilledOrderList.GetColumnCount(); col++)
						{//ÿһ�ж�Ӧһ���ֶΣ���ʼ����������Ϣ
							GV_ITEM Item;
							Item.mask = GVIF_TEXT | GVIF_FORMAT;
							Item.row = row;
							Item.col = col;//��ͬ�����������к�,ָ��item��������
							int field_id = m_wndUnfilledOrderList.GetItemData(0, col);//�ӱ�ͷ�ĸ��л�ȡfield_id,����Ϊ��0��
							CString disp_str(GetTextById(order, field_id).c_str());//����field_id��ȡҪ���ֵ
							Item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;
							Item.strText.Format(_T("%s"), disp_str);
							if (bFined)//���Ѳ�����У���ֻ��ˢ��
							{
								m_wndUnfilledOrderList.SetItem(&Item);
							}
							else
							{
								row = m_wndUnfilledOrderList.InsertRow(Instrument_id);//����һ��,��һ�е����ݾ��Ǻ�Լ��
								bFined = true;
							}
						}
						m_wndUnfilledOrderList.Refresh();
					}
				}
			}

			{//��������������
				if (  !(order.ContingentCondition == THOST_FTDC_CC_Immediately)  )
				{//��δ�������Ѵ�����������˴�����
					for (int j = 1; j <= m_wndConditionOrderList.GetRowCount() - 1; ++j)
					{//row==1����ʼ��������, m_wndOrderList.GetRowCount()-1Ϊ����������, �б��0��ʼ,��0��Ϊ��ͷ��
						CString orderSysID = m_wndConditionOrderList.GetItemText(j, m_wndConditionOrderList.GetColumnCount() - 1);//Ҫȷ�����һ��Ϊ�������
						if (orderSysID == order.OrderSysID) //Ҫ��֤ί���б��б�����ŵ�Ψһ��
						{
							if (orderSysID != _T(""))
							{//�ڷǽ���ʱ�εı�����û�б�����ŵģ���ʱ��Ҫ����
								m_wndConditionOrderList.DeleteRow(j);
							}
						}
					}
					bool bFined = false; //��δ�������
					int row = m_wndConditionOrderList.GetRowCount(); //��ȡ��ʼ��ID
					for (int col = 0; col < m_wndConditionOrderList.GetColumnCount(); col++)
					{//ÿһ�ж�Ӧһ���ֶΣ���ʼ����������Ϣ
						GV_ITEM Item;
						Item.mask = GVIF_TEXT | GVIF_FORMAT;
						Item.row = row;
						Item.col = col;//��ͬ�����������к�,ָ��item��������
						int field_id = m_wndConditionOrderList.GetItemData(0, col);//�ӱ�ͷ�ĸ��л�ȡfield_id,����Ϊ��0��
						CString disp_str(GetTextById(order, field_id).c_str());//����field_id��ȡҪ���ֵ
						Item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;
						Item.strText.Format(_T("%s"), disp_str);
						if (bFined)//���Ѳ�����У���ֻ��ˢ��
						{
							m_wndConditionOrderList.SetItem(&Item);
						}
						else
						{
							row = m_wndConditionOrderList.InsertRow(Instrument_id);//����һ��,��һ�е����ݾ��Ǻ�Լ��, Ҫ���뵽��һ����
							bFined = true;
						}
					}
					m_wndConditionOrderList.Refresh();
				}
			}

		}
		return 1;
	}

	if (wParam==ctp_msg::RspQryTrade)
	{
		vector<Trade>* Trades_list = reinterpret_cast<vector<Trade>*>(lParam);
		if (Trades_list != NULL)
		{
			for (int i = 0; i < Trades_list->size(); ++i) //ѭ��ȡ���еĸ���Ԫ��
			{
				Trade trade = (*Trades_list)[i];
				CString Instrument_id(trade.InstrumentID);//��ȡ��ͷ
				{//��������ί�в���
					bool bFined = false;
					int row = m_wndTrade.GetRowCount(); //��ȡ��ʼ��
					for (int col = 0; col < m_wndTrade.GetColumnCount(); col++)
					{//ÿһ�ж�Ӧһ���ֶΣ���ʼ�������û���������Ϣ
						GV_ITEM Item;
						Item.mask = GVIF_TEXT | GVIF_FORMAT;
						Item.row = row;
						Item.col = col;//��ͬ�����������к�,ָ��item��������
						int field_id = m_wndTrade.GetItemData(0, col);//�ӱ�ͷ�ĸ��л�ȡfield_id,����Ϊ��0��
						CString disp_str(GetTextById(trade, field_id).c_str());//����field_id��ȡҪ���ֵ
						Item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;
						Item.strText.Format(_T("%s"), disp_str);
						if (bFined)//���ӱ��п����ҵ��Ѵ��иú�Լ����Ϣ����ֻ��ˢ��
						{
							m_wndTrade.SetItem(&Item);
						}
						else
						{
							row = m_wndTrade.InsertRow(Instrument_id);//����һ��,��һ�е����ݾ��Ǻ�Լ��
							bFined = true;
						}
					}
					m_wndTrade.Refresh();
				}
			}
		}
		return 1;
	}

	if (wParam == ctp_msg::RtnTrade)
	{
		Trade* ptrade = reinterpret_cast<Trade*>(lParam);
		if (ptrade != NULL)
		{
			Trade trade = *ptrade;
			CString Instrument_id(trade.InstrumentID);
			{
				bool bFined = false; 
				int row = m_wndTrade.GetRowCount(); //��ȡ��ʼ��index
				for (int col = 0; col < m_wndTrade.GetColumnCount(); col++)
				{
					GV_ITEM Item;
					Item.mask = GVIF_TEXT | GVIF_FORMAT;
					Item.row = row;
					Item.col = col;
					int field_id = m_wndTrade.GetItemData(0, col);
					CString disp_str(GetTextById(trade, field_id).c_str());
					Item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;
					Item.strText.Format(_T("%s"), disp_str);
					if (bFined)
					{
						m_wndTrade.SetItem(&Item);
					}
					else
					{
						row = m_wndTrade.InsertRow(Instrument_id);
						bFined = true;
					}
				}
				m_wndTrade.Refresh();
			}
		}
		return 1;
	}

	if (wParam == ctp_msg::RspQryInvestorPositionDetail)
	{
		{//���³ֲ���ϸ�б�
			vector<InvestorPositionDetail>* InvestorPositionDetails_list = reinterpret_cast<vector<InvestorPositionDetail>*>(lParam);
			if (InvestorPositionDetails_list != NULL)
			{
				int row = 0;
				for (int i = 0; i < InvestorPositionDetails_list->size(); ++i)
				{
					InvestorPositionDetail PositionDetail = (*InvestorPositionDetails_list)[i];
					CString Instrument_id(PositionDetail.InstrumentID);//��ȡ��ͷ
					CString trade_id(PositionDetail.TradeID);//��ȡ�������ݵĳɽ����

					bool bFined = false;//һ��ʼ���費����
					row = m_wndPositionDetail.GetRowCount(); //һ��ʼ���費����,�趨������Index
					for (int row_index = 1; row_index < m_wndPositionDetail.GetRowCount(); row_index++)
					{
						if (trade_id == m_wndPositionDetail.GetItemText(row_index, m_wndPositionDetail.GetColumnCount() - 1)) //Ҫ��֤���һ��Ϊ�ɽ����
						{
							bFined = true;
							row = row_index; //��ȡrow index,��0��ʼ
							break;
						}
					}
					for (int col = 0; col < m_wndPositionDetail.GetColumnCount(); col++)
					{//ÿһ�ж�Ӧһ���ֶΣ���ʼ�������û���������Ϣ
						GV_ITEM Item;
						Item.mask = GVIF_TEXT | GVIF_FORMAT;
						Item.row = row;//ͨ����һ��ѭ�������ҵ���row�Ѿ�����,����δ�ҵ������ĩβ�е���һ��,ָ��item��������
						Item.col = col;//��ͬ�����������к�,ָ��item��������
						int field_id = m_wndPositionDetail.GetItemData(0, col);//�ӱ�ͷ�ĸ��л�ȡfield_id,����Ϊ��0��
						CString disp_str(GetTextById(PositionDetail, field_id).c_str());//����field_id��ȡҪ���ֵ
						Item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;
						Item.strText.Format(_T("%s"), disp_str);
						if (bFined)//���ӱ��п����ҵ��Ѵ��иú�Լ����Ϣ����ֻ��ˢ��
						{
							m_wndPositionDetail.SetItem(&Item);
						}
						else
						{
							row = m_wndPositionDetail.InsertRow(Instrument_id);//����һ��,��һ�е����ݾ��Ǻ�Լ��
							bFined = true;
						}
						if (FIELD_ID_InvestorPositionDetail::FIELD_PositionProfitByTrade_PositionDetail == field_id)//����ʱ��ˢ����ͷ��Լ������
						{
							string floatprofit_str = CT2A(m_wndPositionDetail.GetItemText(row, col).GetBuffer());
							double floatprofit = stod(floatprofit_str);
							if (floatprofit <= 0)
							{
								m_wndPositionDetail.SetItemBkColour(row, col, RGB(0, 255, 0)); //���ñ���ɫ
								m_wndPositionDetail.SetItemFgColour(row, col, RGB(0, 0, 0)); //����ǰ��ɫ
							}
							if (floatprofit > 0)
							{
								m_wndPositionDetail.SetItemBkColour(row, col, RGB(255, 0, 0)); //���ñ���ɫ
								m_wndPositionDetail.SetItemFgColour(row, col, RGB(0, 0, 0)); //����ǰ��ɫ
							}
						}
					}
					m_wndPositionDetail.Refresh();
					//m_wndPositionDetail.ExpandColumnsToFit(TRUE);
					//�����ֲ��б����Ѿ��ֲ�Ϊ�յĸ���ɾ����
					if (PositionDetail.Volume == 0)
					{
						for (int rowcnt = 1; rowcnt < m_wndPositionDetail.GetRowCount(); rowcnt++)
						{
							CString PositionValue = m_wndPositionDetail.GetItemText(rowcnt, 3); //Ҫȷ����3�д�һ���ǳֲ����ֶ�
							USES_CONVERSION;
							string PositionValue_stringtype = CT2A(PositionValue.GetBuffer()); //��cstringת��Ϊstring
							if (PositionValue_stringtype == "0")
							{
								m_wndPositionDetail.DeleteRow(rowcnt);
								break;
							}
						}
						continue;
					}
				}
			}
		}

		{//�����ֲ���ϸ������ƽ���б�
			vector<InvestorPositionDetail> InvestorPositionDetail_vector;
			get_global_mgr().GetService()->get_all_InvestorPositionDetails(InvestorPositionDetail_vector);
			if (InvestorPositionDetail_vector.size() != 0)
			{
				int row = 0;
				for (int i = 0; i < InvestorPositionDetail_vector.size(); ++i)
				{
					InvestorPositionDetail PositionDetail = InvestorPositionDetail_vector[i];
					if (PositionDetail.CloseVolume == 0)
					{//��ƽ�����ݹ��˵�
						continue;
					}
					CString Instrument_id(PositionDetail.InstrumentID);//��ȡ��ͷ
					CString trade_id(PositionDetail.TradeID);//��ȡ�������ݵĳɽ����
					bool bFined = false;//һ��ʼ���費����
					row = m_wndCloseTrade.GetRowCount(); //һ��ʼ���費����,�趨������Index
					for (int row_index = 1; row_index < m_wndCloseTrade.GetRowCount(); row_index++)
					{
						if (trade_id == m_wndCloseTrade.GetItemText(row_index, m_wndCloseTrade.GetColumnCount() - 1)) //Ҫ��֤���һ��Ϊ�ɽ����
						{
							bFined = true;
							row = row_index; //��ȡrow index,
							break;
						}
					}
					int col = 0;
					for (col = 0; col < m_wndCloseTrade.GetColumnCount(); col++)
					{
						GV_ITEM Item;
						Item.mask = GVIF_TEXT | GVIF_FORMAT;
						Item.row = row;//ͨ����һ��ѭ�������ҵ���row�Ѿ�����,����δ�ҵ������ĩβ�е���һ��,ָ��item��������
						Item.col = col;//��ͬ�����������к�,ָ��item��������
						CString disp_str;
						if (m_wndCloseTrade.GetItemText(0, col) == _T("��Լ"))
						{
							disp_str = PositionDetail.InstrumentID;
						}
						else if (m_wndCloseTrade.GetItemText(0, col) == _T("����"))
						{
							if (PositionDetail.Direction == THOST_FTDC_D_Buy)
							{
								disp_str = _T("��");
							}
							if (PositionDetail.Direction == THOST_FTDC_D_Sell)
							{
								disp_str = _T("��");
							}
						}
						else if (m_wndCloseTrade.GetItemText(0, col) == _T("ƽ����"))
						{
							disp_str = to_string(PositionDetail.CloseVolume).c_str();
						}
						else if (m_wndCloseTrade.GetItemText(0, col) == _T("���ּ�"))
						{
							char OpenPrice_cstr[100];
							sprintf(OpenPrice_cstr, "%0.2f", PositionDetail.OpenPrice); //��ȡΪ��λ���Ⱥ�ת��Ϊ�ַ�������
							disp_str = OpenPrice_cstr;
						}
						else if (m_wndCloseTrade.GetItemText(0, col) == _T("ƽ�ּ�"))
						{
							int Instrument_Multiple = (PositionDetail.CloseAmount - (PositionDetail.CloseProfitByTrade / PositionDetail.CloseVolume)) / PositionDetail.OpenPrice;
							double ClosePrice = PositionDetail.CloseAmount / Instrument_Multiple;
							char ClosePrice_cstr[100];
							sprintf(ClosePrice_cstr, "%0.2f", ClosePrice); //��ȡΪ��λ���Ⱥ�ת��Ϊ�ַ�������
							disp_str = ClosePrice_cstr;
						}
						else if (m_wndCloseTrade.GetItemText(0, col) == _T("ƽ��ӯ��"))
						{
							char CloseProfitByTrade_cstr[100];
							sprintf(CloseProfitByTrade_cstr, "%0.2f", PositionDetail.CloseProfitByTrade); //��ȡΪ��λ���Ⱥ�ת��Ϊ�ַ�������
							disp_str = CloseProfitByTrade_cstr;
						}
						else if (m_wndCloseTrade.GetItemText(0, col) == _T("��������"))
						{
							disp_str = PositionDetail.OpenDate;
						}
						else if (m_wndCloseTrade.GetItemText(0, col) == _T("�ɽ����"))
						{
							disp_str = PositionDetail.TradeID;
						}
						else
						{
							disp_str = _T("---");
						}
						Item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;
						Item.strText.Format(_T("%s"), disp_str);
						if (bFined)//���ӱ��п����ҵ��Ѵ��иú�Լ����Ϣ����ֻ��ˢ��
						{
							m_wndCloseTrade.SetItem(&Item);
						}
						else
						{
							row = m_wndCloseTrade.InsertRow(Instrument_id);//����һ��,��һ�е����ݾ��Ǻ�Լ��
							bFined = true;
						}
						if (m_wndCloseTrade.GetItemText(0, col) == _T("ƽ��ӯ��"))
						{
							string closeprofit_str = CT2A(m_wndCloseTrade.GetItemText(row, col).GetBuffer());
							double closeprofit = stod(closeprofit_str);
							if (closeprofit <= 0)
							{
								m_wndCloseTrade.SetItemBkColour(row, col, RGB(0, 255, 0)); //���ñ���ɫ
								m_wndCloseTrade.SetItemFgColour(row, col, RGB(0, 0, 0)); //����ǰ��ɫ
							}
							if (closeprofit > 0)
							{
								m_wndCloseTrade.SetItemBkColour(row, col, RGB(255, 0, 0)); //���ñ���ɫ
								m_wndCloseTrade.SetItemFgColour(row, col, RGB(0, 0, 0)); //����ǰ��ɫ
							}
						}
					}
					m_wndCloseTrade.Refresh();
				}
			}
		}
		return 1;
	}

	if (wParam == ctp_msg::RspQryParkedOrder)
	{
		{//����Ԥ���б�
			vector<ParkedOrder>* parkedOrder_list = reinterpret_cast<vector<ParkedOrder>*>(lParam);
			if (parkedOrder_list != NULL)
			{
				int row = 0;
				for (int i = 0; i < parkedOrder_list->size(); ++i)
				{
					ParkedOrder parkedOrder = (*parkedOrder_list)[i];
					CString Instrument_id(parkedOrder.InstrumentID);//��ȡ��ͷ
					CString parkedOrder_id(parkedOrder.ParkedOrderID);//��ȡ�������ݵı������
					int  Error_id(parkedOrder.ErrorID);//��ȡ�������ݵĴ���ID
					/*
					if (Error_id != 0)
					{//�д����Ԥ�񱨵����Ǵ��ɽ���Ԥ�񱨵�,���˵�, ��ʱ������
					continue;
					}
					*/
					bool bFined = false;//һ��ʼ���費����
					row = m_wndParkedOrderList.GetRowCount(); //һ��ʼ���費����,�趨������Index
					for (int row_index = 1; row_index < m_wndParkedOrderList.GetRowCount(); row_index++)
					{
						if (parkedOrder_id == m_wndParkedOrderList.GetItemText(row_index, m_wndParkedOrderList.GetColumnCount() - 1)) //Ҫ��֤���һ��ΪԤ�񱨵����
						{
							bFined = true;
							row = row_index; //��ȡrow index
							break;
						}
					}
					int col = 0;
					for (col = 0; col < m_wndParkedOrderList.GetColumnCount(); col++)
					{//ÿһ�ж�Ӧһ���ֶ�
						GV_ITEM Item;
						Item.mask = GVIF_TEXT | GVIF_FORMAT;
						Item.row = row;//ͨ����һ��ѭ�������ҵ���row�Ѿ�����,����δ�ҵ������ĩβ�е���һ��,ָ��item��������
						Item.col = col;//��ͬ�����������к�,ָ��item��������
						CString disp_str;
						if (m_wndParkedOrderList.GetItemText(0, col) == _T("��Լ"))
						{
							disp_str = parkedOrder.InstrumentID;
						}
						else if (m_wndParkedOrderList.GetItemText(0, col) == _T("����"))
						{
							if (parkedOrder.Direction == THOST_FTDC_D_Buy)
							{
								disp_str = _T("��");
							}
							if (parkedOrder.Direction == THOST_FTDC_D_Sell)
							{
								disp_str = _T("��");
							}
						}
						else if (m_wndParkedOrderList.GetItemText(0, col) == _T("��������"))
						{
							disp_str = to_string(parkedOrder.VolumeTotalOriginal).c_str();
						}
						else if (m_wndParkedOrderList.GetItemText(0, col) == _T("�����۸�"))
						{
							char OpenPrice_cstr[100];
							sprintf(OpenPrice_cstr, "%0.2f", parkedOrder.LimitPrice); //��ȡΪ��λ���Ⱥ�ת��Ϊ�ַ�������
							disp_str = OpenPrice_cstr;
						}
						else if (m_wndParkedOrderList.GetItemText(0, col) == _T("������"))
						{
							char	TriggerPrice_cstr[100];
							sprintf(TriggerPrice_cstr, "%0.2f", parkedOrder.StopPrice); //��ȡΪ��λ���Ⱥ�ת��Ϊ�ַ�������
							disp_str = TriggerPrice_cstr;
							if (disp_str == _T("0.00") || parkedOrder.StopPrice<0)
							{//parkedOrder.StopPrice<0˵��������,����ĳЩ�ټ����쳣���
								disp_str = _T("��");
							}
						}
						else if (m_wndParkedOrderList.GetItemText(0, col) == _T("��������"))
						{
							if (parkedOrder.ContingentCondition == THOST_FTDC_CC_Immediately)
							{
								disp_str = _T("����");
							}
							if (parkedOrder.ContingentCondition == THOST_FTDC_CC_Touch)
							{
								disp_str = _T("ֹ��");
							}
							if (parkedOrder.ContingentCondition == THOST_FTDC_CC_TouchProfit)
							{
								disp_str = _T("ֹӯ");
							}
							if (parkedOrder.ContingentCondition == THOST_FTDC_CC_ParkedOrder)
							{
								disp_str = _T("Ԥ��");
							}
							if (parkedOrder.ContingentCondition == THOST_FTDC_CC_LastPriceGreaterEqualStopPrice)
							{
								disp_str = _T("���¼۴��ڵ���������");
							}
							if (parkedOrder.ContingentCondition == THOST_FTDC_CC_LastPriceGreaterThanStopPrice)
							{
								disp_str = _T("���¼۴���������");
							}
							if (parkedOrder.ContingentCondition == THOST_FTDC_CC_LastPriceLesserEqualStopPrice)
							{
								disp_str = _T("���¼�С�ڵ���������");
							}
							if (parkedOrder.ContingentCondition == THOST_FTDC_CC_LastPriceLesserThanStopPrice)
							{
								disp_str = _T("���¼�С��������");
							}
						}
						else if (m_wndParkedOrderList.GetItemText(0, col) == _T("״̬"))
						{
							if (parkedOrder.Status == THOST_FTDC_PAOS_NotSend)
							{
								disp_str = _T("δ����");
							}
							if (parkedOrder.Status == THOST_FTDC_PAOS_Send)
							{
								disp_str = _T("�ѷ���");
							}
							if (parkedOrder.Status == THOST_FTDC_PAOS_Deleted)
							{
								disp_str = _T("��ɾ��");
							}
						}
						else if (m_wndParkedOrderList.GetItemText(0, col) == _T("Ԥ�񵥱��"))
						{
							disp_str = parkedOrder.ParkedOrderID;
						}
						else if (m_wndParkedOrderList.GetItemText(0, col) == _T("������"))
						{
							disp_str = parkedOrder.ExchangeID;
						}
						else if (m_wndParkedOrderList.GetItemText(0, col) == _T("��ƽ"))
						{
							if (parkedOrder.CombOffsetFlag[0] == THOST_FTDC_OF_Open)
							{
								disp_str = _T("����");
							}
							else if (parkedOrder.CombOffsetFlag[0] == THOST_FTDC_OF_Close)
							{
								disp_str = _T("ƽ��");
							}
							else if (parkedOrder.CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday)
							{
								disp_str = _T("ƽ��");
							}
							else if (parkedOrder.CombOffsetFlag[0] == THOST_FTDC_OF_ForceClose)
							{
								disp_str = _T("ǿƽ");
							}
							else if (parkedOrder.CombOffsetFlag[0] == THOST_FTDC_OF_CloseYesterday)
							{
								disp_str = _T("ƽ��");
							}
							else if (parkedOrder.CombOffsetFlag[0] == THOST_FTDC_OF_ForceOff)
							{
								disp_str = _T("ǿ��");
							}
							else if (parkedOrder.CombOffsetFlag[0] == THOST_FTDC_OF_LocalForceClose)
							{
								disp_str = _T("����ǿƽ");
							}
							else
							{
								disp_str = _T("---");
							}
						}
						else if (m_wndParkedOrderList.GetItemText(0, col) == _T("��������"))
						{
							if (parkedOrder.OrderPriceType == THOST_FTDC_OPT_AnyPrice)
							{
								disp_str = _T("�м�");
							}
							else if (parkedOrder.OrderPriceType == THOST_FTDC_OPT_LimitPrice)
							{
								disp_str = _T("�޼�");
							}
							else
							{
								disp_str = _T("����");
							}
						}
						else if (m_wndParkedOrderList.GetItemText(0, col) == _T("������Ϣ"))
						{
							disp_str = parkedOrder.ErrorMsg;
							if (disp_str==_T(""))
							{
								disp_str = _T("��");
							}
						}
						else
						{
							disp_str = _T("---");
						}
						Item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;
						Item.strText.Format(_T("%s"), disp_str);
						if (bFined)//���ӱ��п����ҵ��Ѵ��иú�Լ����Ϣ����ֻ��ˢ��
						{
							m_wndParkedOrderList.SetItem(&Item);
						}
						else
						{
							row = m_wndParkedOrderList.InsertRow(Instrument_id);//����һ��,��һ�е����ݾ��Ǻ�Լ��
							bFined = true;
						}
					}
					m_wndParkedOrderList.Refresh();
				}
			}
		}
		return 1;
	}

	if (wParam == ctp_msg::RspParkedOrderInsert)
	{
		{//�����ڷǽ���ʱ��,��Ԥ�񵥲��ܳɹ�,����Ԥ�񵥳ɹ���,ˢ��Ԥ���¼�б�
			ParkedOrder* pParkedOrder = reinterpret_cast<ParkedOrder*>(lParam);
			ParkedOrder parkedOrder = *pParkedOrder;
			if (pParkedOrder != NULL)
			{
				int row = 0;
				CString Instrument_id(parkedOrder.InstrumentID);//��ȡ��ͷ
				CString parkedOrder_id(parkedOrder.ParkedOrderID);//��ȡ�������ݵı������
					bool bFined = false;//һ��ʼ���費����
					row = m_wndParkedOrderList.GetRowCount(); //һ��ʼ���費����,�趨������Index
					for (int row_index = 1; row_index < m_wndParkedOrderList.GetRowCount(); row_index++)
					{
						if (parkedOrder_id == m_wndParkedOrderList.GetItemText(row_index, m_wndParkedOrderList.GetColumnCount() - 1)) //Ҫ��֤���һ��ΪԤ�񱨵����
						{
							bFined = true;
							row = row_index; //��ȡrow index
							break;
						}
					}
					int col = 0;
					for (col = 0; col < m_wndParkedOrderList.GetColumnCount(); col++)
					{//ÿһ�ж�Ӧһ���ֶ�
						GV_ITEM Item;
						Item.mask = GVIF_TEXT | GVIF_FORMAT;
						Item.row = row;//ͨ����һ��ѭ�������ҵ���row�Ѿ�����,����δ�ҵ������ĩβ�е���һ��,ָ��item��������
						Item.col = col;//��ͬ�����������к�,ָ��item��������
						CString disp_str;
						if (m_wndParkedOrderList.GetItemText(0, col) == _T("��Լ"))
						{
							disp_str = parkedOrder.InstrumentID;
						}
						else if (m_wndParkedOrderList.GetItemText(0, col) == _T("����"))
						{
							if (parkedOrder.Direction == THOST_FTDC_D_Buy)
							{
								disp_str = _T("��");
							}
							if (parkedOrder.Direction == THOST_FTDC_D_Sell)
							{
								disp_str = _T("��");
							}
						}
						else if (m_wndParkedOrderList.GetItemText(0, col) == _T("��������"))
						{
							disp_str = to_string(parkedOrder.VolumeTotalOriginal).c_str();
						}
						else if (m_wndParkedOrderList.GetItemText(0, col) == _T("�����۸�"))
						{
							char OpenPrice_cstr[100];
							sprintf(OpenPrice_cstr, "%0.2f", parkedOrder.LimitPrice); //��ȡΪ��λ���Ⱥ�ת��Ϊ�ַ�������
							disp_str = OpenPrice_cstr;
						}
						else if (m_wndParkedOrderList.GetItemText(0, col) == _T("������"))
						{
							char	TriggerPrice_cstr[100];
							sprintf(TriggerPrice_cstr, "%0.2f", parkedOrder.StopPrice); //��ȡΪ��λ���Ⱥ�ת��Ϊ�ַ�������
							disp_str = TriggerPrice_cstr;
							if (disp_str == _T("0.00") || parkedOrder.StopPrice<0)
							{//parkedOrder.StopPrice<0˵��������,����ĳЩ�ټ����쳣���
								disp_str = _T("��");
							}
						}
						else if (m_wndParkedOrderList.GetItemText(0, col) == _T("��������"))
						{
							if (parkedOrder.ContingentCondition == THOST_FTDC_CC_Immediately)
							{
								disp_str = _T("����");
							}
							if (parkedOrder.ContingentCondition == THOST_FTDC_CC_Touch)
							{
								disp_str = _T("ֹ��");
							}
							if (parkedOrder.ContingentCondition == THOST_FTDC_CC_TouchProfit)
							{
								disp_str = _T("ֹӯ");
							}
							if (parkedOrder.ContingentCondition == THOST_FTDC_CC_ParkedOrder)
							{
								disp_str = _T("Ԥ��");
							}
							if (parkedOrder.ContingentCondition == THOST_FTDC_CC_LastPriceGreaterEqualStopPrice)
							{
								disp_str = _T("���¼۴��ڵ���������");
							}
							if (parkedOrder.ContingentCondition == THOST_FTDC_CC_LastPriceGreaterThanStopPrice)
							{
								disp_str = _T("���¼۴���������");
							}
							if (parkedOrder.ContingentCondition == THOST_FTDC_CC_LastPriceLesserEqualStopPrice)
							{
								disp_str = _T("���¼�С�ڵ���������");
							}
							if (parkedOrder.ContingentCondition == THOST_FTDC_CC_LastPriceLesserThanStopPrice)
							{
								disp_str = _T("���¼�С��������");
							}
						}
						else if (m_wndParkedOrderList.GetItemText(0, col) == _T("״̬"))
						{
							if (parkedOrder.Status == THOST_FTDC_PAOS_NotSend)
							{
								disp_str = _T("δ����");
							}
							if (parkedOrder.Status == THOST_FTDC_PAOS_Send)
							{
								disp_str = _T("�ѷ���");
							}
							if (parkedOrder.Status == THOST_FTDC_PAOS_Deleted)
							{
								disp_str = _T("��ɾ��");
							}
						}
						else if (m_wndParkedOrderList.GetItemText(0, col) == _T("Ԥ�񵥱��"))
						{
							disp_str = parkedOrder.ParkedOrderID;
						}
						else if (m_wndParkedOrderList.GetItemText(0, col) == _T("������"))
						{
							disp_str = parkedOrder.ExchangeID;
						}
						else if (m_wndParkedOrderList.GetItemText(0, col) == _T("��ƽ"))
						{
							if (parkedOrder.CombOffsetFlag[0] == THOST_FTDC_OF_Open)
							{
								disp_str = _T("����");
							}
							else if (parkedOrder.CombOffsetFlag[0] == THOST_FTDC_OF_Close)
							{
								disp_str = _T("ƽ��");
							}
							else if (parkedOrder.CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday)
							{
								disp_str = _T("ƽ��");
							}
							else if (parkedOrder.CombOffsetFlag[0] == THOST_FTDC_OF_ForceClose)
							{
								disp_str = _T("ǿƽ");
							}
							else if (parkedOrder.CombOffsetFlag[0] == THOST_FTDC_OF_CloseYesterday)
							{
								disp_str = _T("ƽ��");
							}
							else if (parkedOrder.CombOffsetFlag[0] == THOST_FTDC_OF_ForceOff)
							{
								disp_str = _T("ǿ��");
							}
							else if (parkedOrder.CombOffsetFlag[0] == THOST_FTDC_OF_LocalForceClose)
							{
								disp_str = _T("����ǿƽ");
							}
							else
							{
								disp_str = _T("---");
							}
						}
						else if (m_wndParkedOrderList.GetItemText(0, col) == _T("��������"))
						{
							if (parkedOrder.OrderPriceType == THOST_FTDC_OPT_AnyPrice)
							{
								disp_str = _T("�м�");
							}
							else if (parkedOrder.OrderPriceType == THOST_FTDC_OPT_LimitPrice)
							{
								disp_str = _T("�޼�");
							}
							else
							{
								disp_str = _T("����");
							}
						}
						else if (m_wndParkedOrderList.GetItemText(0, col) == _T("������Ϣ"))
						{
							disp_str = parkedOrder.ErrorMsg;
							if (disp_str == _T(""))
							{
								disp_str = _T("��");
							}
						}
						else
						{
							disp_str = _T("---");
						}
						Item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;
						Item.strText.Format(_T("%s"), disp_str);
						if (bFined)//���ӱ��п����ҵ��Ѵ��иú�Լ����Ϣ����ֻ��ˢ��
						{
							m_wndParkedOrderList.SetItem(&Item);
						}
						else
						{
							row = m_wndParkedOrderList.InsertRow(Instrument_id);//����һ��,��һ�е����ݾ��Ǻ�Լ��
							bFined = true;
						}
					}
					m_wndParkedOrderList.Refresh();
				}
		}
		return 1;
	}

	if (wParam == ctp_msg::RspRemoveParkedOrder)
	{//ɾ����ֻ����״̬
		Remove_ParkedOrder* pParkedOrder = reinterpret_cast<Remove_ParkedOrder*>(lParam);
		CString parkedOrderID(pParkedOrder->ParkedOrderID);
		int row = 0;
		for (int row_index = 1; row_index < m_wndParkedOrderList.GetRowCount(); row_index++)
		{
			if (parkedOrderID == m_wndParkedOrderList.GetItemText(row_index, m_wndParkedOrderList.GetColumnCount() - 1)) //Ҫ��֤���һ��ΪԤ�񱨵����
			{
				row = row_index; //��ȡrow index
				break;
			}
		}
		CString disp_str;
		int col = 0;
		for (col = 0; col < m_wndParkedOrderList.GetColumnCount(); col++)
		{//ÿһ�ж�Ӧһ���ֶ�
			GV_ITEM Item;
			Item.mask = GVIF_TEXT | GVIF_FORMAT;
			Item.row = row;//ͨ����һ��ѭ�������ҵ���row�Ѿ�����,����δ�ҵ������ĩβ�е���һ��,ָ��item��������
			Item.col = col;//��ͬ�����������к�,ָ��item��������
			if (m_wndParkedOrderList.GetItemText(0, col) == _T("״̬"))
			{
				disp_str = _T("��ɾ��");
				Item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;
				Item.strText.Format(_T("%s"), disp_str);
				m_wndParkedOrderList.SetItem(&Item);
				break;
			}
		}
		m_wndParkedOrderList.Refresh();
	}


	if (wParam == ctp_msg::RspQryParkedOrderAction)
	{
		{//����Ԥ�񳷵��б�
			vector<ParkedOrderAction>* parkedOrderAction_list = reinterpret_cast<vector<ParkedOrderAction>*>(lParam);
			if (parkedOrderAction_list != NULL)
			{
				int row = 0;
				for (int i = 0; i < parkedOrderAction_list->size(); ++i)
				{
					ParkedOrderAction parkedOrderAction = (*parkedOrderAction_list)[i];
					CString Instrument_id(parkedOrderAction.InstrumentID);//��ȡ��ͷ
					CString parkedOrderAction_id(parkedOrderAction.ParkedOrderActionID);//��ȡ�������ݵı������
					int  Error_id(parkedOrderAction.ErrorID);//��ȡ�������ݵĴ���ID
					/*
					if (Error_id != 0)
					{//�д����Ԥ�񳷵��������Ǵ��ɽ���Ԥ�񳷵�����,���˵�, ��ʱ������
					continue;
					}
					*/
					bool bFined = false;//һ��ʼ���費����
					row = m_wndParkedOrderActionList.GetRowCount(); //һ��ʼ���費����,�趨������Index
					for (int row_index = 1; row_index < m_wndParkedOrderActionList.GetRowCount(); row_index++)
					{
						if (parkedOrderAction_id == m_wndParkedOrderActionList.GetItemText(row_index, m_wndParkedOrderActionList.GetColumnCount() - 1)) //Ҫ��֤���һ��ΪԤ�񳷵��������
						{
							bFined = true;
							row = row_index; //��ȡrow index
							break;
						}
					}
					int col = 0;
					for (col = 0; col < m_wndParkedOrderActionList.GetColumnCount(); col++)
					{//ÿһ�ж�Ӧһ���ֶ�
						GV_ITEM Item;
						Item.mask = GVIF_TEXT | GVIF_FORMAT;
						Item.row = row;//ͨ����һ��ѭ�������ҵ���row�Ѿ�����,����δ�ҵ������ĩβ�е���һ��,ָ��item��������
						Item.col = col;//��ͬ�����������к�,ָ��item��������
						CString disp_str;
						if (m_wndParkedOrderActionList.GetItemText(0, col) == _T("��Լ"))
						{
							disp_str = parkedOrderAction.InstrumentID;
						}
						else if (m_wndParkedOrderActionList.GetItemText(0, col) == _T("������"))
						{
							disp_str = to_string(parkedOrderAction.VolumeChange).c_str();
						}
						else if (m_wndParkedOrderActionList.GetItemText(0, col) == _T("�۸�"))
						{
							char OpenPrice_cstr[100];
							sprintf(OpenPrice_cstr, "%0.2f", parkedOrderAction.LimitPrice); //��ȡΪ��λ���Ⱥ�ת��Ϊ�ַ�������
							disp_str = OpenPrice_cstr;
						}
						else if (m_wndParkedOrderActionList.GetItemText(0, col) == _T("״̬"))
						{
							if (parkedOrderAction.Status == THOST_FTDC_PAOS_NotSend)
							{
								disp_str = _T("δ����");
							}
							if (parkedOrderAction.Status == THOST_FTDC_PAOS_Send)
							{
								disp_str = _T("�ѷ���");
							}
							if (parkedOrderAction.Status == THOST_FTDC_PAOS_Deleted)
							{
								disp_str = _T("��ɾ��");
							}
						}
						else if (m_wndParkedOrderActionList.GetItemText(0, col) == _T("Ԥ�񳷵����"))
						{
							disp_str = parkedOrderAction.ParkedOrderActionID;
						}
						else if (m_wndParkedOrderActionList.GetItemText(0, col) == _T("������"))
						{
							disp_str = parkedOrderAction.ExchangeID;
						}
						else if (m_wndParkedOrderActionList.GetItemText(0, col) == _T("�������"))
						{
							disp_str = parkedOrderAction.OrderSysID;
						}
						else if (m_wndParkedOrderActionList.GetItemText(0, col) == _T("������Ϣ"))
						{
							disp_str = parkedOrderAction.ErrorMsg;
							if (disp_str == _T(""))
							{
								disp_str = _T("��");
							}
						}
						else
						{
							disp_str = _T("---");
						}
						Item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;
						Item.strText.Format(_T("%s"), disp_str);
						if (bFined)//���ӱ��п����ҵ��Ѵ��иú�Լ����Ϣ����ֻ��ˢ��
						{
							m_wndParkedOrderActionList.SetItem(&Item);
						}
						else
						{
							row = m_wndParkedOrderActionList.InsertRow(Instrument_id);//����һ��,��һ�е����ݾ��Ǻ�Լ��
							bFined = true;
						}
					}
					m_wndParkedOrderActionList.Refresh();
				}
			}
		}
		return 1;
	}

	if (wParam == ctp_msg::RspParkedOrderAction)
	{
		{//�����ڷǽ���ʱ��,��Ԥ�񳷵����ܳɹ�,����Ԥ�񳷵��ɹ���,ˢ��Ԥ�񳷵���¼�б�
			ParkedOrderAction* pParkedOrderAction = reinterpret_cast<ParkedOrderAction*>(lParam);
			ParkedOrderAction parkedOrderAction = *pParkedOrderAction;
			if (pParkedOrderAction != NULL)
			{
				int row = 0;
				CString Instrument_id(parkedOrderAction.InstrumentID);//��ȡ��ͷ
				CString parkedOrderAction_id(parkedOrderAction.ParkedOrderActionID);//��ȡ�������ݵı������
				bool bFined = false;//һ��ʼ���費����
				row = m_wndParkedOrderActionList.GetRowCount(); //һ��ʼ���費����,�趨������Index
				for (int row_index = 1; row_index < m_wndParkedOrderActionList.GetRowCount(); row_index++)
				{
					if (parkedOrderAction_id == m_wndParkedOrderActionList.GetItemText(row_index, m_wndParkedOrderActionList.GetColumnCount() - 1)) //Ҫ��֤���һ��ΪԤ�񱨵����
					{
						bFined = true;
						row = row_index; //��ȡrow index
						break;
					}
				}
				int col = 0;
				for (col = 0; col < m_wndParkedOrderActionList.GetColumnCount(); col++)
				{//ÿһ�ж�Ӧһ���ֶ�
					GV_ITEM Item;
					Item.mask = GVIF_TEXT | GVIF_FORMAT;
					Item.row = row;//ͨ����һ��ѭ�������ҵ���row�Ѿ�����,����δ�ҵ������ĩβ�е���һ��,ָ��item��������
					Item.col = col;//��ͬ�����������к�,ָ��item��������
					CString disp_str;
					if (m_wndParkedOrderActionList.GetItemText(0, col) == _T("��Լ"))
					{
						disp_str = parkedOrderAction.InstrumentID;
					}
					else if (m_wndParkedOrderActionList.GetItemText(0, col) == _T("������"))
					{
						disp_str = to_string(parkedOrderAction.VolumeChange).c_str();
					}
					else if (m_wndParkedOrderActionList.GetItemText(0, col) == _T("�۸�"))
					{
						char OpenPrice_cstr[100];
						sprintf(OpenPrice_cstr, "%0.2f", parkedOrderAction.LimitPrice); //��ȡΪ��λ���Ⱥ�ת��Ϊ�ַ�������
						disp_str = OpenPrice_cstr;
					}
					else if (m_wndParkedOrderActionList.GetItemText(0, col) == _T("״̬"))
					{
						if (parkedOrderAction.Status == THOST_FTDC_PAOS_NotSend)
						{
							disp_str = _T("δ����");
						}
						if (parkedOrderAction.Status == THOST_FTDC_PAOS_Send)
						{
							disp_str = _T("�ѷ���");
						}
						if (parkedOrderAction.Status == THOST_FTDC_PAOS_Deleted)
						{
							disp_str = _T("��ɾ��");
						}
					}
					else if (m_wndParkedOrderActionList.GetItemText(0, col) == _T("Ԥ�񳷵����"))
					{
						disp_str = parkedOrderAction.ParkedOrderActionID;
					}
					else if (m_wndParkedOrderActionList.GetItemText(0, col) == _T("������"))
					{
						disp_str = parkedOrderAction.ExchangeID;
					}
					else if (m_wndParkedOrderActionList.GetItemText(0, col) == _T("�������"))
					{
						disp_str = parkedOrderAction.OrderSysID;
					}
					else if (m_wndParkedOrderActionList.GetItemText(0, col) == _T("������Ϣ"))
					{
						disp_str = parkedOrderAction.ErrorMsg;
						if (disp_str == _T(""))
						{
							disp_str = _T("��");
						}
					}
					else
					{
						disp_str = _T("---");
					}
					Item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;
					Item.strText.Format(_T("%s"), disp_str);
					if (bFined)//���ӱ��п����ҵ��Ѵ��иú�Լ����Ϣ����ֻ��ˢ��
					{
						m_wndParkedOrderActionList.SetItem(&Item);
					}
					else
					{
						row = m_wndParkedOrderActionList.InsertRow(Instrument_id);//����һ��,��һ�е����ݾ��Ǻ�Լ��
						bFined = true;
					}
				}
				m_wndParkedOrderActionList.Refresh();
			}
		}
		return 1;
	}

	if (wParam == ctp_msg::RspRemoveParkedOrderAction)
	{//ɾ����ֻ����״̬
		Remove_ParkedOrderAction* pParkedOrderAction = reinterpret_cast<Remove_ParkedOrderAction*>(lParam);
		CString parkedOrderActionID(pParkedOrderAction->ParkedOrderActionID);
		int row = 0;
		for (int row_index = 1; row_index < m_wndParkedOrderActionList.GetRowCount(); row_index++)
		{
			if (parkedOrderActionID == m_wndParkedOrderActionList.GetItemText(row_index, m_wndParkedOrderActionList.GetColumnCount() - 1)) //Ҫ��֤���һ��ΪԤ�񱨵����
			{
				row = row_index; //��ȡrow index
				break;
			}
		}
		CString disp_str;
		int col = 0;
		for (col = 0; col < m_wndParkedOrderActionList.GetColumnCount(); col++)
		{//ÿһ�ж�Ӧһ���ֶ�
			GV_ITEM Item;
			Item.mask = GVIF_TEXT | GVIF_FORMAT;
			Item.row = row;//ͨ����һ��ѭ�������ҵ���row�Ѿ�����,����δ�ҵ������ĩβ�е���һ��,ָ��item��������
			Item.col = col;//��ͬ�����������к�,ָ��item��������
			if (m_wndParkedOrderActionList.GetItemText(0, col) == _T("״̬"))
			{
				disp_str = _T("��ɾ��");
				Item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;
				Item.strText.Format(_T("%s"), disp_str);
				m_wndParkedOrderActionList.SetItem(&Item);
				break;
			}
		}
		m_wndParkedOrderActionList.Refresh();
	}

}


/////////////////////////////////////////////////////////////////////////////
// COutputList1

CTradeSituationWnd::CTradeSituationWnd()
{
}

CTradeSituationWnd::~CTradeSituationWnd()
{
}

BEGIN_MESSAGE_MAP(CTradeSituationWnd, CListBox)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// COutputList ��Ϣ�������

void CTradeSituationWnd::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_OUTPUT_POPUP);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
}

void CTradeSituationWnd::OnEditCopy()
{
	MessageBox(_T("�������"));
}

void CTradeSituationWnd::OnEditClear()
{
	MessageBox(_T("������"));
}

void CTradeSituationWnd::OnViewOutput()
{
	CDockablePane* pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

	if (pMainFrame != NULL && pParentBar != NULL)
	{
		pMainFrame->SetFocus();
		pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
		pMainFrame->RecalcLayout();

	}
}