// MakeOrderBoardPane.cpp : implementation file
//

#include "stdafx.h"
#include "../App.h"
#include "MakeOrderBoardPane.h"


// CMakeOrderBoardPane

IMPLEMENT_DYNAMIC(CMakeOrderBoardPane, CDockablePane)

CMakeOrderBoardPane::CMakeOrderBoardPane()
:m_wndSMOBoard(NULL)
{//�ǵ��ڹ��캯���и�ָ���ʼ��Ϊnull

}

CMakeOrderBoardPane::~CMakeOrderBoardPane()
{
}

//����Ϣmap�������Ϣ��
BEGIN_MESSAGE_MAP(CMakeOrderBoardPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CMakeOrderBoardPane message handlers

int CMakeOrderBoardPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	m_wndSMOBoard = new CStandardMakeOrderBoard(this);  //����CStandardMakeOrderBoard��Ĺ��캯���������µ���
	return 0;
}


void CMakeOrderBoardPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if (GetSafeHwnd() == NULL || (AfxGetMainWnd() != NULL && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}
	CRect rectClient;
	GetClientRect(rectClient); //�˴����ڻ�ȡ���ڿͻ����Ĵ�С��������Ӧ�Ĵ�С��������rectClient����ṹ����ȥ
	m_wndSMOBoard->SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}

