// GridCtrlOrderList.cpp : implementation file
//

#include "stdafx.h"
#include "../App.h"
#include "GridCtrlOrderList.h"


// CGridCtrlOrderList

IMPLEMENT_DYNAMIC(CGridCtrlOrderList, CCustomGridCtrl)

CGridCtrlOrderList::CGridCtrlOrderList()
{

}

CGridCtrlOrderList::~CGridCtrlOrderList()
{
}


BEGIN_MESSAGE_MAP(CGridCtrlOrderList, CCustomGridCtrl)
END_MESSAGE_MAP()

void CGridCtrlOrderList::OnFixedColumnClick(CCellID& cell)
{
	//::MessageBox(NULL, _T("����"), _T("����"), MB_OK);//������
	//�����������������Ե��ú��ʵĵ������
	//CLoginDlg login_dlg;
	//login_dlg.DoModal();//�˴�������ʾ��¼�˵�ѡ��Ľ���
}



// CGridCtrlOrderList message handlers


