#include "stdafx.h"
#include "CtpMsgDistributor.h"

CtpMsgDistributor::CtpMsgDistributor()
{
}


CtpMsgDistributor::~CtpMsgDistributor()
{
}

void CtpMsgDistributor::AddReceiver(CtpMsgReceiver* receiver)
{
	vector<CtpMsgReceiver*>::iterator it = std::find(receivers.begin(), receivers.end(), receiver);
	if (it == receivers.end())
	{//�����ڳ�β������,˵��û�ҵ�,��ѹ�������������
		receivers.push_back(receiver);
	}
}

void CtpMsgDistributor::DelReceiver(CtpMsgReceiver* receiver)
{
	//��ĳ�����������������������޳���ȥ
	for (vector<CtpMsgReceiver*>::iterator it = receivers.begin(); it != receivers.end(); ++it)
	{
		if (*it == receiver)//����ַ��ͬ
		{
			it = receivers.erase(it);//����ָ���ĵ�����,ɾ��ָ���Ľ�����
			break;
		}
	}
}

void  CtpMsgDistributor::OnResponse(int response_id, LPARAM lParam)
{
	//��receivers���vector�����е����н����߶����ø������ص�OnReceiveData�������������ݣ���Ȼ�����Ǵ�������룬����OnReceiveData���غ����İ汾�����Ľ���������йأ�
	for (int i = 0; i<receivers.size(); ++i)
	{
		if (NULL != receivers[i])
		{
			receivers[i]->OnReceiveData(response_id, lParam);
		}
	}
}

CtpMsgDistributor* get_CtpMsgDistributor()
{
	static CtpMsgDistributor* g_CtpMsgDistributor = NULL;
	if (NULL == g_CtpMsgDistributor)
	{
		g_CtpMsgDistributor = new CtpMsgDistributor();
	}
	return g_CtpMsgDistributor;
}
