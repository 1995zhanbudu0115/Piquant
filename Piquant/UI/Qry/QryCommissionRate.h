#pragma once
#include "../../ServiceLayer/CtpMsgDistributor.h"

// CQryCommissionRate dialog

class CQryCommissionRate : public CDialogEx, public CtpMsgReceiver
{
	DECLARE_DYNAMIC(CQryCommissionRate)

public:
	CQryCommissionRate(CWnd* pParent = NULL);   // standard constructor
	virtual ~CQryCommissionRate();

// Dialog Data
	enum { IDD = DIALOG_QRYCOMMISSIONRATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();

public:
	afx_msg void OnBnClickedQryCommissionRate();
	virtual void OnReceiveData(WPARAM wParam, LPARAM lParam);

	// �󶨺�Լ��ѡ���ֵ����
	CString m_strInstrument;
	// �󶨽�������ѡ���ֵ����
	CString m_strExchange;
	afx_msg void OnClose();
};
