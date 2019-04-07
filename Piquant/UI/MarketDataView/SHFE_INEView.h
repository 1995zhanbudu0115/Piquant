#pragma once
#include "../resource.h"
#include "../../Tool/ConfigReader.h"
#include "../GridCtrl/GridCtrl.h"
#include "../Doc.h"
#include "../LocalMessageEvent/LocalMessage.h"
#include "../../ServiceLayer/CtpMsgDistributor.h"
#include "../../ServiceLayer/DataTypes.h"
#include "../../ServiceLayer/CtpService.h"
#include <vector>
#include <set>
#include "MarketDataGridCtrl.h"
class CGridCtrl;//ǰ������

// CSHFE_INEView view

class CSHFE_INEView : public CView, public CtpMsgReceiver
{
	DECLARE_DYNCREATE(CSHFE_INEView)

protected:
	CSHFE_INEView();           // protected constructor used by dynamic creation
	virtual ~CSHFE_INEView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	CMarketDataGridCtrl* m_pGridCtrl;//ָ�����
public:
	virtual void OnReceiveData(WPARAM wParam, LPARAM lParam);
	virtual void OnInitialUpdate();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT  OnUpdateGridCtrl(WPARAM wParam, LPARAM lParam);//�Զ������Ϣӳ�亯��
};

//������
class CSHFE_INE_ChildView : public CSHFE_INEView
{
public:
	CSHFE_INE_ChildView() :CSHFE_INEView(){};
	~CSHFE_INE_ChildView();
	void UpdateMarketDataView(LPARAM lParam);
};