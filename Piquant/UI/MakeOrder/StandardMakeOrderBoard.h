#pragma once
#include "../resource.h"��//�������Դ��ص�ͷ�ļ�
#include "afxwin.h"
#include "atltime.h"
#include "afxdtctl.h"
#include "afxcmn.h"
#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")
#include "../LocalMessageEvent/LocalMessage.h"
#include "../../ServiceLayer/CtpMsgDistributor.h"

// CStandardMakeOrderBoard form view

class CStandardMakeOrderBoard : public CFormView, public CtpMsgReceiver
{//���ؼ̳������еĽ�����������
	DECLARE_DYNCREATE(CStandardMakeOrderBoard)

public:
	CStandardMakeOrderBoard(CWnd* pParent = NULL);  //���ǻ����µ�����Ĺ��캯����ȥ�����µ���ʵ��
	virtual ~CStandardMakeOrderBoard();

public:
	enum { IDD = FORMVIEW_STANDARD_MAKEORDER_BOARD };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOrderInsert();
	// �Ӹ�ѡ���л�ȡ�ĺ�ԼID����ڴ�
	CString m_strInstrumentID;
	// �������ı����ֵ��������¼�µ�����
	UINT m_nInstrumentVolume;
	// ��ָ���ۿؼ��Ŀؼ�����, �����л��۸�����
	CButton m_btnPriceType;
	afx_msg void OnBnClickedPricetype();
	virtual void OnInitialUpdate();     //���µ��屻���δ�����ʱ�򣬻���ø����ص��麯�������ǿ���������Щ��ʼ���Ĳ���

	virtual void OnReceiveData(WPARAM wParam, LPARAM lParam);

	// �󶨸�ѡ��Ŀؼ�����
	CComboBox m_cbInstruments;
	// �󶨽�������ѡ��Ŀؼ�����
	CComboBox m_cbExchanges;
	// ���׸�ѡ����ѡ�����������ı�ֵ����
	CString m_strExchange;
	// �������͸�ѡ���ֵ����
	CString m_strTouchPriceType;
	// �������͸�ѡ��Ŀؼ����� 
	CComboBox m_cbTouchPriceType;
	// �����۸��ı����ֵ����
	double m_InstrumentPrice;
	// �����۸����
	double m_TouchPrice;
	// �������ı����۸�
	double m_TradePrice;
	// ����Ч�����Ϳ�Ŀؼ�����
	CComboBox m_cbTimeCondition;
	// ����Ч�����͵�ֵ����
	CString m_strTimeCondition;
	// ������ѡ��ؼ��Ŀؼ�����	// ������ѡ��ؼ��Ŀؼ�����
	CDateTimeCtrl m_GTD;
	afx_msg void OnBnClickedTouchpricePricetype();
	// �󶨴������۸����Ϳؼ��Ŀؼ�����
	CButton m_btnPriceType_TouchPrice;
	// ���޼�����������Ŀؼ�����
	CComboBox m_cbPriceType;
	// �󶨽������䶯��ť�Ŀؼ�����
	CSpinButtonCtrl m_SpinVolumeChange;
	afx_msg void OnDeltaposVolume(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeVolume();
	// �۸����͸�ѡ��ؼ�����,���ڷ�Ԥ������м۵�
	CComboBox m_cbPriceType_Normal;
	afx_msg void OnEnChangePrice();
	// �۸�䶯��ť�ؼ�����
	CSpinButtonCtrl m_SpinPriceChange;
	afx_msg void OnDeltaposPrice(NMHDR *pNMHDR, LRESULT *pResult);
	// �󶨴����۸�䶯��ť�ؼ�����
	CSpinButtonCtrl m_SpinTouchPriceChange;
	afx_msg void OnDeltaposTouchprice(NMHDR *pNMHDR, LRESULT *pResult);
	// �󶨴��۽��ױ����۸�䶯��ť�Ŀؼ�����
	CSpinButtonCtrl m_SpinPriceChange_Touch;
	afx_msg void OnDeltaposPriceTouch(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeStopprice();
	afx_msg void OnEnChangeTradeprice();
	afx_msg void OnBnClickedRetset();
	// �󶨼۸���ϸ��Ϣ���Ŀؼ�����
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	// �󶨼۸���ϸ��ܵĿؼ�����
	CStatic m_gbPriceDetail;
	afx_msg LRESULT  OnUpdateTickDetailDisp(WPARAM wParam, LPARAM lParam);//�Զ������Ϣӳ�亯��
	afx_msg void OnClose();
};


