#pragma once
#include "../resource.h"

// CCancelOrderDlg dialog

class CCancelOrderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCancelOrderDlg)

public:
	CCancelOrderDlg(CString OrderSysID,
								CString InstrumentID,
								CString Direction,
								CString OpenClose,
								CString Price,
								CString insertVolume,
								CString ExchangeID,
								CWnd* pParent = NULL);   // standard constructor
	virtual ~CCancelOrderDlg();

// Dialog Data
	enum { IDD = DIALOG_CANCELORDER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();

public:
	// �󶨳����ı����ֵ����
	CString m_strOrderSysID;
	// �󶨺�Լ�ı����ֵ����
	CString m_strInstrumentID;
	// �󶨷����ı����ֵ����
	CString m_strDirection;
	// �󶨿�ƽ�ı����ֵ����
	CString m_strOpenClose;
	// �󶨽��������ֵ����
	CString m_strVolume;
	// �󶨼۸��ı����ֵ����
	CString m_strPrice;
	//��¼������,�ǿؼ�����
	CString exchangeID;
	afx_msg void OnBnClickedCancelorder();
	afx_msg void OnBnClickedBatchorderaction();
	afx_msg void OnBnClickedCancelparkedorder();
};
