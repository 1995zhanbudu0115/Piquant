
#pragma once
#include "CustomGridCtrl.h"
#include "../../ServiceLayer/CtpService.h"
#include "GridCtrlOrderList.h"
#include "GridCtrlPositionList.h"
#include "GridCtrlTradeList.h"
#include "GridCtrlUnfilledOrderList.h"
#include "GridCtrlPositionDetailList.h"
#include "GridCtrlCloseTradeList.h"
#include "GridCtrlParkedOrderList.h"
#include "GridCtrlParkedOrderActionList.h"
#include "GridCtrlConditionOrderList.h"
#include "../LocalMessageEvent/LocalMessage.h"
//#define WM_UPDATE_GRIDCTRL WM_USER+10 //������
/////////////////////////////////////////////////////////////////////////////
// COutputList ����

class CTradeSituationWnd : public CListBox
{
// ����
public:
	CTradeSituationWnd();

// ʵ��
public:
	virtual ~CTradeSituationWnd();

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()

};

class COutputWnd : public CDockablePane, public CtpMsgReceiver
{
// ����
public:
	COutputWnd();

	void UpdateFonts();

// ����
protected:
	CMFCTabCtrl	m_wndTabs;

	//ί���б�;
	CGridCtrlOrderList m_wndOrderList;
	//δ��ί���б�
	CGridCtrlUnfilledOrderList m_wndUnfilledOrderList;
	//�ֲ�;
	CGridCtrlPositionList m_wndPosition; 
	//�ֲ���ϸ�б�
	CGridCtrlPositionDetailList m_wndPositionDetail;
	//�ɽ�;
	CGridCtrlTradeList m_wndTrade;
	//ƽ���б�
	CGridCtrlCloseTradeList m_wndCloseTrade;
	//Ԥ���б�
	CGridCtrlParkedOrderList m_wndParkedOrderList;
	//Ԥ�񳷵��б�
	CGridCtrlParkedOrderActionList m_wndParkedOrderActionList;
	//������
	CGridCtrlConditionOrderList m_wndConditionOrderList;

protected:
	void FillBuildWindow();
	void FillDebugWindow();
	void FillFindWindow();

	void AdjustHorzScroll(CListBox& wndListBox);

// ʵ��
public:
	virtual ~COutputWnd();
	afx_msg LRESULT OnUpdateGridCtrl(WPARAM wParam, LPARAM lParam);
	virtual void OnReceiveData(WPARAM wParam, LPARAM lParam);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	

	DECLARE_MESSAGE_MAP()

	

};

