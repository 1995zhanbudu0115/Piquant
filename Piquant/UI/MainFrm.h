
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "StrategyWnd/StrategyTree.h"
#include "TradeSituationWnd/TradeSituationWnd.h"
#include "MakeOrder/MakeOrderBoardPane.h"
#include "AccountStatusBar/AccountStatusBar.h"
#include"../ServiceLayer/CtpService.h"
#include "../ServiceLayer/DataTypes.h"
#include "../ServiceLayer/CtpMsgDistributor.h"
#include "GlobalMgr.h"
#include "LocalMessageEvent/LocalMessage.h"
#include "LocalMessageEvent/LocalMessageEventHandle.h"
#include "StrategyWnd/StrategyView.h"
#define WM_UPDATE_STATUSBAR WM_USER+1003 //�Զ������Ϣ����

class CMainFrame : public CMDIFrameWndEx, public CtpMsgReceiver
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	
protected:  // �ؼ���Ƕ���Ա
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCToolBarImages m_UserImages;
	CAccountStatusBar     m_wndAccountStatusBar;
	CFileView         m_wndFileView;
	CStrategyView  m_wndStrategyView;
	COutputWnd        m_wndOutput;
	//��Ӹ��µ���
	CMakeOrderBoardPane m_wndSMOBoardPane;
// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
public:
	afx_msg void OnSystemLogin();
	afx_msg void OnQrycommissionrate();
	afx_msg void OnQryinstrumentmarginrate();
	afx_msg void OnQryexchangerate();
	afx_msg void OnQryordercommrate();
	afx_msg void OnLinkexchange();
	afx_msg void OnQrymaincontract();
	afx_msg LRESULT  OnUpdateStatusBar(WPARAM wParam, LPARAM lParam);//�Զ������Ϣӳ�亯��

public:
	virtual void OnReceiveData(WPARAM wParam, LPARAM lParam);

public:
	HANDLE  hThread_LocalMessageThread;   //������Ϣ�߳̾��
	DWORD  ThreadID_LocalMessageThread;  //������Ϣ�߳�ID
	HANDLE  hThread;   //��ʱ��ѯ�߳̾��
	DWORD  ThreadID;  //��ʱ��ѯ�߳�ID
	afx_msg void OnSystemLoginout();
};

//ȫ�ֺ���
void CALLBACK  Timerproc_LocalMessageThread(); //������Ϣ�̺߳���ԭ������
void CALLBACK  Timerproc_ReqQry();					   //��ʱ��ѯ�̺߳���ԭ������