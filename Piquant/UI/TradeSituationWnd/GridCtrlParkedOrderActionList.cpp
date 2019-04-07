// GridCtrlParkedOrderActionList.cpp : implementation file
//

#include "stdafx.h"
#include "../App.h"
#include "GridCtrlParkedOrderActionList.h"
#include "../MakeOrder/RemoveParkedOrderActionDlg.h"


// CGridCtrlParkedOrderActionList

IMPLEMENT_DYNAMIC(CGridCtrlParkedOrderActionList, CCustomGridCtrl)

CGridCtrlParkedOrderActionList::CGridCtrlParkedOrderActionList()
{

}

CGridCtrlParkedOrderActionList::~CGridCtrlParkedOrderActionList()
{
}


void CGridCtrlParkedOrderActionList::OnFixedColumnClick(CCellID& cell)
{
	int row = cell.row;
	CString InstrumentID = this->GetItemText(row, 0);//��ȡ��ԼID��ȷ����0��һ���Ǻ�ԼID
	CString ParkOrderActionID = this->GetItemText(row, 7);//��ȡԤ��ID��ȷ����7��һ����Ԥ�񳷵�ID
	if (this->GetItemText(row, 4) == _T("δ����"))
	{//ֻ��δ����״̬�²ſ���Ԥ�񳷵�
		CRemoveParkedOrderActionDlg removeparkedorderaction_dlg(InstrumentID, ParkOrderActionID);
		removeparkedorderaction_dlg.DoModal();
	}
	else
	{
		//������
	}
}

BEGIN_MESSAGE_MAP(CGridCtrlParkedOrderActionList, CCustomGridCtrl)
END_MESSAGE_MAP()



// CGridCtrlParkedOrderActionList message handlers


