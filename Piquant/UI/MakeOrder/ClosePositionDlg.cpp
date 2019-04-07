// ClosePositionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../App.h"
#include "ClosePositionDlg.h"
#include "afxdialogex.h"
#include "../resource.h"
#include "atlconv.h" //ʹ��USES_CONVERSION��Ҫ�õ���ͷ�ļ�
#include "../../ServiceLayer/DataTypes.h"
#include "../../../sdk/ThostFtdcUserApiDataType.h"
#include "../GlobalMgr.h"
#include "../../ServiceLayer/CtpService.h"
#include "../../ServiceLayer/CtpMsg.h"

// CClosePositionDlg dialog

IMPLEMENT_DYNAMIC(CClosePositionDlg, CDialogEx)

CClosePositionDlg::CClosePositionDlg(CString InstrumentID,
														CString Direction,
														CString Position,
														CString PrePosition,
														CString TodayPosition,
														CString SettlementPrice,
														CWnd* pParent /*=NULL*/)
	: CDialogEx(CClosePositionDlg::IDD, pParent)
	, m_strInstrument(_T(""))
	, m_strDirection(_T(""))
	, m_strVolume(_T(""))
	, m_Price(0)
	, m_strExchange(_T(""))
{
	m_strInstrument = InstrumentID;
	m_strDirection = Direction;
	m_strVolume = Position;//�˴����������ǳֲ�����
	this->SettlementPrice = SettlementPrice;
}

CClosePositionDlg::~CClosePositionDlg()
{
}

void CClosePositionDlg::OnReceiveData(WPARAM wParam, LPARAM lParam)
{
	if (wParam==ctp_msg::RtnMarketData)
	{
		MarketData* pMarketData = reinterpret_cast<MarketData*>(lParam);
		MarketData marketData = *pMarketData;
		CString InstrumentID_Dlg;
		GetDlgItem(EDIT_INSTRUMENT)->GetWindowText(InstrumentID_Dlg);
		CString InstrumentID(marketData.code);
		if (InstrumentID_Dlg == InstrumentID)
		{
			CString SpecORMarkPrice;
			((CComboBox*)GetDlgItem(COMBO_PRICETYPE))->GetWindowText(SpecORMarkPrice);
			if (SpecORMarkPrice == _T("���̱�����"))
			{
				CString CloseDirection;
				GetDlgItem(EDIT_DIRECTION)->GetWindowText(CloseDirection);
				if (CloseDirection==_T("��"))
				{//ƽ�ַ���ѡ��Ϊ��
					CString price;
					price.Format(_T("%.2f"), marketData.AskPrice);
					GetDlgItem(EDIT_PRICE)->SetWindowText(price);
				}
				else
				{//ƽ�ַ���ѡ��Ϊ��
					CString price;
					price.Format(_T("%.2f"), marketData.BidPrice);
					GetDlgItem(EDIT_PRICE)->SetWindowText(price);
				}
			}
		}
	}
}

void CClosePositionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, EDIT_INSTRUMENT, m_strInstrument);
	DDX_Text(pDX, EDIT_DIRECTION, m_strDirection);
	DDX_Text(pDX, EDIT_VOLUME, m_strVolume);
	DDX_Control(pDX, BUTTON_PRICETYPE, m_bnPriceType);
	DDX_Text(pDX, EDIT_PRICE, m_Price);
	DDX_CBString(pDX, COMBO_EXCHANGE, m_strExchange);
	DDX_Control(pDX, COMBO_EXCHANGE, m_cbExchange);
}


BEGIN_MESSAGE_MAP(CClosePositionDlg, CDialogEx)
	ON_BN_CLICKED(BUTTON_PRICETYPE, &CClosePositionDlg::OnBnClickedPricetype)
	ON_BN_CLICKED(BUTTON_CLOSE, &CClosePositionDlg::OnBnClickedClose)
	ON_EN_CHANGE(EDIT_VOLUME, &CClosePositionDlg::OnEnChangeVolume)
	ON_EN_CHANGE(EDIT_PRICE, &CClosePositionDlg::OnEnChangePrice)
	ON_EN_CHANGE(EDIT_TOUCHPRICE, &CClosePositionDlg::OnEnChangeTouchprice)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

BOOL CClosePositionDlg::OnInitDialog()
{
	get_CtpMsgDistributor()->AddReceiver(this);
	GetDlgItem(EDIT_INSTRUMENT)->SetWindowText(m_strInstrument); //����Լ����Ĭ��ֵ
	GetDlgItem(EDIT_PRICE)->SetWindowText(_T("0")); //��������������Ĭ��ֵ
	USES_CONVERSION;
	string opendirection = CT2A(m_strDirection.GetBuffer());
	if (opendirection == "��")
	{
		GetDlgItem(EDIT_DIRECTION)->SetWindowText(_T("��")); //���������Ĭ��ֵ
	}
	if (opendirection == "��")   //_T()����ʵ����C++�ַ�����cstring��ת��
	{
		GetDlgItem(EDIT_DIRECTION)->SetWindowText(_T("��")); //���������Ĭ��ֵ
	}
	GetDlgItem(EDIT_VOLUME)->SetWindowText(m_strVolume); //������������Ĭ��ֵ
	//Ĭ��ѡƽ�ְ�ť
	CButton* pCloseButton = reinterpret_cast<CButton*>(this->GetDlgItem(RADIO_CLOSE));
	pCloseButton->SetCheck(true);
	//��佻������ѡ��
	if (((CComboBox*)GetDlgItem(COMBO_EXCHANGE))->GetCount() != 0)
	{//����Ϊ�գ���ֹ�����ظ�����
		((CComboBox*)GetDlgItem(COMBO_EXCHANGE))->ResetContent();
	}
	((CComboBox*)GetDlgItem(COMBO_EXCHANGE))->AddString(_T("SHFE"));
	((CComboBox*)GetDlgItem(COMBO_EXCHANGE))->AddString(_T("DCE"));
	((CComboBox*)GetDlgItem(COMBO_EXCHANGE))->AddString(_T("CZCE"));
	((CComboBox*)GetDlgItem(COMBO_EXCHANGE))->AddString(_T("CFFEX"));
	((CComboBox*)GetDlgItem(COMBO_EXCHANGE))->AddString(_T("INE"));
	((CComboBox*)GetDlgItem(COMBO_EXCHANGE))->SetCurSel(0);//Ĭ��ѡ���һ��������
	//����޼����͸�ѡ��
	if (((CComboBox*)GetDlgItem(COMBO_PRICETYPE))->GetCount() != 0)
	{//����Ϊ�գ���ֹ�����ظ�����
		((CComboBox*)GetDlgItem(COMBO_PRICETYPE))->ResetContent();
	}
	((CComboBox*)GetDlgItem(COMBO_PRICETYPE))->AddString(_T("ָ��������"));
	((CComboBox*)GetDlgItem(COMBO_PRICETYPE))->AddString(_T("���̱�����"));
	((CComboBox*)GetDlgItem(COMBO_PRICETYPE))->SetCurSel(1);//Ĭ��ѡ���һ��ָ����
	//��ѡ���޼ۼ����̼ۣ���ײ�Ҫ���ݿ���������tick �۹���

	//ֹӯ��ѡ��Ĭ�ϲ�ѡ��
	((CButton*)GetDlgItem(CHECK_PROFITLOSS))->SetCheck(false);

	//ֹӯ��ť��Ĭ��ѡֹӯ
	CButton* pProfitORLossButton = reinterpret_cast<CButton*>(this->GetDlgItem(RADIO_PROFIT));
	pProfitORLossButton->SetCheck(true);
	pProfitORLossButton = reinterpret_cast<CButton*>(this->GetDlgItem(RADIO_LOSS));
	pProfitORLossButton->SetCheck(false);

	//Ĭ�ϼ۸���1
	CString cstr;
	cstr.Format(_T("%d"), 1);
	GetDlgItem(EDIT_TOUCHPRICE)->SetWindowText(cstr);

	return true;
}


// CClosePositionDlg message handlers


void CClosePositionDlg::OnBnClickedPricetype()
{
	CString priceTypeName;
	GetDlgItem(BUTTON_PRICETYPE)->GetWindowTextW(priceTypeName);
	if (priceTypeName == "�޼�")
	{//���ð�ť����ȥ������ǰΪ�޼ۣ����º��Ϊ�м�
		GetDlgItem(BUTTON_PRICETYPE)->SetWindowText(_T("�м�"));
	}
	else
	{
		GetDlgItem(BUTTON_PRICETYPE)->SetWindowText(_T("�޼�"));
	}
}


void CClosePositionDlg::OnBnClickedClose()
{
	//UpdateData(TRUE);	//���ؼ��������ˢ�µ��󶨵Ŀؼ�������,��仰����Ҫ,�����������
	USES_CONVERSION;
	InputOrderParam order_param;
	//����Լ
	order_param.InstrumentID = T2A(m_strInstrument);//T2A��mfc��cstring����ת��Ϊchar*����
	//��佻����
	order_param.ExchangeID = T2A(m_strExchange);//T2A��mfc��cstring����ת��Ϊchar*����
	//��䷽��
	string direction_stringtype = CT2A(m_strDirection.GetBuffer()); //��cstringת��Ϊstring
	if (direction_stringtype == "��")
	{
		order_param.Direction = THOST_FTDC_D_Buy;
	}
	if (direction_stringtype == "��")
	{
		order_param.Direction = THOST_FTDC_D_Sell;
	}
	//���ƽ������
	CButton* pCloseTodayButton = reinterpret_cast<CButton*>(this->GetDlgItem(RADIO_CLOSETODAY));
	if (pCloseTodayButton->GetCheck() == BST_CHECKED)
	{
		order_param.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday; //ƽ��
	}
	CButton* pCloseButton = reinterpret_cast<CButton*>(this->GetDlgItem(RADIO_CLOSE));
	if (pCloseButton->GetCheck() == BST_CHECKED)
	{
		order_param.CombOffsetFlag[0] = THOST_FTDC_OF_Close; //ƽ��
	}
	//�������
	GetDlgItem(EDIT_VOLUME)->GetWindowText(m_strVolume);
	order_param.VolumeTotalOriginal = _ttoi(m_strVolume);//���Խ�Cstring����ת��Ϊint����

	//���۸�����
	order_param.OrderPriceType = THOST_FTDC_OPT_LimitPrice; //Ĭ�����޼ۣ���ť��Ĭ��ֵҲ���޼�
	order_param.ContingentCondition = THOST_FTDC_CC_Immediately; //�޼۵����м۵���Ĭ�ϴ������Ͷ��������������������������޼۵��������м۵�
	CString priceTypeName;
	GetDlgItem(BUTTON_PRICETYPE)->GetWindowText(priceTypeName); //��ȡ�۸����Ͱ�ť�ı�ǩֵ
	if (priceTypeName == "�м�")
	{
		order_param.OrderPriceType = THOST_FTDC_OPT_AnyPrice;  //Ϊ�����м��µ�
	}
	//��¼�۸�
	CString text;
	GetDlgItemText(EDIT_PRICE, text);
	string price = CT2A(text.GetBuffer());
	order_param.LimitPrice = stod(price);               //��䱨���۸�

	if (BST_CHECKED == IsDlgButtonChecked(CHECK_PROFITLOSS)) //��ֹӯ���������Ƿ�ѡ��
	{
		double settlement_price = _ttof(this->SettlementPrice);//ʵ�ִ�CString��double��ת��

		CString touchPrice_cstr;
		GetDlgItem(EDIT_TOUCHPRICE)->GetWindowText(touchPrice_cstr);
		double touchPrice = _ttof(touchPrice_cstr);

		string opendirection = CT2A(m_strDirection.GetBuffer());
		CButton* pProfitORLossButton = reinterpret_cast<CButton*>(this->GetDlgItem(RADIO_PROFIT));
		if (pProfitORLossButton->GetCheck() == BST_CHECKED)
		{//ѡ��ֹӯ�Ĵ���
			if (opendirection == "��")
			{//���ֹӯ���������۸������ڵ����ּ�(�ñ��ν���۴���)
				if ( !(touchPrice >= settlement_price) ) //�쳣�����۱���
				{
					::MessageBox(NULL, _T("�൥ֹӯ��������Ӧ���ڵ����ּ�"), _T("����"), MB_OK);//������
					return;//�����ֱ��return
				}
				else
				{//��Ϊ��ȷ��������BuyStop���͵Ķ൥ֹӯ
					order_param.ContingentCondition = THOST_FTDC_CC_LastPriceGreaterEqualStopPrice;
					order_param.StopPrice = touchPrice;
				}
			}
			else if (opendirection == "��")
			{//�ղ�ֹӯ���������۸����С�ڵ����ּ�(�ñ��ν���۴���)
				if (!(touchPrice <= settlement_price)) //�쳣�����۱���
				{
					::MessageBox(NULL, _T("�յ�ֹӯ��������ӦС�ڵ����ּ�"), _T("����"), MB_OK);//������
					return;//�����ֱ��return
				}
				else
				{//��Ϊ��ȷ��������SellStop���͵Ŀյ�ֹӯ
					order_param.ContingentCondition = THOST_FTDC_CC_LastPriceLesserEqualStopPrice;
					order_param.StopPrice = touchPrice;
				}
			}
			else
			{

			}
		}
		pProfitORLossButton = reinterpret_cast<CButton*>(this->GetDlgItem(RADIO_LOSS));
		if (pProfitORLossButton->GetCheck() == BST_CHECKED)
		{//ѡ��ֹ��Ĵ���
			if (opendirection == "��")
			{//���ֹ�𵥣������۸����С�ڵ����ּ�(�ñ��ν���۴���)
				if (!(touchPrice <= settlement_price)) //�쳣�����۱���
				{
					::MessageBox(NULL, _T("�൥ֹ�𣺴�����ӦС�ڵ����ּ�"), _T("����"), MB_OK);//������
					return;//�����ֱ��return
				}
				else
				{//�൥ֹ��SellStop��
					order_param.ContingentCondition = THOST_FTDC_CC_LastPriceLesserEqualStopPrice;
					order_param.StopPrice = touchPrice;
				}
			}
			else if (opendirection == "��")
			{//�ղ�ֹ�𵥣������۸������ڵ����ּ�(�ñ��ν���۴���)
				if (!(touchPrice >= settlement_price)) //�쳣�����۱���
				{
					::MessageBox(NULL, _T("�յ�ֹ�𣺴�����Ӧ���ڵ����ּ�"), _T("����"), MB_OK);//������
					return;//�����ֱ��return
				}
				else
				{//�յ�ֹ��BuyStop��
					order_param.ContingentCondition = THOST_FTDC_CC_LastPriceGreaterEqualStopPrice;
					order_param.StopPrice = touchPrice;
				}
			}
			else
			{

			}
		}
		order_param.TimeCondition = THOST_FTDC_TC_GFD;//������Ч,CTP��������Ŀǰ��֧�ֵ�����Ч�����ֶ��������ر��ֶδ��󣬲�Ҫ���Ը��ֶ�
	}

	if (::MessageBox(NULL, _T("�Ƿ�ȷ��ִ��"), _T("ȷ��"), MB_OKCANCEL) == IDOK)
	{
		get_global_mgr().GetService()->request(ctp_msg::MakeOrder, &order_param);
		PostMessage(WM_CLOSE, NULL, NULL);
	}
}


void CClosePositionDlg::OnEnChangeVolume()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
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
	if (strTemp == _T("0"))
	{
		editHelp->SetWindowText(_T("1"));
	}
}


void CClosePositionDlg::OnEnChangePrice()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString strTemp = _T("");
	CEdit* editHelp = ((CEdit*)(GetDlgItem(EDIT_PRICE)));
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


void CClosePositionDlg::OnEnChangeTouchprice()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString strTemp = _T("");
	CEdit* editHelp = ((CEdit*)(GetDlgItem(EDIT_TOUCHPRICE)));
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


void CClosePositionDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	get_CtpMsgDistributor()->DelReceiver(this);
	__super::OnClose();
}
