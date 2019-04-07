#pragma once
#include "../stdafx.h"
#include "afxwin.h"
#include "afxext.h"
#include "afxcmn.h"
#include "afxdialogex.h"
#include "../resource.h"//��Դͷ�ļ��������������mfc�������
#include "../../ServiceLayer/CtpMsgDistributor.h"		
#include "../../Tool/IniFile.h"							


// CLoginDlg �Ի���
class CLoginDlg : public CDialogEx, public CtpMsgReceiver
{
// ����
public:
	CLoginDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = DIALOG_LOGIN };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedExitApp();
	afx_msg void OnBnClickedLogin();
	afx_msg void OnBnClickedStorePassword();
	afx_msg void OnClose();
	afx_msg void OnLoginProgress(NMHDR *pNMHDR, LRESULT *pResult);

	CEdit m_edtUsername;
	CEdit m_edtPassword;
	CComboBox m_cbServers;
	CString m_strusername;
	CString m_strpassword;
	BOOL m_bSaveUserName;
	CProgressCtrl m_pbLoginProgress;
	std::string m_strConfigfile;//ini�ļ���
	bool m_storepassword;

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual void OnReceiveData(WPARAM wParam, LPARAM lParam);
};
