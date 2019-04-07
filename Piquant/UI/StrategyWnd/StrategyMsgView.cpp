// StrategyMsgView.cpp : implementation file
//

#include "stdafx.h"
#include "../App.h"
#include "StrategyMsgView.h"


// CStrategyMsgView

IMPLEMENT_DYNCREATE(CStrategyMsgView, CFormView)

CStrategyMsgView::CStrategyMsgView()
	: CFormView(CStrategyMsgView::IDD)
{

}

CStrategyMsgView::~CStrategyMsgView()
{
}

void CStrategyMsgView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CStrategyMsgView, CFormView)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CStrategyMsgView diagnostics

#ifdef _DEBUG
void CStrategyMsgView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CStrategyMsgView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

void CStrategyMsgView::OnInitialUpdate()
{
	get_strategymsg_distributor()->AddReceiver(this); //���Ϊ������Ϣ�ַ����Ĺ۲���

	((CListCtrl*)GetDlgItem(LIST_STRATEGYMSG))->SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	((CListCtrl*)GetDlgItem(LIST_STRATEGYMSG))->InsertColumn(0, _T("������"), NULL, 60, -1);
	((CListCtrl*)GetDlgItem(LIST_STRATEGYMSG))->InsertColumn(1, _T("��־ʱ��"), LVCFMT_CENTER, 60, -1);
	((CListCtrl*)GetDlgItem(LIST_STRATEGYMSG))->InsertColumn(2, _T("��־����"), NULL, 60, -1);
	((CListCtrl*)GetDlgItem(LIST_STRATEGYMSG))->InsertColumn(3, _T("��־����"), NULL, 200, -1);
}

void CStrategyMsgView::OnReceiveData(WPARAM wParam, LPARAM lParam)
{
	if (wParam == WM_UPDATESTRATEGYMSG)
	{
		StrategyMsg* pStrategyMsg = reinterpret_cast<StrategyMsg*>(lParam);
		StrategyMsg msg = *pStrategyMsg;
		CString strategyName((msg.strategyName).c_str());
		if (strategyName == _T(""))
		{//�����쳣
			return;
		}
		//����һ�У���ͷΪ������
		int rowcount = ((CListCtrl*)(this->GetDlgItem(LIST_STRATEGYMSG)))->GetItemCount(); //��ȡ����
		((CListCtrl*)(this->GetDlgItem(LIST_STRATEGYMSG)))->InsertItem(rowcount, strategyName); //����һ��
		//�����Ϣ�ı���ʱ��
		time_t nowtime; 
		nowtime = time(NULL); //��ȡ����ʱ��
		struct tm* local;
		local = localtime(&nowtime);
		char loggingTime[100];
		sprintf_s(loggingTime, "%s", asctime(local)); //���������ʱ�������
		msg.loggingTime = loggingTime;
		//����Ϊ����ˢ�²���
		if (msg.loggingTime != "")
		{
			CString loggingTime(msg.loggingTime.c_str());
			((CListCtrl*)(this->GetDlgItem(LIST_STRATEGYMSG)))->SetItemText(rowcount, 1, loggingTime);
		}
		if (msg.loggingLevel != 0)
		{
			CString loggingLevel;
			if (msg.loggingLevel == UEGENT)
			{
				loggingLevel = _T("����");
			}
			else if (msg.loggingLevel==IMPORTANT)
			{
				loggingLevel = _T("��Ҫ");
			}
			else
			{
				loggingLevel = _T("һ��");
			}
			((CListCtrl*)(this->GetDlgItem(LIST_STRATEGYMSG)))->SetItemText(rowcount, 2, loggingLevel);
		}
		if (msg.loggingContent != "")
		{
			CString loggingContent(msg.loggingContent.c_str());
			((CListCtrl*)(this->GetDlgItem(LIST_STRATEGYMSG)))->SetItemText(rowcount, 3, loggingContent);
		}
	}
}

#endif
#endif //_DEBUG


// CStrategyMsgView message handlers


void CStrategyMsgView::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	get_strategymsg_distributor()->DelReceiver(this); //�ӹ۲����б����޳�
	__super::OnClose();
}
