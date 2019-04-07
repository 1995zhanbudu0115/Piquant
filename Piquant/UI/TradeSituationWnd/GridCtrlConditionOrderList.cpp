// GridCtrlConditionOrderList.cpp : implementation file
//

#include "stdafx.h"
#include "../App.h"
#include "GridCtrlConditionOrderList.h"
#include "../MakeOrder/CancelOrderDlg.h"

// CGridCtrlConditionOrderList

IMPLEMENT_DYNAMIC(CGridCtrlConditionOrderList, CCustomGridCtrl)

CGridCtrlConditionOrderList::CGridCtrlConditionOrderList()
{

}

CGridCtrlConditionOrderList::~CGridCtrlConditionOrderList()
{
}


void CGridCtrlConditionOrderList::OnFixedColumnClick(CCellID& cell)
{
	int row = cell.row;
	CString OrderSysID = this->GetItemText(row, 16);//��ȡ������ţ�ȷ����16��һ���Ǳ������
	CString InstrumentID = this->GetItemText(row, 0);//��ȡ��ԼID��ȷ����0��һ���Ǻ�ԼID
	CString Direction = this->GetItemText(row, 2);//��ȡ����ȷ����2��һ���Ƿ���
	CString OpenClose = this->GetItemText(row, 3);//��ȡ��ƽ��ȷ����3��һ���ǿ�ƽ����
	CString Price = this->GetItemText(row, 5);//��ȡ�۸�ȷ����5��һ���Ǳ����۸�
	CString insertVolume = this->GetItemText(row, 6);//��ȡ��������ȷ����6��һ���Ǳ�����
	CString ExchangeID = this->GetItemText(row, 15);//��ȡ������ID��ȷ����15��һ���ǽ�����ID
	if (this->GetItemText(row, 9) == _T("��δ����"))
	{
		CCancelOrderDlg cancelorder_dlg(OrderSysID, InstrumentID, Direction, OpenClose, Price, insertVolume, ExchangeID);
		cancelorder_dlg.DoModal();
	}
	else
	{
		//������
	}
}

BEGIN_MESSAGE_MAP(CGridCtrlConditionOrderList, CCustomGridCtrl)
END_MESSAGE_MAP()



// CGridCtrlConditionOrderList message handlers


