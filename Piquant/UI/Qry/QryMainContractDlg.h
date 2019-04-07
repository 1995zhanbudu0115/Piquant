#pragma once
#include "../../ServiceLayer/CtpMsgDistributor.h"
#include "../../ServiceLayer/DataTypes.h"
#include <vector>
#include <set>
#include <map>
#include <utility>//pair��ģ��ʹ��
#define MSG_QRYMAINCONTRACT WM_USER+1001 //���ڸ�PostThreadMessageʹ�õ��Զ�����Ϣ

// CQryMainContractDlg dialog

class CQryMainContractDlg : public CDialogEx, public CtpMsgReceiver
{
	DECLARE_DYNAMIC(CQryMainContractDlg)

public:
	CQryMainContractDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CQryMainContractDlg();

// Dialog Data
	enum { IDD = DIALOG_QRYMAINCONTRACT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	virtual void OnReceiveData(WPARAM wParam, LPARAM lParam);

	virtual BOOL OnInitDialog();

public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedQrymaincontract();

public:
	bool button_first_click;
	int Product_InvaildInstrument_cnt;//�ò�Ʒ��������Ч��Լ��Ŀ
	int insertInstrument_cnt;
	vector<DepthMarketData> product_invaildInstrumentSet;

	bool button_second_click;
	map<string, int> Product_InvaildInstrument_cnt_multimap;
	map<string, int> Product_insertInstrument_cnt;
	multimap<string, DepthMarketData> product_invaildInstrument_insert_multimap;
	vector<CString> maincontract_vector;

	HANDLE  hThread;   //�߳̾��
	DWORD  ThreadID;  //�߳�ID

public:
	afx_msg void OnBnClickedSpecificqrymaincontract();

public:
	friend void ThreadFunc_UpdateDlg(); //��Ԫ�̺߳���
};

struct  MSG_Param_QryMainContract
{//�̼߳䴫�͵���Ϣ�ṹ��
	CQryMainContractDlg* pQryMainContractDlg;
	WPARAM wParam;
	LPARAM lParam;
};