
#pragma once

#include "ViewTree.h"
#include "../../Interface/IStrategy.h"

class CFileViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CFileView : public CDockablePane
{
// ����
public:
	CFileView();

	void AdjustLayout();
	void OnChangeVisualStyle();


	virtual BOOL CanBeClosed() const; //�������ò��ɹر�


	virtual BOOL FloatPane(CRect rectFloat, AFX_DOCK_METHOD dockMethod = DM_UNKNOWN, bool bShow = true);

	// ����
protected:

	CViewTree m_wndFileView;
	CImageList m_FileViewImages;
	CFileViewToolBar m_wndToolBar;

protected:
	void FillFileView();

// ʵ��
public:
	virtual ~CFileView();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void StartStrategy();
	afx_msg void OnRefresh();
	afx_msg void OnOpenfile();
	afx_msg void OnStopstrategy();
};

