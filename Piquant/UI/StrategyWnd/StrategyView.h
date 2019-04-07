#pragma once
#include "StrategyStatusView.h"
#include "StrategyMsgView.h"

// CStrategyView

class CStrategyView : public CDockablePane
{
	DECLARE_DYNAMIC(CStrategyView)

public:
	CStrategyView();
	virtual ~CStrategyView();

protected:
	CFrameWnd*      m_pMyFrame;    //������
	CSplitterWnd      m_cSplitter;       // ���ҷָ����Ӵ���

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


