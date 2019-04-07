// GridCtrlPositionList.cpp : implementation file
//

#include "stdafx.h"
#include "../App.h"
#include "GridCtrlPositionList.h"
#include "../MakeOrder/ClosePositionDlg.h"


// CGridCtrlPositionList

IMPLEMENT_DYNAMIC(CGridCtrlPositionList, CCustomGridCtrl)

CGridCtrlPositionList::CGridCtrlPositionList()
{

}

CGridCtrlPositionList::~CGridCtrlPositionList()
{
}


BEGIN_MESSAGE_MAP(CGridCtrlPositionList, CCustomGridCtrl)
END_MESSAGE_MAP()

void CGridCtrlPositionList::OnFixedColumnClick(CCellID& cell)
{
	int row = cell.row;
	CString InstrumentID = this->GetItemText(row,  0);//��ȡ��ԼID��ȷ����һ��һ���Ǻ�ԼID
	CString Direction = this->GetItemText(row, 2);//��ȡ����ȷ����2��һ���Ƿ���
	CString Position = this->GetItemText(row, 6);//��ȡ�ֲ֣ܳ�ȷ����6��һ�����ֲܳ�
	CString PrePosition = this->GetItemText(row, 7);//��ȡ��ֲ֣�ȷ����7��һ������ֲ�
	CString TodayPosition = this->GetItemText(row, 8);//��ȡ��ֲ֣�ȷ����8��һ���ǽ�ֲ�
	CString SettlementPrice = this->GetItemText(row, 10);//��ȡ����ۣ�ȷ����10��һ���ǽ����
	CClosePositionDlg PositionDlg(InstrumentID, Direction, Position, PrePosition, TodayPosition, SettlementPrice);
	PositionDlg.DoModal();
}



// CGridCtrlPositionList message handlers


