#pragma once
#include "../resource.h"
#include "../../Tool/ConfigReader.h"
#include "../LocalMessageEvent/LocalMessage.h"
#include "../LocalMessageEvent/LocalMessageEventHandle.h"
#include "../MarketDataView/MarketDataGridCtrl.h"

class CGridCtrl;//ǰ������

class CCFFEXView : public CFormView, public CtpMsgReceiver
{
protected: // �������л�����
	CCFFEXView();
	DECLARE_DYNCREATE(CCFFEXView)

public:
	enum{ IDD = IDD_MULTITRADER_FORM };

// ����
public:
	CDoc* GetDocument() const;
	CMarketDataGridCtrl* m_pGridCtrl;

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CCFFEXView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg LRESULT  OnUpdateGridCtrl(WPARAM wParam, LPARAM lParam);//�Զ������Ϣӳ�亯��
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	void SetGridCtrlColumns(const vector<ColumnInfo>& header_columns);
	virtual void OnReceiveData(WPARAM wParam, LPARAM lParam);//CMultiTraderView�����˼̳���IServiceReceiver��ķ���
	virtual void OnDraw(CDC* pDC);
	afx_msg void OnClose();
};

#ifndef _DEBUG  // MultiTraderView.cpp �еĵ��԰汾
inline CDoc* CCFFEXView::GetDocument() const
   { return reinterpret_cast<CDoc*>(m_pDocument); }
#endif

//������
class CCFFEX_ChildView : public CCFFEXView
{
public:
	CCFFEX_ChildView() :CCFFEXView(){};
	~CCFFEX_ChildView();
	void UpdateMarketDataView(LPARAM lParam);
};