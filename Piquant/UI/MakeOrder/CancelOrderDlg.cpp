// CancelOrderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../App.h"
#include "CancelOrderDlg.h"
#include "afxdialogex.h"
#include "../resource.h"
#include "atlconv.h" //ʹ��USES_CONVERSION��Ҫ�õ���ͷ�ļ�
#include "../../ServiceLayer/DataTypes.h"
#include "../../../sdk/ThostFtdcUserApiDataType.h"
#include "../GlobalMgr.h"
#include "../../ServiceLayer/CtpService.h"
#include "../../ServiceLayer/CtpMsg.h"


// CCancelOrderDlg dialog

IMPLEMENT_DYNAMIC(CCancelOrderDlg, CDialogEx)

CCancelOrderDlg::CCancelOrderDlg(CString OrderSysID,
														CString InstrumentID,
														CString Direction,
														CString OpenClose,
														CString Price,
														CString insertVolume,
														CString ExchangeID,
														CWnd* pParent /*=NULL*/)
	: CDialogEx(CCancelOrderDlg::IDD, pParent)
	, m_strOrderSysID(_T(""))
	, m_strInstrumentID(_T(""))
	, m_strDirection(_T(""))
	, m_strOpenClose(_T(""))
	, m_strVolume(_T(""))
	, m_strPrice(_T(""))
{
	m_strOrderSysID = OrderSysID;
	m_strInstrumentID = InstrumentID;
	m_strDirection = Direction;
	m_strOpenClose = OpenClose;
	m_strVolume = insertVolume;
	m_strPrice = Price;
	exchangeID = ExchangeID;
}

CCancelOrderDlg::~CCancelOrderDlg()
{
}

void CCancelOrderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, STATIC_ORDERSYSIDVALUE, m_strOrderSysID);
	DDX_Text(pDX, STATIC_INSTRUMENTVALUE, m_strInstrumentID);
	DDX_Text(pDX, STATIC_DIRECTIONVALUE, m_strDirection);
	DDX_Text(pDX, STATIC_OPENCLOSEVALUE, m_strOpenClose);
	DDX_Text(pDX, STATIC_VOLUMVALUE, m_strVolume);
	DDX_Text(pDX, STATIC_PRICEVALUE, m_strPrice);
}


BEGIN_MESSAGE_MAP(CCancelOrderDlg, CDialogEx)
	ON_BN_CLICKED(BUTTON_CANCELORDER, &CCancelOrderDlg::OnBnClickedCancelorder)
	ON_BN_CLICKED(MFCBUTTON_BATCHORDERACTION, &CCancelOrderDlg::OnBnClickedBatchorderaction)
	ON_BN_CLICKED(BUTTON_CANCELPARKEDORDER, &CCancelOrderDlg::OnBnClickedCancelparkedorder)
END_MESSAGE_MAP()

BOOL CCancelOrderDlg::OnInitDialog()
{
	GetDlgItem(STATIC_ORDERSYSIDVALUE)->SetWindowText(m_strOrderSysID);
	GetDlgItem(STATIC_INSTRUMENTVALUE)->SetWindowText(m_strInstrumentID);
	GetDlgItem(STATIC_DIRECTIONVALUE)->SetWindowText(m_strDirection);
	GetDlgItem(STATIC_OPENCLOSEVALUE)->SetWindowText(m_strOpenClose);
	GetDlgItem(STATIC_VOLUMVALUE)->SetWindowText(m_strVolume);
	GetDlgItem(STATIC_PRICEVALUE)->SetWindowText(m_strPrice);
	return TRUE;
}


// CCancelOrderDlg message handlers


void CCancelOrderDlg::OnBnClickedCancelorder()
{
	USES_CONVERSION;
	ActionOrderParam order_param = {0};
	//����Լ
	strcpy(order_param.InstrumentID,T2A(m_strInstrumentID));//T2A��mfc��cstring����ת��Ϊchar*����
	//��佻����
	strcpy(order_param.ExchangeID, T2A(exchangeID));//T2A��mfc��cstring����ת��Ϊchar*����
	//��䱨�����
	strcpy(order_param.OrderSysID, T2A(m_strOrderSysID));//T2A��mfc��cstring����ת��Ϊchar*����
	if (::MessageBox(NULL, _T("�Ƿ�ȷ�ϳ���"), _T("ȷ��"), MB_OKCANCEL) == IDOK)
	{
		get_global_mgr().GetService()->request(ctp_msg::CancelOrder, &order_param);
		PostMessage(WM_CLOSE, NULL, NULL);
	}
}


void CCancelOrderDlg::OnBnClickedBatchorderaction()
{
	BatchOrderActionParam order_param;
	if (::MessageBox(NULL, _T("�Ƿ�ȫ��"), _T("ȷ��"), MB_OKCANCEL) == IDOK)
	{
		get_global_mgr().GetService()->request(ctp_msg::BatchOrderAction, &order_param);
		PostMessage(WM_CLOSE, NULL, NULL);
	}
}


void CCancelOrderDlg::OnBnClickedCancelparkedorder()
{
	// TODO: Add your control notification handler code here
	USES_CONVERSION;
	ParkedOrderActionParam order_param;
	memset(&order_param, 0, sizeof(order_param));
	//����Լ
	order_param.InstrumentID =T2A(m_strInstrumentID);//T2A��mfc��cstring����ת��Ϊchar*����
	//��佻����
	order_param.ExchangeID = T2A(exchangeID);//T2A��mfc��cstring����ת��Ϊchar*����
	//��䱨�����
	order_param.OrderSysID = T2A(m_strOrderSysID);//T2A��mfc��cstring����ת��Ϊchar*����
	if (::MessageBox(NULL, _T("�Ƿ�ȷ��Ԥ�񳷵�"), _T("ȷ��"), MB_OKCANCEL) == IDOK)
	{
		get_global_mgr().GetService()->request(ctp_msg::Cancel_ParkedOrder, &order_param);
		PostMessage(WM_CLOSE, NULL, NULL);
	}
}
