// GridCtrlUnfilledOrderList.cpp : implementation file
//

#include "stdafx.h"
#include "../App.h"
#include "GridCtrlUnfilledOrderList.h"
#include "../MakeOrder/CancelOrderDlg.h"


// CGridCtrlUnfilledOrderList

IMPLEMENT_DYNAMIC(CGridCtrlUnfilledOrderList, CCustomGridCtrl)

CGridCtrlUnfilledOrderList::CGridCtrlUnfilledOrderList()
{

}

CGridCtrlUnfilledOrderList::~CGridCtrlUnfilledOrderList()
{
}


BEGIN_MESSAGE_MAP(CGridCtrlUnfilledOrderList, CCustomGridCtrl)
END_MESSAGE_MAP()

void CGridCtrlUnfilledOrderList::OnFixedColumnClick(CCellID& cell)
{
	int row = cell.row;
	CString OrderSysID = this->GetItemText(row, 15);//��ȡ������ţ�ȷ����16��һ���Ǳ������
	CString InstrumentID = this->GetItemText(row, 0);//��ȡ��ԼID��ȷ����0��һ���Ǻ�ԼID
	CString Direction = this->GetItemText(row, 1);//��ȡ����ȷ����1��һ���Ƿ���
	CString OpenClose = this->GetItemText(row, 2);//��ȡ��ƽ��ȷ����2��һ���ǿ�ƽ����
	CString Price = this->GetItemText(row, 4);//��ȡ�۸�ȷ����4��һ���Ǳ����۸�
	CString insertVolume = this->GetItemText(row, 5);//��ȡ��������ȷ����5��һ���Ǳ�����
	CString ExchangeID = this->GetItemText(row, 14);//��ȡ������ID��ȷ����15��һ���ǽ�����ID
	CCancelOrderDlg cancelorder_dlg(OrderSysID, InstrumentID, Direction, OpenClose, Price, insertVolume, ExchangeID);
	cancelorder_dlg.DoModal();

}



// CGridCtrlUnfilledOrderList message handlers


