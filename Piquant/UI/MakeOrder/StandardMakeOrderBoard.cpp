// StandardMakeOrderBoard.cpp : implementation file
//

#include "stdafx.h"
#include "../App.h"
#include "StandardMakeOrderBoard.h"
#include "../../ServiceLayer/DataTypes.h" //�˴���Ҫ�õ�ctp������е����ݽṹ
#include "atlconv.h" //ʹ��USES_CONVERSION��Ҫ�õ���ͷ�ļ�
#include "../../../sdk/ThostFtdcUserApiDataType.h"
#include "../GlobalMgr.h"
#include "../../ServiceLayer/CtpService.h"
#include "../../ServiceLayer/CtpMsgDistributor.h"
#include "../../ServiceLayer/CtpMsg.h"


// CStandardMakeOrderBoard

IMPLEMENT_DYNCREATE(CStandardMakeOrderBoard, CFormView)

CStandardMakeOrderBoard::CStandardMakeOrderBoard(CWnd* pParent)
	: CFormView(CStandardMakeOrderBoard::IDD)
	, m_strInstrumentID(_T(""))
	, m_nInstrumentVolume(0)
	, m_strExchange(_T(""))
	, m_strTouchPriceType(_T(""))
	, m_InstrumentPrice(0)
	, m_TouchPrice(0)
	, m_TradePrice(0)
	, m_strTimeCondition(_T(""))
{//���µ�����Ĺ��캯���д����µ���ʵ��
	CCreateContext *pContext = NULL;
	if (!Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 300, 300), pParent, 0, pContext))
	{
		AfxMessageBox(_T("�����µ���ʧ��"));
	}
	OnInitialUpdate();
}

CStandardMakeOrderBoard::~CStandardMakeOrderBoard()
{
}

void CStandardMakeOrderBoard::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_CBString(pDX, COMBO_INSTRUMENTS, m_strInstrumentID);
	DDX_Text(pDX, EDIT_VOLUME, m_nInstrumentVolume);
	DDX_Control(pDX, BUTTON_PRICETYPE, m_btnPriceType);
	DDX_Control(pDX, COMBO_INSTRUMENTS, m_cbInstruments);
	DDX_Control(pDX, COMBO_EXCHANGE, m_cbExchanges);
	DDX_CBString(pDX, COMBO_EXCHANGE, m_strExchange);
	DDX_CBString(pDX, COMBO_TOUCHPRICETYPE, m_strTouchPriceType);
	DDX_Control(pDX, COMBO_TOUCHPRICETYPE, m_cbTouchPriceType);
	DDX_Text(pDX, EDIT_PRICE, m_InstrumentPrice);
	DDX_Text(pDX, EDIT_STOPPRICE, m_TouchPrice);
	DDX_Text(pDX, EDIT_TRADEPRICE, m_TradePrice);
	DDX_Control(pDX, COMBO_TimeCondition, m_cbTimeCondition);
	DDX_CBString(pDX, COMBO_TimeCondition, m_strTimeCondition);
	DDX_Control(pDX, DATETIMEPICKER_GTD, m_GTD);
	DDX_Control(pDX, BUTTON_TOUCHPRICE_PRICETYPE, m_btnPriceType_TouchPrice);
	DDX_Control(pDX, COMBO_PRICETYPE, m_cbPriceType);
	DDX_Control(pDX, SPIN_VOLUME, m_SpinVolumeChange);
	DDX_Control(pDX, COMBO_PRICETYPE_NORMAL, m_cbPriceType_Normal);
	DDX_Control(pDX, SPIN_PRICE, m_SpinPriceChange);
	DDX_Control(pDX, SPIN_TOUCHPRICE, m_SpinTouchPriceChange);
	DDX_Control(pDX, SPIN_PRICE_TOUCH, m_SpinPriceChange_Touch);
	DDX_Control(pDX, STATIC_GROUPBOX, m_gbPriceDetail);
}

BEGIN_MESSAGE_MAP(CStandardMakeOrderBoard, CFormView)
	ON_BN_CLICKED(BUTTON_ORDER_INSERT, &CStandardMakeOrderBoard::OnBnClickedOrderInsert)
	ON_BN_CLICKED(BUTTON_PRICETYPE, &CStandardMakeOrderBoard::OnBnClickedPricetype)
	ON_BN_CLICKED(BUTTON_TOUCHPRICE_PRICETYPE, &CStandardMakeOrderBoard::OnBnClickedTouchpricePricetype)
	ON_NOTIFY(UDN_DELTAPOS, SPIN_VOLUME, &CStandardMakeOrderBoard::OnDeltaposVolume)
	ON_EN_CHANGE(EDIT_VOLUME, &CStandardMakeOrderBoard::OnEnChangeVolume)
	ON_EN_CHANGE(EDIT_PRICE, &CStandardMakeOrderBoard::OnEnChangePrice)
	ON_NOTIFY(UDN_DELTAPOS, SPIN_PRICE, &CStandardMakeOrderBoard::OnDeltaposPrice)
	ON_NOTIFY(UDN_DELTAPOS, SPIN_TOUCHPRICE, &CStandardMakeOrderBoard::OnDeltaposTouchprice)
	ON_NOTIFY(UDN_DELTAPOS, SPIN_PRICE_TOUCH, &CStandardMakeOrderBoard::OnDeltaposPriceTouch)
	ON_EN_CHANGE(EDIT_STOPPRICE, &CStandardMakeOrderBoard::OnEnChangeStopprice)
	ON_EN_CHANGE(EDIT_TRADEPRICE, &CStandardMakeOrderBoard::OnEnChangeTradeprice)
	ON_BN_CLICKED(BUTTON_RETSET, &CStandardMakeOrderBoard::OnBnClickedRetset)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_UPDATETICKDETAILDISP, &CStandardMakeOrderBoard::OnUpdateTickDetailDisp)//����Ϣ����Ϣӳ�亯�����
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CStandardMakeOrderBoard diagnostics

#ifdef _DEBUG
void CStandardMakeOrderBoard::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CStandardMakeOrderBoard::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CStandardMakeOrderBoard message handlers


void CStandardMakeOrderBoard::OnBnClickedOrderInsert()
{
	UpdateData(TRUE);	//���ؼ��������ˢ�µ��󶨵Ŀؼ�������
	USES_CONVERSION;
	InputOrderParam order_param;
	InputParkedOrderParam parkedorder_param;
	//����Լ
	order_param.InstrumentID =T2A(m_strInstrumentID);//T2A��mfc��cstring����ת��Ϊchar*����
	parkedorder_param.InstrumentID = T2A(m_strInstrumentID);

	//��佻����
	order_param.ExchangeID = T2A(m_strExchange);//T2A��mfc��cstring����ת��Ϊchar*����
	parkedorder_param.ExchangeID = T2A(m_strExchange);

	//��䷽��
	CButton* pBuyButton = reinterpret_cast<CButton*>(this->GetDlgItem(RADIO_BUY));
	if (pBuyButton != NULL)
	{//�����ť�Ƿ�ѡ����ͨ��GetCheck() == BST_CHECKED���Ƚϣ���Ϊ��ѡ��ť����Ĭ�ϰ�ť���������δѡ��״������˶��ڶ�ѡ��ĵ�ѡ��ť��ϣ�ֻ���ж�����һ������
		order_param.Direction = pBuyButton->GetCheck() == BST_CHECKED ? THOST_FTDC_D_Buy : THOST_FTDC_D_Sell;
		parkedorder_param.Direction = pBuyButton->GetCheck() == BST_CHECKED ? THOST_FTDC_D_Buy : THOST_FTDC_D_Sell;
	}
	else
	{
		ASSERT(FALSE); //�������������ж�ִ��
	}

	//��俪ƽ��־
	CButton* pOpenCloseButton = reinterpret_cast<CButton*>(this->GetDlgItem(RADIO_OPEN));
	if (pOpenCloseButton->GetCheck() == BST_CHECKED)
	{
		order_param.CombOffsetFlag[0] = THOST_FTDC_OF_Open; //���Ϊ����
		parkedorder_param.CombOffsetFlag[0] = THOST_FTDC_OF_Open; //���Ϊ����
	}
	pOpenCloseButton = reinterpret_cast<CButton*>(this->GetDlgItem(RADIO_CLOSE));
	if (pOpenCloseButton->GetCheck() == BST_CHECKED)
	{
		order_param.CombOffsetFlag[0] = THOST_FTDC_OF_Close; //���Ϊƽ��
		parkedorder_param.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
	}
	pOpenCloseButton = reinterpret_cast<CButton*>(this->GetDlgItem(RADIO_CLOSETODAY));
	if (pOpenCloseButton->GetCheck() == BST_CHECKED)
	{
		order_param.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;  //���Ϊƽ��
		parkedorder_param.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
	}

	//�������
	order_param.VolumeTotalOriginal = m_nInstrumentVolume;
	parkedorder_param.VolumeTotalOriginal = m_nInstrumentVolume;

	//���۸�����
	order_param.OrderPriceType = THOST_FTDC_OPT_LimitPrice; //Ĭ�����޼ۣ���ť��Ĭ��ֵҲ���޼�
	parkedorder_param.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	CString priceTypeName;
	m_btnPriceType.GetWindowText(priceTypeName); //��ȡ�۸����Ͱ�ť�ı�ǩֵ
	if (priceTypeName == "�м�")
	{
		order_param.OrderPriceType = THOST_FTDC_OPT_AnyPrice;  //Ϊ�����м��µ�
		parkedorder_param.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
	}

	//��䴥������
	CButton* pCommissionORAdvanceButton = reinterpret_cast<CButton*>(this->GetDlgItem(RADIO_COMMISSION));
	if (pCommissionORAdvanceButton->GetCheck() == BST_CHECKED)
	{//��ѡ��ί�а�ť���򴥷�����Ϊ�����޼�/�м�ί��
		order_param.ContingentCondition = THOST_FTDC_CC_Immediately; //�޼۵����м۵���Ĭ�ϴ������Ͷ��������������������������޼۵��������м۵�
	}
	pCommissionORAdvanceButton = reinterpret_cast<CButton*>(this->GetDlgItem(RADIO_ADVANCE));
	if (pCommissionORAdvanceButton->GetCheck() == BST_CHECKED)
	{//��ѡ��Ԥ��ť���򴥷�����Ϊ�����޼�/�м�Ԥ��
		parkedorder_param.ContingentCondition = THOST_FTDC_CC_Immediately; 
	}

	//��¼�۸�
	order_param.LimitPrice = m_InstrumentPrice; //��䱨���۸�
	parkedorder_param.LimitPrice = m_InstrumentPrice;

	//������Ч������
	order_param.TimeCondition = THOST_FTDC_TC_GFD; //Ĭ�ϵ�����Ч
	parkedorder_param.TimeCondition = THOST_FTDC_TC_GFD;

	//������������
	if (BST_CHECKED == IsDlgButtonChecked(CHECK_TOUCHPRICETYPE)) //���۽����Ƿ�ѡ�У�����ѡ�У���Ϊ�޼����͵�ͬʱ��ҲҪ�޸�ȷ���䴥����������
	{
		order_param.StopPrice = m_TouchPrice; //��䴥���۸�
		order_param.LimitPrice = m_TradePrice;//��䱨���۸�
		//����������
		if (strcmp(T2A(m_strTouchPriceType), "BuyStop") == 0)
		{
			order_param.ContingentCondition = THOST_FTDC_CC_LastPriceGreaterEqualStopPrice;
			order_param.Direction = THOST_FTDC_D_Buy;
		}
		if (strcmp(T2A(m_strTouchPriceType), "SellStop") == 0)
		{
			order_param.ContingentCondition = THOST_FTDC_CC_LastPriceLesserEqualStopPrice;
			order_param.Direction = THOST_FTDC_D_Sell;
		}
		if (strcmp(T2A(m_strTouchPriceType), "BuyLimit") == 0)
		{
			order_param.ContingentCondition = THOST_FTDC_CC_LastPriceLesserEqualStopPrice;
			order_param.Direction = THOST_FTDC_D_Buy;
		}
		if (strcmp(T2A(m_strTouchPriceType), "SellLimit") == 0)
		{
			order_param.ContingentCondition = THOST_FTDC_CC_LastPriceGreaterEqualStopPrice;
			order_param.Direction = THOST_FTDC_D_Sell;
		}
		//������Ч������
		if (strcmp(T2A(m_strTimeCondition), "������Ч") == 0)
		{
			order_param.TimeCondition = THOST_FTDC_TC_GFD;
		}
		if (strcmp(T2A(m_strTimeCondition), "����ǰ��Ч") == 0)
		{
			order_param.TimeCondition = THOST_FTDC_TC_GTC;
		}
		if (strcmp(T2A(m_strTimeCondition), "ָ������ǰ��Ч") == 0)
		{
			order_param.TimeCondition = THOST_FTDC_TC_GTD;
			CTime Date;
			m_GTD.GetTime(Date);
			CString strTime = Date.Format("%Y%m%d");
			order_param.GTDDate = T2A(strTime); //T2A��cstring����ת��Ϊchar*���ͣ�֮���ٽ�char*���͸���string����
		}
		//�����˽⣬ctpĿǰֻ֧�ֵ�����Ч��Ϊ�������¸��ӿڣ�����Ĭ��Ϊ������Ч��GTD�ֶ�Ҳ����Ҫ
		order_param.TimeCondition = THOST_FTDC_TC_GFD;

		//����۸����ͣ�Ĭ�ϴ��ۺ�ת�޼ۣ�Ҳ�ɴ��ۺ�ת�м�
		order_param.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
		CString priceTypeName_TouchPrice;
		m_btnPriceType_TouchPrice.GetWindowText(priceTypeName_TouchPrice); 
		if (priceTypeName_TouchPrice == "�м�")
		{
			order_param.OrderPriceType = THOST_FTDC_OPT_AnyPrice;  //���ۺ�ת�м�
		}

		//�˴������������޿��֣���Ϊƽ�֣�������ֹ��ֹӯ�������ڴ˴�����
		order_param.CombOffsetFlag[0] = THOST_FTDC_OF_Open; //���Ϊ����
	}
	if (::MessageBox(NULL, _T("�Ƿ�ȷ���µ�"), _T("ȷ��"), MB_OKCANCEL) == IDOK)
	{
		if (BST_CHECKED == IsDlgButtonChecked(CHECK_TOUCHPRICETYPE))
		{//���۽��ױ�ѡ�У���ί�е�
			get_global_mgr().GetService()->request(ctp_msg::MakeOrder, &order_param);
			return;
		}
		pCommissionORAdvanceButton = reinterpret_cast<CButton*>(this->GetDlgItem(RADIO_COMMISSION));
		if (pCommissionORAdvanceButton->GetCheck() == BST_CHECKED)
		{//����ѡί�У���ί�е�
			get_global_mgr().GetService()->request(ctp_msg::MakeOrder, &order_param);
			return;
		}
		pCommissionORAdvanceButton = reinterpret_cast<CButton*>(this->GetDlgItem(RADIO_ADVANCE));
		if (pCommissionORAdvanceButton->GetCheck() == BST_CHECKED)
		{//����ѡ��Ԥ����Ԥ��
			get_global_mgr().GetService()->request(ctp_msg::Make_ParkedOrder, &parkedorder_param);
			return;
		}
	}
}


void CStandardMakeOrderBoard::OnBnClickedPricetype()
{
	CString priceTypeName;
	m_btnPriceType.GetWindowText(priceTypeName); //���۸����Ͱ�ť��ֵͨ��m_btnPriceType.GetWindowText��ȡ��priceTypeName������
	if (priceTypeName == "�޼�")
	{//���ð�ť����ȥ������ǰΪ�޼ۣ����º��Ϊ�м�
		//order_param.OrderPriceType=THOST_FTDC_OPT_AnyPrice;
		m_btnPriceType.SetWindowText(_T("�м�"));
	}
	else
	{
		m_btnPriceType.SetWindowText(_T("�޼�"));
	}
}


void CStandardMakeOrderBoard::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	get_CtpMsgDistributor()->AddReceiver(this);//��CStandardMakeOrderBoard��ʵ����ӽ�������������

	//�ڷ���ѡ��ť���У�Ĭ��ѡ����ť
	CButton* pBuyButton = reinterpret_cast<CButton*>(this->GetDlgItem(RADIO_BUY));
	pBuyButton->SetCheck(true);

	//�ڿ�ƽ��ѡ��ť���У�Ĭ��ѡ�����ְ�ť
	CButton* pOpenCloseButton = reinterpret_cast<CButton*>(this->GetDlgItem(RADIO_OPEN));
	pOpenCloseButton->SetCheck(true);

	//��ί����Ԥ��ѡ��ť���У�Ĭ��ѡ��ί�а�ť
	CButton* pCommissionORAdvanceButton = reinterpret_cast<CButton*>(this->GetDlgItem(RADIO_COMMISSION));
	pCommissionORAdvanceButton->SetCheck(true);

	//����Ψһ��get_global_mgr�ⲿ�ӿ�ȥ��ȡctpservice����, ������ȡ����api��������api����, ��������ȡ��Ӧ����Ϣ
	vector<BaseData> instruments;
	if (NULL != get_global_mgr().GetService())
	{
		get_global_mgr().GetService()->get_basic_data(instruments);
		if (!instruments.empty())
		{//������, ��������䵽��Ͽ���ȥ
			if (m_cbInstruments.GetCount() != 0)
			{//��ֹ���μ���,Ϊ���ټ���ǰ����Ϊ��,�����������
				m_cbInstruments.ResetContent();
			}
			for (int i = 0; i < instruments.size(); ++i)
			{
				m_cbInstruments.AddString(CString(instruments[i].code));
			}
		}
	}

	vector<Exchange> exchanges;
	if (NULL != get_global_mgr().GetService())
	{
		get_global_mgr().GetService()->get_exchanges(exchanges);
		if (!exchanges.empty())
		{//������, ��������䵽��Ͽ���ȥ
			if (m_cbExchanges.GetCount() != 0)
			{//��ֹ���μ���,Ϊ���ټ���ǰ����Ϊ��,�����������
				m_cbExchanges.ResetContent();
			}
			for (int j = 0; j <exchanges.size(); ++j)
			{
				m_cbExchanges.AddString(CString(exchanges[j].ExchangeID.c_str()));
			}
			m_cbExchanges.SetCurSel(0);
		}
	}

	if (m_cbTouchPriceType.GetCount()!=0)
	{//����Ϊ�գ���ֹ�����ظ�����
		m_cbTouchPriceType.ResetContent();
	}
	m_cbTouchPriceType.AddString(CString("BuyStop"));
	m_cbTouchPriceType.AddString(CString("SellStop"));
	m_cbTouchPriceType.AddString(CString("BuyLimit"));
	m_cbTouchPriceType.AddString(CString("SellLimit"));
	m_cbTouchPriceType.SetCurSel(0);

	if (m_cbTimeCondition.GetCount()!=0)
	{
		m_cbTimeCondition.ResetContent();
	}
	m_cbTimeCondition.AddString(CString("������Ч"));
	m_cbTimeCondition.AddString(CString("ָ������ǰ��Ч"));
	m_cbTimeCondition.AddString(CString("����ǰ��Ч"));
	m_cbTimeCondition.SetCurSel(1);

	if (m_cbPriceType.GetCount() != 0)
	{
		m_cbPriceType.ResetContent();
	}
	m_cbPriceType.AddString(CString("ָ��������"));
	//m_cbPriceType.AddString(CString("���̼�"));//������û�и��̼�ѡ��
	m_cbPriceType.SetCurSel(0);

	if (m_cbPriceType_Normal.GetCount() != 0)
	{
		m_cbPriceType_Normal.ResetContent();
	}
	m_cbPriceType_Normal.AddString(CString("ָ����"));
	m_cbPriceType_Normal.AddString(CString("���̼�")); 
	m_cbPriceType_Normal.SetCurSel(1);

	CString cstr;
	cstr.Format(_T("%d"), 1);
	GetDlgItem(EDIT_VOLUME)->SetWindowText(cstr);
	GetDlgItem(EDIT_PRICE)->SetWindowText(cstr);
	GetDlgItem(EDIT_STOPPRICE)->SetWindowText(cstr);
	GetDlgItem(EDIT_TRADEPRICE)->SetWindowText(cstr);

	m_SpinVolumeChange.SetBuddy(GetDlgItem(m_nInstrumentVolume)); //���ı����뽻�����䶯��ť�Ļ���ϵ
	m_SpinVolumeChange.SetRange32(0, 1000);//��ʾ��ֵֻ����0��10�ڱ仯
	m_SpinVolumeChange.SetBase(10);//���ý�����,ֻ����10���ƺ�16����

	m_SpinPriceChange.SetBuddy(GetDlgItem(m_InstrumentPrice)); //�󶨼۸��ı�����۸�䶯��ť�Ļ���ϵ
	m_SpinPriceChange.SetRange32(0, 999999);//��ʾ��ֵֻ����0��999999�ڱ仯
	m_SpinPriceChange.SetBase(10);//���ý�����,ֻ����10���ƺ�16����

	m_SpinTouchPriceChange.SetBuddy(GetDlgItem(m_TouchPrice)); //�󶨴����۸��ı�����۸�䶯��ť�Ļ���ϵ
	m_SpinTouchPriceChange.SetRange32(0, 999999);//��ʾ��ֵֻ����0��999999�ڱ仯
	m_SpinTouchPriceChange.SetBase(10);//���ý�����,ֻ����10���ƺ�16����

	m_SpinPriceChange_Touch.SetBuddy(GetDlgItem(m_TradePrice)); //�󶨴��������۸��ı�����۸�䶯��ť�Ļ���ϵ
	m_SpinPriceChange_Touch.SetRange32(0, 999999);//��ʾ��ֵֻ����0��999999�ڱ仯
	m_SpinPriceChange_Touch.SetBase(10);//���ý�����,ֻ����10���ƺ�16����

	SetWindowTheme(GetDlgItem(STATIC_GROUPBOX)->GetSafeHwnd(), L"", L"");//��������Group Box�ؼ��õģ�����Ե��δ�
	SetWindowTheme(GetDlgItem(STATIC_PRICEDETAIL_SUBBOX1)->GetSafeHwnd(), L"", L""); 
	SetWindowTheme(GetDlgItem(STATIC_PRICEDETAIL_SUBBOX2)->GetSafeHwnd(), L"", L""); 
	SetWindowTheme(GetDlgItem(STATIC_PRICEDETAIL_SUBBOX3)->GetSafeHwnd(), L"", L"");


	CFont font_Ask;
	font_Ask.CreatePointFont(120, L"����");
	GetDlgItem(STATIC_ASK)->SetFont(&font_Ask);
	CFont font_Bid;
	font_Bid.CreatePointFont(120, L"����");
	GetDlgItem(STATIC_BID)->SetFont(&font_Bid);
	CFont font_AskValue;
	font_AskValue.CreatePointFont(100, L"����");
	GetDlgItem(STATIC_ASKVALUE)->SetFont(&font_AskValue);
	CFont font_BidValue;
	font_BidValue.CreatePointFont(100, L"����");
	GetDlgItem(STATIC_BIDVALUE)->SetFont(&font_BidValue);
	CFont font_AskVolume;
	font_AskVolume.CreatePointFont(100, L"����");
	GetDlgItem(STATIC_ASKVOLUME)->SetFont(&font_AskVolume);
	CFont font_BidVolume;
	font_BidVolume.CreatePointFont(100, L"����");
	GetDlgItem(STATIC_BIDVOLUME)->SetFont(&font_BidVolume);

	CFont font;
	font.CreatePointFont(120, L"����");
	GetDlgItem(STATIC_LASTPRICE)->SetFont(&font);
	GetDlgItem(STATIC_PreSettlementPrice)->SetFont(&font);
	GetDlgItem(STATIC_SettlementPrice)->SetFont(&font);
	GetDlgItem(STATIC_PreClosePrice)->SetFont(&font);
	GetDlgItem(STATIC_UpperLimitPrice)->SetFont(&font);
	GetDlgItem(STATIC_OpenPrice)->SetFont(&font);
	GetDlgItem(STATIC_LowerLimitPrice)->SetFont(&font);
	GetDlgItem(STATIC_HighestPrice)->SetFont(&font);
	GetDlgItem(STATIC_LowestPrice)->SetFont(&font);
	GetDlgItem(STATIC_RANGE)->SetFont(&font);
	GetDlgItem(STATIC_LASTPRICE_VALUE)->SetFont(&font);
	GetDlgItem(STATIC_PreSettlementPrice_value)->SetFont(&font);
	GetDlgItem(STATIC_SettlementPrice_value)->SetFont(&font);
	GetDlgItem(STATIC_PreClosePrice_value)->SetFont(&font);
	GetDlgItem(STATIC_UpperLimitPrice_value)->SetFont(&font);
	GetDlgItem(STATIC_OpenPrice_value)->SetFont(&font);
	GetDlgItem(STATIC_LowerLimitPrice_value)->SetFont(&font);
	GetDlgItem(STATIC_HighestPrice_value)->SetFont(&font);
	GetDlgItem(STATIC_LowestPrice_value)->SetFont(&font);
	GetDlgItem(STATIC_RANGE_value)->SetFont(&font);
	GetDlgItem(STATIC_tickInstrument)->SetFont(&font);
	GetDlgItem(STATIC_tickExchange)->SetFont(&font);


}

void CStandardMakeOrderBoard::OnReceiveData(WPARAM wParam, LPARAM lParam)
{
	if (wParam == ctp_msg::RspQryInstrument)
	{//������api��ѯ�����к�Լ��Ϣ���ص����ص�ʱ��, �����ڴ˷���WM_INITIALUPDATE��Ϣ, ��OnInitialUpdate����ȥ���¸�ѡ��
		PostMessage(WM_INITIALUPDATE,  0, 0); //WM_INITIALUPDATE��Ϣ�ķ��ͻᴥ��CStandardMakeOrderBoard::OnInitialUpdate()�����ĵ���
	}
	if (wParam == ctp_msg::RspQryexchanges)
	{
		PostMessage(WM_INITIALUPDATE, 0, 0);//������Ϣȥ���½�������ѡ��
	}
	if (wParam==ctp_msg::RtnMarketData)
	{
		PostMessage(WM_UPDATETICKDETAILDISP, RtnMarketData, lParam);
	}
}



void CStandardMakeOrderBoard::OnBnClickedTouchpricePricetype()
{
	CString priceTypeName;
	m_btnPriceType_TouchPrice.GetWindowText(priceTypeName); 
	if (priceTypeName == "�޼�")
	{//���ð�ť����ȥ������ǰΪ�޼ۣ����º��Ϊ�м�
		m_btnPriceType_TouchPrice.SetWindowText(_T("�м�"));
	}
	else
	{
		m_btnPriceType_TouchPrice.SetWindowText(_T("�޼�"));
	}
}






void CStandardMakeOrderBoard::OnDeltaposVolume(NMHDR *pNMHDR, LRESULT *pResult)
{//���ܿ��ƽ������ı߽緶Χ
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	UpdateData(true); //������ˢ�µ�ֵ������
	USES_CONVERSION;
	UINT  maxVolume = 1000;
	UINT  minVolume = 1;
	char* InstrumentID = T2A(m_strInstrumentID);//T2A��mfc��cstring����ת��Ϊchar*����
	CString priceTypeName;
	m_btnPriceType.GetWindowText(priceTypeName); //��ȡ�۸����Ͱ�ť�ı�ǩֵ
	bool IsAnyPrice = false; //Ĭ�Ϸ��м�,�����޼�
	if (priceTypeName == "�м�")
	{
		IsAnyPrice=true;  //Ϊ�м��µ�
	}

	if (strlen(InstrumentID) != 0)
	{
		vector<BaseData> instruments;
		if (NULL != get_global_mgr().GetService())
		{
			get_global_mgr().GetService()->get_basic_data(instruments);
			for (vector<BaseData>::iterator it = instruments.begin(); it != instruments.end(); ++it)
			{
				if (strcmp(it->code,InstrumentID)==0)
				{
					if (IsAnyPrice==false)
					{
						maxVolume = it->MaxLimitOrderVolume;
						minVolume = it->MinLimitOrderVolume;
						break;
					}
					if (IsAnyPrice==true)
					{
						maxVolume = it->MaxMarketOrderVolume;
						minVolume = it->MinMarketOrderVolume;
						break;
					}
				}
			}
		}
		UINT calculateVolume;
		if (pNMUpDown->iDelta == -1) // �����ֵΪ-1 , ˵�������Spin�����µļ�ͷ
		{
			calculateVolume = m_nInstrumentVolume - 1;
			if (calculateVolume < minVolume)  calculateVolume = minVolume;
			if (calculateVolume > maxVolume)  calculateVolume = maxVolume;
		}
		else if (pNMUpDown->iDelta == 1) // �����ֵΪ1, ˵�������Spin�����ϵļ�ͷ
		{
			calculateVolume = m_nInstrumentVolume + 1;
			if (calculateVolume > maxVolume)  calculateVolume = maxVolume;
			if (calculateVolume < minVolume)  calculateVolume = minVolume;
		}
		m_nInstrumentVolume = calculateVolume;
		CString volume_cstr;
		volume_cstr.Format(_T("%d"), m_nInstrumentVolume);
		GetDlgItem(EDIT_VOLUME)->SetWindowText(volume_cstr);
	}
	else
	{
		UINT calculateVolume;
		if (pNMUpDown->iDelta == -1) // �����ֵΪ-1 , ˵�������Spin�����µļ�ͷ
		{
			calculateVolume = m_nInstrumentVolume - 1;
			if (calculateVolume < minVolume)  calculateVolume = minVolume;
			if (calculateVolume > maxVolume)  calculateVolume = maxVolume;
		}
		else if (pNMUpDown->iDelta == 1) // �����ֵΪ1, ˵�������Spin�����ϵļ�ͷ
		{
			calculateVolume = m_nInstrumentVolume + 1;
			if (calculateVolume < minVolume)  calculateVolume = minVolume;
			if (calculateVolume > maxVolume)  calculateVolume = maxVolume;
		}
		m_nInstrumentVolume = calculateVolume;
		CString volume_cstr;
		volume_cstr.Format(_T("%d"), m_nInstrumentVolume);
		GetDlgItem(EDIT_VOLUME)->SetWindowText(volume_cstr);
	}
	//UpdateData(false);
	*pResult = 0;
}


void CStandardMakeOrderBoard::OnEnChangeVolume()
{//��������쳣����
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the __super::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString strTemp = _T("");
	CEdit* editHelp = ((CEdit*)(GetDlgItem(EDIT_VOLUME)));
	editHelp->GetWindowText(strTemp);
	int len = strTemp.GetLength();
	for (int i = 0; i < len; i++)
	{
		if (strTemp.GetAt(i) < '0' || strTemp.GetAt(i) > '9')
		{//�޶��������������
			strTemp = strTemp.Left(i);
			editHelp->SetWindowText(strTemp);
			editHelp->SetSel(i, i, TRUE);
			return;
		}
	}
	editHelp->GetWindowText(strTemp);
	if (strTemp==_T("0"))
	{
		editHelp->SetWindowText(_T("1"));
	}
}


void CStandardMakeOrderBoard::OnEnChangePrice()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the __super::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString strTemp = _T("");
	CEdit* editHelp = ((CEdit*)(GetDlgItem(EDIT_PRICE)));
	editHelp->GetWindowText(strTemp);
	int len = strTemp.GetLength();
	for (int i = 0; i < len; i++)
	{
		if (strTemp.GetAt(i) < '0' || strTemp.GetAt(i) > '9' )
		{//�޶��������������
			if (strTemp.GetAt(i) == '.')
			{//�۸�����Ҫ�ܿ�С����
				continue;
			}
			strTemp = strTemp.Left(i);
			editHelp->SetWindowText(strTemp);
			editHelp->SetSel(i, i, TRUE);
			return;
		}
	}
	editHelp->GetWindowText(strTemp);
	if (strTemp == _T("0"))
	{
		editHelp->SetWindowText(_T("1"));
	}
}


void CStandardMakeOrderBoard::OnDeltaposPrice(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	UpdateData(true); //������ˢ�µ�ֵ������
	USES_CONVERSION;
	double  pricechange_unit = 1;
	char* InstrumentID = T2A(m_strInstrumentID);//T2A��mfc��cstring����ת��Ϊchar*����

	if (strlen(InstrumentID) != 0)
	{
		vector<BaseData> instruments;
		if (NULL != get_global_mgr().GetService())
		{
			get_global_mgr().GetService()->get_basic_data(instruments);
			for (vector<BaseData>::iterator it = instruments.begin(); it != instruments.end(); ++it)
			{
				if (strcmp(it->code, InstrumentID) == 0)
				{
					pricechange_unit = it->PriceTick;
					break;
				}
			}
		}
		double changedPrice;
		if (pNMUpDown->iDelta == -1) // �����ֵΪ-1 , ˵�������Spin�����µļ�ͷ
		{
			changedPrice = m_InstrumentPrice - pricechange_unit;
			if (changedPrice <= 0)  changedPrice = pricechange_unit;
		}
		else if (pNMUpDown->iDelta == 1) // �����ֵΪ1, ˵�������Spin�����ϵļ�ͷ
		{
			changedPrice = m_InstrumentPrice + pricechange_unit;
			if (changedPrice <= 0)  changedPrice = pricechange_unit;
		}
		m_InstrumentPrice = changedPrice;
		CString Price_cstr;
		Price_cstr.Format(_T("%0.2f"), m_InstrumentPrice);
		GetDlgItem(EDIT_PRICE)->SetWindowText(Price_cstr);
	}
	else
	{
		//�����κβ���
	}
	//UpdateData(false);
	*pResult = 0;
}


void CStandardMakeOrderBoard::OnDeltaposTouchprice(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	UpdateData(true); //������ˢ�µ�ֵ������
	USES_CONVERSION;
	double  touchpricechange_unit = 1;
	char* InstrumentID = T2A(m_strInstrumentID);//T2A��mfc��cstring����ת��Ϊchar*����

	if (strlen(InstrumentID) != 0)
	{
		vector<BaseData> instruments;
		if (NULL != get_global_mgr().GetService())
		{
			get_global_mgr().GetService()->get_basic_data(instruments);
			for (vector<BaseData>::iterator it = instruments.begin(); it != instruments.end(); ++it)
			{
				if (strcmp(it->code, InstrumentID) == 0)
				{
					touchpricechange_unit = it->PriceTick;
					break;
				}
			}
		}
		double changedPrice;
		if (pNMUpDown->iDelta == -1) // �����ֵΪ-1 , ˵�������Spin�����µļ�ͷ
		{
			changedPrice = m_TouchPrice - touchpricechange_unit;
			if (changedPrice <= 0)  changedPrice = touchpricechange_unit;
		}
		else if (pNMUpDown->iDelta == 1) // �����ֵΪ1, ˵�������Spin�����ϵļ�ͷ
		{
			changedPrice = m_TouchPrice + touchpricechange_unit;
			if (changedPrice <= 0)  changedPrice = touchpricechange_unit;
		}
		m_TouchPrice = changedPrice;
		CString Price_cstr;
		Price_cstr.Format(_T("%0.2f"), m_TouchPrice);
		GetDlgItem(EDIT_STOPPRICE)->SetWindowText(Price_cstr);
	}
	else
	{
		//�����κβ���
	}
	//UpdateData(false);
	*pResult = 0;
}


void CStandardMakeOrderBoard::OnDeltaposPriceTouch(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	UpdateData(true); //������ˢ�µ�ֵ������
	USES_CONVERSION;
	double  pricechange_touch_unit = 1;
	char* InstrumentID = T2A(m_strInstrumentID);//T2A��mfc��cstring����ת��Ϊchar*����

	if (strlen(InstrumentID) != 0)
	{
		vector<BaseData> instruments;
		if (NULL != get_global_mgr().GetService())
		{
			get_global_mgr().GetService()->get_basic_data(instruments);
			for (vector<BaseData>::iterator it = instruments.begin(); it != instruments.end(); ++it)
			{
				if (strcmp(it->code, InstrumentID) == 0)
				{
					pricechange_touch_unit = it->PriceTick;
					break;
				}
			}
		}
		double changedPrice;
		if (pNMUpDown->iDelta == -1) // �����ֵΪ-1 , ˵�������Spin�����µļ�ͷ
		{
			changedPrice = m_TradePrice - pricechange_touch_unit;
			if (changedPrice <= 0)  changedPrice = pricechange_touch_unit;
		}
		else if (pNMUpDown->iDelta == 1) // �����ֵΪ1, ˵�������Spin�����ϵļ�ͷ
		{
			changedPrice = m_TradePrice + pricechange_touch_unit;
			if (changedPrice <= 0)  changedPrice = pricechange_touch_unit;
		}
		m_TradePrice = changedPrice;
		CString Price_cstr;
		Price_cstr.Format(_T("%0.2f"), m_TradePrice);
		GetDlgItem(EDIT_TRADEPRICE)->SetWindowText(Price_cstr);
	}
	else
	{
		//�����κβ���
	}
	//UpdateData(false);
	*pResult = 0;
}


void CStandardMakeOrderBoard::OnEnChangeStopprice()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the __super::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString strTemp = _T("");
	CEdit* editHelp = ((CEdit*)(GetDlgItem(EDIT_STOPPRICE)));
	editHelp->GetWindowText(strTemp);
	int len = strTemp.GetLength();
	for (int i = 0; i < len; i++)
	{
		if (strTemp.GetAt(i) < '0' || strTemp.GetAt(i) > '9')
		{//�޶��������������
			if (strTemp.GetAt(i) == '.')
			{//�۸�����Ҫ�ܿ�С����
				continue;
			}
			strTemp = strTemp.Left(i);
			editHelp->SetWindowText(strTemp);
			editHelp->SetSel(i, i, TRUE);
			return;
		}
	}
	editHelp->GetWindowText(strTemp);
	if (strTemp == _T("0"))
	{
		editHelp->SetWindowText(_T("1"));
	}
}


void CStandardMakeOrderBoard::OnEnChangeTradeprice()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the __super::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString strTemp = _T("");
	CEdit* editHelp = ((CEdit*)(GetDlgItem(EDIT_TRADEPRICE)));
	editHelp->GetWindowText(strTemp);
	int len = strTemp.GetLength();
	for (int i = 0; i < len; i++)
	{
		if (strTemp.GetAt(i) < '0' || strTemp.GetAt(i) > '9')
		{//�޶��������������
			if (strTemp.GetAt(i) == '.')
			{//�۸�����Ҫ�ܿ�С����
				continue;
			}
			strTemp = strTemp.Left(i);
			editHelp->SetWindowText(strTemp);
			editHelp->SetSel(i, i, TRUE);
			return;
		}
	}
	editHelp->GetWindowText(strTemp);
	if (strTemp == _T("0"))
	{
		editHelp->SetWindowText(_T("1"));
	}
}


void CStandardMakeOrderBoard::OnBnClickedRetset()
{
	// TODO: Add your control notification handler code here
	//��ί����Ԥ��ѡ��ť���У�Ĭ��ѡ��ί�а�ť
	CButton* pCommissionORAdvanceButton = reinterpret_cast<CButton*>(this->GetDlgItem(RADIO_COMMISSION));
	pCommissionORAdvanceButton->SetCheck(true);
	pCommissionORAdvanceButton = reinterpret_cast<CButton*>(this->GetDlgItem(RADIO_ADVANCE));
	pCommissionORAdvanceButton->SetCheck(false);

	//���ü۸�ͽ�����
	CString cstr;
	cstr.Format(_T("%d"), 1);
	GetDlgItem(EDIT_VOLUME)->SetWindowText(cstr);
	GetDlgItem(EDIT_PRICE)->SetWindowText(cstr);
	GetDlgItem(EDIT_STOPPRICE)->SetWindowText(cstr);
	GetDlgItem(EDIT_TRADEPRICE)->SetWindowText(cstr);
	//���۽��׹�ѡ��Ĭ�ϲ�ѡ��
	((CButton*)GetDlgItem(CHECK_TOUCHPRICETYPE))->SetCheck(false);
}


HBRUSH CStandardMakeOrderBoard::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{//ʵ��GroupBox���϶���ɫ���ã�ʵ������δ����������⣬ȡ�ɶ�Ϊ
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO:  Change any attributes of the DC here
	//��Բ�ͬ�ؼ��޸���ɫ
	if (pWnd->GetDlgCtrlID() == STATIC_PRICEDETAIL_SUBBOX1)
	{
	hbr = CreateSolidBrush(RGB(255, 0, 0));//������ɫ
	pDC->SetTextColor(RGB(255, 0, 0));
	pDC->SetBkColor(RGB(255, 0, 0));
	}

	if (pWnd->GetDlgCtrlID() == STATIC_PRICEDETAIL_SUBBOX3)
	{
		hbr = CreateSolidBrush(RGB(0, 0, 255));//������ɫ
		pDC->SetTextColor(RGB(0, 0, 255));
		pDC->SetBkColor(RGB(0, 0, 255));
	}

	if (pWnd->GetDlgCtrlID() == STATIC_ASK)
	{
		pDC->SetTextColor(RGB(255, 0, 0));
	}
	if (pWnd->GetDlgCtrlID() == STATIC_BID)
	{
		pDC->SetTextColor(RGB(0, 0, 255));
	}
	if (pWnd->GetDlgCtrlID() == STATIC_LASTPRICE ||
		pWnd->GetDlgCtrlID() == STATIC_SettlementPrice ||
		pWnd->GetDlgCtrlID() == STATIC_UpperLimitPrice ||
		pWnd->GetDlgCtrlID() == STATIC_LowerLimitPrice ||
		pWnd->GetDlgCtrlID() == STATIC_RANGE)
	{
		pDC->SetTextColor(RGB(255, 0, 0));
	}
	if (pWnd->GetDlgCtrlID() == STATIC_PreSettlementPrice  ||
		pWnd->GetDlgCtrlID() == STATIC_PreClosePrice ||
		pWnd->GetDlgCtrlID() == STATIC_OpenPrice ||
		pWnd->GetDlgCtrlID() == STATIC_HighestPrice ||
		pWnd->GetDlgCtrlID() == STATIC_LowestPrice)
	{
		pDC->SetTextColor(RGB(0, 0, 255));
	}
	if (pWnd->GetDlgCtrlID() == STATIC_tickInstrument ||
		pWnd->GetDlgCtrlID() == STATIC_tickExchange)
	{
		pDC->SetTextColor(RGB(255, 128, 64));
	}
	
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

LRESULT  CStandardMakeOrderBoard::OnUpdateTickDetailDisp(WPARAM wParam, LPARAM lParam)
{
	CString instrumentID;
	CString InstrumentID_combobox;
	CString cstr;
	if (wParam==ctp_msg::RtnMarketData)
	{
		InstrumentID_combobox = _T("");//����
		instrumentID = _T("");//����
		cstr = _T("");//����
		GetDlgItem(COMBO_INSTRUMENTS)->GetWindowText(InstrumentID_combobox);
		if (InstrumentID_combobox == _T(""))//m_strInstrumentIDΪ��Լ���ֵ����
		{
			return 0;
		}
		else
		{//��ѡ����Լ
			MarketData* pmarketData = reinterpret_cast<MarketData*>(lParam);
			MarketData marketData = *pmarketData;
			instrumentID = marketData.code;
			if (InstrumentID_combobox == instrumentID)
			{//�˴������������������²���
				cstr = marketData.code;//tick��Լ
				GetDlgItem(STATIC_tickInstrument)->SetWindowText(cstr);
				cstr = marketData.ExchangeID;//tick������
				GetDlgItem(STATIC_tickExchange)->SetWindowText(cstr);
				cstr.Format(_T("%.2f"), marketData.AskPrice);
				GetDlgItem(STATIC_ASKVALUE)->SetWindowText(cstr);
				cstr.Format(_T("%.2f"), marketData.BidPrice);
				GetDlgItem(STATIC_BIDVALUE)->SetWindowText(cstr);
				cstr.Format(_T("%d"), marketData.AskVolume);
				GetDlgItem(STATIC_ASKVOLUME)->SetWindowText(cstr);
				cstr.Format(_T("%d"), marketData.BidVolume);
				GetDlgItem(STATIC_BIDVOLUME)->SetWindowText(cstr);

				cstr.Format(_T("%.2f"), marketData.LastPrice);
				GetDlgItem(STATIC_LASTPRICE_VALUE)->SetWindowText(cstr);
				cstr.Format(_T("%.2f"), marketData.PreSettlementPrice);
				GetDlgItem(STATIC_PreSettlementPrice_value)->SetWindowText(cstr);
				cstr.Format(_T("%.2f"), marketData.LastPrice-marketData.PreClosePrice);
				GetDlgItem(STATIC_changeprice_value)->SetWindowText(cstr);
				cstr.Format(_T("%.2f"), marketData.PreClosePrice);
				GetDlgItem(STATIC_PreClosePrice_value)->SetWindowText(cstr);
				cstr.Format(_T("%.2f"), marketData.UpperLimitPrice);
				GetDlgItem(STATIC_UpperLimitPrice_value)->SetWindowText(cstr);
				cstr.Format(_T("%.2f"), marketData.OpenPrice);
				GetDlgItem(STATIC_OpenPrice_value)->SetWindowText(cstr);
				cstr.Format(_T("%.2f"), marketData.LowerLimitPrice);
				GetDlgItem(STATIC_LowerLimitPrice_value)->SetWindowText(cstr);
				cstr.Format(_T("%.2f"), marketData.HighestPrice);
				GetDlgItem(STATIC_HighestPrice_value)->SetWindowText(cstr);
				cstr.Format(_T("%.2f"), marketData.LowestPrice);
				GetDlgItem(STATIC_LowestPrice_value)->SetWindowText(cstr);
				double rangevalue;
				if (marketData.PreClosePrice!=0)
				{
					rangevalue = (marketData.LastPrice - marketData.PreClosePrice) / marketData.PreClosePrice *100;//����100��Ϊ�˿��ǰٷֺ���ʾ
					cstr.Format(_T("%.2f%%"), rangevalue);
					GetDlgItem(STATIC_RANGE_value)->SetWindowText(cstr);
				}
				else
				{
					GetDlgItem(STATIC_RANGE_value)->SetWindowText(_T("------"));
				}
				//���ø��̼�
				CString SpecORMarkPrice;
				((CComboBox*)GetDlgItem(COMBO_PRICETYPE_NORMAL))->GetWindowText(SpecORMarkPrice);
				if (SpecORMarkPrice==_T("���̼�"))
				{
					CButton* pBuyButton = reinterpret_cast<CButton*>(this->GetDlgItem(RADIO_BUY));
					if (pBuyButton->GetCheck() == BST_CHECKED)
					{//����ѡ��Ϊ��
						CString price;
						price.Format(_T("%.2f"), marketData.AskPrice);
						GetDlgItem(EDIT_PRICE)->SetWindowText(price);
					}
					else
					{//����ѡ��Ϊ��
						CString price;
						price.Format(_T("%.2f"), marketData.BidPrice);
						GetDlgItem(EDIT_PRICE)->SetWindowText(price);
					}
				}
				return 1;
			}
			else
			{
				return 0;
			}
		}
	}
}





void CStandardMakeOrderBoard::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	get_CtpMsgDistributor()->DelReceiver(this);
	__super::OnClose();
}
