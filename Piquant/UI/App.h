//Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


class CApp : public CWinAppEx
{
public:
	CApp();

	CMultiDocTemplate* pDocTemplate_CFFEX;
	CMultiDocTemplate* pDocTemplate_ShowInstrument;
	CMultiDocTemplate* pDocTemplate_SHFEINEView;
	CMultiDocTemplate* pDocTemplate_DCEView;
	CMultiDocTemplate* pDocTemplate_CZCEView;

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	DECLARE_MESSAGE_MAP()
};

extern CApp theApp;
