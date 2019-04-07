// StrategyStatusView.cpp : implementation file
//

#include "stdafx.h"
#include "../App.h"
#include "StrategyStatusView.h"


// CStrategyStatusView

IMPLEMENT_DYNCREATE(CStrategyStatusView, CFormView)

CStrategyStatusView::CStrategyStatusView()
	: CFormView(CStrategyStatusView::IDD)
{

}

CStrategyStatusView::~CStrategyStatusView()
{
}

void CStrategyStatusView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CStrategyStatusView, CFormView)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CStrategyStatusView diagnostics

#ifdef _DEBUG
void CStrategyStatusView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CStrategyStatusView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

void CStrategyStatusView::OnInitialUpdate()
{
	get_strategymsg_distributor()->AddReceiver(this); //���Ϊ������Ϣ�ַ����Ĺ۲���

	((CListCtrl*)GetDlgItem(LIST_STRATEGYSTATUS))->SetExtendedStyle(LVS_EX_GRIDLINES  | LVS_EX_FULLROWSELECT);
	((CListCtrl*)GetDlgItem(LIST_STRATEGYSTATUS))->InsertColumn(0, _T("��������"), NULL, 60, -1);
	((CListCtrl*)GetDlgItem(LIST_STRATEGYSTATUS))->InsertColumn(1, _T("��ͷ�ֲ�"), LVCFMT_CENTER, 60, -1);
	((CListCtrl*)GetDlgItem(LIST_STRATEGYSTATUS))->InsertColumn(2, _T("��ͷ�ֲ�"), NULL, 60, -1);
	((CListCtrl*)GetDlgItem(LIST_STRATEGYSTATUS))->InsertColumn(3, _T("״̬"), NULL, 60, -1);
	((CListCtrl*)GetDlgItem(LIST_STRATEGYSTATUS))->InsertColumn(4, _T("����"), NULL, 200, 1);
}

#endif
#endif //_DEBUG


// CStrategyStatusView message handlers

void CStrategyStatusView::OnReceiveData(WPARAM wParam, LPARAM lParam)
{//���²���״̬�б�
	if (wParam == WM_UPDATESTRATEGYSTATUS)
	{
		StrategyStatus* pStrategyStatus = reinterpret_cast<StrategyStatus*>(lParam);
		CString strategyName((pStrategyStatus->strategyName).c_str());
		if (strategyName == _T(""))
		{//�����쳣
			return;
		}
		int rowcount = ((CListCtrl*)(this->GetDlgItem(LIST_STRATEGYSTATUS)))->GetItemCount(); //��ȡ����
		for (int i = 0; i < rowcount; ++i)
		{
			CString rowheader = ((CListCtrl*)(this->GetDlgItem(LIST_STRATEGYSTATUS)))->GetItemText(i, 0); //��ͷΪ������
			if (rowheader == strategyName)
			{//ˢ��������
				CString position;
				if (pStrategyStatus->position_buy != -1)
				{//-1��ζ�Ÿ�ֵ������
					position.Format(_T("%d"), pStrategyStatus->position_buy);
					((CListCtrl*)(this->GetDlgItem(LIST_STRATEGYSTATUS)))->SetItemText(i, 1, position);
				}
				if (pStrategyStatus->position_sell != -1)
				{//-1��ζ�Ÿ�ֵ������
					position.Format(_T("%d"), pStrategyStatus->position_sell);
					((CListCtrl*)(this->GetDlgItem(LIST_STRATEGYSTATUS)))->SetItemText(i, 2, position);
				}
				CString status((pStrategyStatus->status).c_str());
				if (status != _T(""))
				{
					((CListCtrl*)(this->GetDlgItem(LIST_STRATEGYSTATUS)))->SetItemText(i, 3, status);
				}
				CString description((pStrategyStatus->description).c_str());
				if (description != _T(""))
				{
					((CListCtrl*)(this->GetDlgItem(LIST_STRATEGYSTATUS)))->SetItemText(i, 4, description);
				}
				return; //ˢ����return��ȥ
			}
		}
		//����Ϊ�������
		if (strategyName != _T(""))
		{
			((CListCtrl*)(this->GetDlgItem(LIST_STRATEGYSTATUS)))->InsertItem(rowcount, strategyName);
		}
		CString position;
		if (pStrategyStatus->position_buy != -1)
		{//-1��ζ�Ÿ�ֵ������
			position.Format(_T("%d"), pStrategyStatus->position_buy);
			((CListCtrl*)(this->GetDlgItem(LIST_STRATEGYSTATUS)))->SetItemText(rowcount, 1, position);
		}
		if (pStrategyStatus->position_sell != -1)
		{//-1��ζ�Ÿ�ֵ������
			position.Format(_T("%d"), pStrategyStatus->position_sell);
			((CListCtrl*)(this->GetDlgItem(LIST_STRATEGYSTATUS)))->SetItemText(rowcount, 2, position);
		}
		CString status((pStrategyStatus->status).c_str());
		if (status != _T(""))
		{
			((CListCtrl*)(this->GetDlgItem(LIST_STRATEGYSTATUS)))->SetItemText(rowcount, 3, status);
		}
		CString description((pStrategyStatus->description).c_str());
		if (description != _T(""))
		{
			((CListCtrl*)(this->GetDlgItem(LIST_STRATEGYSTATUS)))->SetItemText(rowcount, 4, description);
		}
	}
}



void CStrategyStatusView::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	get_strategymsg_distributor()->DelReceiver(this); //�ӹ۲����б����޳�
	__super::OnClose();
}
