// GridCtrlParkedOrderList.cpp : implementation file
//

#include "stdafx.h"
#include "../App.h"
#include "GridCtrlParkedOrderList.h"
#include "../MakeOrder/RemoveParkedOrderDlg.h"


// CGridCtrlParkedOrderList

IMPLEMENT_DYNAMIC(CGridCtrlParkedOrderList, CCustomGridCtrl)

CGridCtrlParkedOrderList::CGridCtrlParkedOrderList()
{

}

CGridCtrlParkedOrderList::~CGridCtrlParkedOrderList()
{
}


void CGridCtrlParkedOrderList::OnFixedColumnClick(CCellID& cell)
{
	int row = cell.row;
	CString InstrumentID = this->GetItemText(row, 0);//��ȡ��ԼID��ȷ����0��һ���Ǻ�ԼID
	CString ParkOrderID = this->GetItemText(row, 11);//��ȡԤ��ID��ȷ����11��һ����Ԥ��ID
	if (this->GetItemText(row, 9) == _T("δ����"))
	{//ֻ��δ����״̬�²ſ���Ԥ�񳷵�
		CRemoveParkedOrderDlg removeparkedorder_dlg(InstrumentID, ParkOrderID);
		removeparkedorder_dlg.DoModal();
	}
	else
	{
		//������
	}
	
}

BEGIN_MESSAGE_MAP(CGridCtrlParkedOrderList, CCustomGridCtrl)
END_MESSAGE_MAP()



// CGridCtrlParkedOrderList message handlers


