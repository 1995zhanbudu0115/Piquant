#pragma once
#include "StandardMakeOrderBoard.h"��//�µ��������õ��࣬������ͷ�ļ�

// CMakeOrderBoardPane

class CMakeOrderBoardPane : public CDockablePane
{
	DECLARE_DYNAMIC(CMakeOrderBoardPane)

public:
	CMakeOrderBoardPane();
	virtual ~CMakeOrderBoardPane();

protected:
	DECLARE_MESSAGE_MAP()

protected:
	CStandardMakeOrderBoard* m_wndSMOBoard; //��屾����û����Դ�ģ�Ҫ����������µ��壬����Ӹ�CStandardMakeOrderBoard���ָ��

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct); //��Ϣӳ�亯��
	afx_msg void OnSize(UINT nType, int cx, int cy); //��Ϣӳ�亯��

};


