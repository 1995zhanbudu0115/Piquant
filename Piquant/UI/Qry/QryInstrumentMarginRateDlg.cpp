// QryInstrumentMarginRateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../App.h"
#include "QryInstrumentMarginRateDlg.h"
#include "afxdialogex.h"
#include "../GlobalMgr.h"
#include "../../ServiceLayer/DataTypes.h"
#include "../../ServiceLayer/CtpService.h"
#include "../../ServiceLayer/CtpMsgDistributor.h"
#include "../../../sdk/ThostFtdcUserApiDataType.h"
#include "../../ServiceLayer/CtpMsg.h"



// CQryInstrumentMarginRateDlg dialog

IMPLEMENT_DYNAMIC(CQryInstrumentMarginRateDlg, CDialogEx)

CQryInstrumentMarginRateDlg::CQryInstrumentMarginRateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CQryInstrumentMarginRateDlg::IDD, pParent)
{

}

CQryInstrumentMarginRateDlg::~CQryInstrumentMarginRateDlg()
{
}

void CQryInstrumentMarginRateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CQryInstrumentMarginRateDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(BUTTON_QRYMARGINRATE, &CQryInstrumentMarginRateDlg::OnBnClickedQrymarginrate)
END_MESSAGE_MAP()

BOOL CQryInstrumentMarginRateDlg::OnInitDialog()
{
	get_global_mgr().SetStopTimeQry();
	get_CtpMsgDistributor()->AddReceiver(this);//��CQryInstrumentMarginRateDlg��ʵ����ӽ�������������
	//����Ψһ��get_global_mgr�ⲿ�ӿ�ȥ��ȡctpservice����, ������ȡ����api��������api����, ��������ȡ��Ӧ����Ϣ
	vector<BaseData> instruments;
	if (NULL != get_global_mgr().GetService())
	{
		get_global_mgr().GetService()->get_basic_data(instruments);
		if (!instruments.empty())
		{//������, ��������䵽��Ͽ���ȥ
			if (((CComboBox*)GetDlgItem(COMBO_INSTRUMENT))->GetCount() != 0)
			{//��ֹ���μ���,Ϊ���ټ���ǰ����Ϊ��,�����������
				((CComboBox*)GetDlgItem(COMBO_INSTRUMENT))->ResetContent();
			}
			for (int i = 0; i < instruments.size(); ++i)
			{
				((CComboBox*)GetDlgItem(COMBO_INSTRUMENT))->AddString(CString(instruments[i].code));
			}
		}
	}

	if (((CComboBox*)GetDlgItem(COMBO_HEDGEFLAG))->GetCount() != 0)
	{//��ֹ���μ���,Ϊ���ټ���ǰ����Ϊ��,�����������
		((CComboBox*)GetDlgItem(COMBO_HEDGEFLAG))->ResetContent();
	}
	((CComboBox*)GetDlgItem(COMBO_HEDGEFLAG))->AddString(_T("Ͷ��"));
	((CComboBox*)GetDlgItem(COMBO_HEDGEFLAG))->AddString(_T("����"));
	((CComboBox*)GetDlgItem(COMBO_HEDGEFLAG))->AddString(_T("�ױ�"));
	((CComboBox*)GetDlgItem(COMBO_HEDGEFLAG))->AddString(_T("������"));
	return true;
}


// CQryInstrumentMarginRateDlg message handlers


void CQryInstrumentMarginRateDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	get_CtpMsgDistributor()->DelReceiver(this);//�����ٶԻ���֮ǰ,Ӧ�Ƚ������Ϣ��������receivers���޳�����ֹ�����ٽ���Ϣ���ݹ��������ǸöԻ����ѱ�����
	get_global_mgr().SetStartTimeQry();
	CDialogEx::OnClose();
}

void CQryInstrumentMarginRateDlg::OnReceiveData(WPARAM wParam, LPARAM lParam)
{
	if (wParam==ctp_msg::RspQryMarginRate)
	{
		MarginRate* margin_rate = reinterpret_cast<MarginRate*>(lParam);
		CString LongMarginRatioByMoney;
		LongMarginRatioByMoney.Format(_T("%0.6lf"), margin_rate->LongMarginRatioByMoney);
		GetDlgItem(EDIT_LongMarginRatioByMoney)->SetWindowText(LongMarginRatioByMoney);
		CString LongMarginRatioByVolume;
		LongMarginRatioByVolume.Format(_T("%0.2lf"), margin_rate->LongMarginRatioByVolume);
		GetDlgItem(EDIT_LongMarginRatioByVolume)->SetWindowText(LongMarginRatioByVolume);
		CString ShortMarginRatioByMoney;
		ShortMarginRatioByMoney.Format(_T("%0.6lf"), margin_rate->ShortMarginRatioByMoney);
		GetDlgItem(EDIT_ShortMarginRatioByMoney)->SetWindowText(ShortMarginRatioByMoney);
		CString ShortMarginRatioByVolume;
		ShortMarginRatioByVolume.Format(_T("%0.2lf"), margin_rate->ShortMarginRatioByVolume);
		GetDlgItem(EDIT_ShortMarginRatioByVolume)->SetWindowText(ShortMarginRatioByVolume);
	}
}


void CQryInstrumentMarginRateDlg::OnBnClickedQrymarginrate()
{
	QryInstrumentMarginRateParam marginrate_param;
	USES_CONVERSION;
	CString instrument_cstr;
	((CComboBox*)GetDlgItem(COMBO_INSTRUMENT))->GetWindowText(instrument_cstr);
	CString hedgeflag_cstr;
	((CComboBox*)GetDlgItem(COMBO_HEDGEFLAG))->GetWindowText(hedgeflag_cstr);
	marginrate_param.InstrumentID = T2A(instrument_cstr); //T2A��mfc��cstring����ת��Ϊchar*����
	if (hedgeflag_cstr == _T("Ͷ��"))
	{
		marginrate_param.HedgeFlag = THOST_FTDC_HF_Speculation;
	}
	else if (hedgeflag_cstr == _T("����"))
	{
		marginrate_param.HedgeFlag = THOST_FTDC_HF_Arbitrage;
	}
	else if (hedgeflag_cstr == _T("�ױ�"))
	{
		marginrate_param.HedgeFlag = THOST_FTDC_HF_Hedge;
	}
	else
	{
		marginrate_param.HedgeFlag = THOST_FTDC_HF_MarketMaker;
	}
	marginrate_param.BrokerID="";
	marginrate_param.InvestorID="";
	if (::MessageBox(NULL, _T("�Ƿ�ȷ�ϲ�ѯ"), _T("ȷ��"), MB_OKCANCEL) == IDOK)
	{
		get_global_mgr().GetService()->request(ctp_msg::QryMarginRate, &marginrate_param);
		//PostMessage(WM_CLOSE, NULL, NULL);
	}
}
