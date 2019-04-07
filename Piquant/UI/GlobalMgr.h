#ifndef _GLOBAL_MGR_H_
#define _GLOBAL_MGR_H_ 

#include "../ServiceLayer/IDataStore.h"
#include "../Tool/minilogger.h"

class IService;//ǰ������

class CGlobalMgr
{
protected:
	CGlobalMgr();
	virtual ~CGlobalMgr();

public:
	IService* GetService();
	IDataStore* GetDataStore();				//��ȡ���ݿ��������
	Logger<TextDecorator>* GetLog();  //��ȡ��־��¼��
	friend  CGlobalMgr& get_global_mgr();
	bool GetTimeQry_Switch(){ return Timequery_switch; }//��ȡ��ʱ��ѯ�ĵ����л�״̬
	void SetStopTimeQry(){ Timequery_switch = false; }
	void SetStartTimeQry(){ Timequery_switch = true; }

	void SetLocalMessageThreadID(DWORD threadID){ ThreadID_LocalMessageThread = threadID; };
	DWORD GetLocalMessageThreadID(){return ThreadID_LocalMessageThread;}

protected:
	IService* m_Service;
	IDataStore* m_DataStore;
	bool Timequery_switch;//��ʱ��ѯ�ĵ����л�״̬
	DWORD  ThreadID_LocalMessageThread;	 //������Ϣ�߳�ID����Ӧ�ó���ȫ�ַ���
	Logger<TextDecorator>* m_log;  //��־��¼��      
};

#endif
