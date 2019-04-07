#include "stdafx.h"
#include "GlobalMgr.h"
#include "../ServiceLayer/IService.h"
#include "../ServiceLayer/CtpService.h"


CGlobalMgr::CGlobalMgr()
	:m_Service(NULL),
	m_DataStore(NULL)
{
	Timequery_switch = true;
}


CGlobalMgr::~CGlobalMgr()
{
	if (NULL != m_Service)
	{
		delete m_Service;
		m_Service = NULL;
	}

	if (NULL != m_DataStore)
	{
		delete m_DataStore;
		m_DataStore = NULL;
	}

	if (NULL != m_log)
	{
		delete m_log;
		m_log = NULL;
	}
}

IService* CGlobalMgr::GetService()
{
	if (NULL == m_Service)
	{
		m_Service = new CtpService();//�����ctpservice�����
	}
	return m_Service;
}

IDataStore* CGlobalMgr::GetDataStore()
{
	if (NULL == m_DataStore)
	{
		m_DataStore = new IDataStore();

		//ֱ���ڴ�����ʱ���¼�����ݿ�
		ServerInfo si;//�˴��������ص�sql server���ݿ��������Ϣ
		si.protocol = "SQL Server";//���ݿ�����
		si.name = "MultiTrader";//ODBC����Դ��,�����ж���
		si.id = "MultiTraderDB";//���ݿ���,�����ж���
		m_DataStore->login("mt", "w123123", si);//��д���ݿ��û���������
	}
	return m_DataStore;
}

Logger<TextDecorator>* CGlobalMgr::GetLog()
{
	if (NULL == m_log)
	{
		m_log = new Logger<TextDecorator>("log", "��־��¼", true, true); //����logger
	}
	return m_log;
}

//��Ϊ��Ԫ����
CGlobalMgr& get_global_mgr()
{//����������CGlobalMgr�����
	static CGlobalMgr g_GlobalMgr;
	return g_GlobalMgr;
}
