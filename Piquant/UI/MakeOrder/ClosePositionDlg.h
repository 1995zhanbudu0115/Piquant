#pragma once
#include "afxwin.h"
#include "../../ServiceLayer/CtpMsgDistributor.h"


// CClosePositionDlg dialog

class CClosePositionDlg : public CDialogEx, public CtpMsgReceiver
{
	DECLARE_DYNAMIC(CClosePositionDlg)

public:
	CClosePositionDlg(CString InstrumentID,
								 CString Direction,
								 CString Position,
								 CString PrePosition,
								 CString TodayPosition,
								 CString SettlementPrice,
								CWnd* pParent = NULL);   // standard constructor
	virtual ~CClosePositionDlg();

// Dialog Data
	enum { IDD = DIALOG_POSITION };


	virtual void OnReceiveData(WPARAM wParam, LPARAM lParam);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();

public:
	// ��Լ���cstringֵ����
	CString m_strInstrument;
	// �󶨷�����ctringֵ����
	CString m_strDirection;
	// �󶨽��������ctringֵ����
	CString m_strVolume;
	afx_msg void OnBnClickedPricetype();
	// �۸�����ѡ��Ŀؼ���ť
	CButton m_bnPriceType;
	// �󶨽��׼۸���doubleֵ����
	double m_Price;
	afx_msg void OnBnClickedClose();
	// �󶨽�������ѡ���ctringֵ����
	CString m_strExchange;
	// ��������Ŀؼ�����
	CComboBox m_cbExchange;

	CString SettlementPrice;//�Զ���ģ���ؼ��޹�
	afx_msg void OnEnChangeVolume();
	afx_msg void OnEnChangePrice();
	afx_msg void OnEnChangeTouchprice();
	afx_msg void OnClose();
};
