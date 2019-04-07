// RemoveParkedOrderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../App.h"
#include "RemoveParkedOrderDlg.h"
#include "afxdialogex.h"
#include "../../ServiceLayer/DataTypes.h"
#include "../../ServiceLayer/CtpService.h"
#include "../GlobalMgr.h"
#include "../../ServiceLayer/CtpMsg.h"

// CRemoveParkedOrderDlg dialog

IMPLEMENT_DYNAMIC(CRemoveParkedOrderDlg, CDialogEx)

CRemoveParkedOrderDlg::CRemoveParkedOrderDlg(CString InstrumentID, CString ParkOrderID, CWnd* pParent /*=NULL*/)
	: CDialogEx(CRemoveParkedOrderDlg::IDD, pParent)
{
	this->InstrumentID = InstrumentID;
	this->ParkOrderID = ParkOrderID;
}

CRemoveParkedOrderDlg::~CRemoveParkedOrderDlg()
{
}

BOOL CRemoveParkedOrderDlg::OnInitDialog()
{
	CString dispText = this->InstrumentID + _T("(") + this->ParkOrderID + _T(")") + _T(":  ") + _T("Ԥ�񳷵�");
	GetDlgItem(STATIC_INSTRUMENT)->SetWindowText(dispText);
	return true;
}

void CRemoveParkedOrderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRemoveParkedOrderDlg, CDialogEx)
	ON_BN_CLICKED(MFCBUTTON_REMOVEPARKEDORDER, &CRemoveParkedOrderDlg::OnBnClickedRemoveparkedorder)
END_MESSAGE_MAP()


// CRemoveParkedOrderDlg message handlers


void CRemoveParkedOrderDlg::OnBnClickedRemoveparkedorder()
{
	USES_CONVERSION;
	RemoveParkedOrderParam order_param;
	//��䱨�����
	order_param.ParkedOrderID = T2A(this->ParkOrderID);//T2A��mfc��cstring����ת��Ϊchar*����
	if (::MessageBox(NULL, _T("�Ƿ�ȷ��Ԥ�񳷵�"), _T("ȷ��"), MB_OKCANCEL) == IDOK)
	{
		get_global_mgr().GetService()->request(ctp_msg::RemoveParkedOrder, &order_param);
		PostMessage(WM_CLOSE, NULL, NULL);
	}
}
