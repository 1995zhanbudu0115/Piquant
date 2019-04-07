#pragma once
#include "afxwin.h"
#include "../http/HttpDataEntry.h" //��ȡ���ݵ����ݽṹ
#include "../http/HttpHelper.h"	  //http������
#include "../http/HttpDataSource.h" //http�µ�һЩ�������
#include "afxdialogex.h"
//ͼ�ο�
#include <GdiPlus.h>
#pragma comment(lib, "GdiPlus.lib")

// CRealtimePriceDlg dialog

class CRealtimePriceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRealtimePriceDlg)

public:
	CRealtimePriceDlg(CString InstrumentID_eastMoneyneed, CString InstrumentID_sinaneed, CWnd* pParent = NULL);   // standard constructor
	virtual ~CRealtimePriceDlg();

// Dialog Data
	enum { IDD = DIALOG_REALTIMEPRICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

private:
	// ������ �������ͷ�����
	//typedef CMRKJDatabase::TupleStockInfo INFO, *PINFO;
	// �洢 ʵʱ����
	data_http::CHttpDataEntry m_httpData{};
	// ���ص�K��ͼ·��
	std::wstring m_strFilePath;
	CString m_strCode;
	std::string rowInstrumentID_eastmoney;
	std::string rowInstrumentID_sina;

public:
	enum WM_REAL_TIME 
	{
		sc_MSG_MIN = (WM_USER + 101),
		sc_MSG_HTTP_QUERY_END, //���ݲ�ѯ�����Ӧ��Ϣ
	};

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);  //��Ϣ�ַ�
	afx_msg void OnPaint();	    //�ػ�
	afx_msg void OnDestroy();	//����������ʱ����
	afx_msg LRESULT OnMsgHttpQueryEnd(WPARAM wParam, LPARAM lParam);		// ����ѯ�̲߳�ѯ������.�ᴥ������Ϣ

	DECLARE_MESSAGE_MAP()
	
};
