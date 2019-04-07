#ifndef _ISERVICE_H_
#define _ISERVICE_H_
#include "stdafx.h"
#include<string>
#include"ServerInfo.h"
using namespace std;

struct BaseData;  //��ǰ����������include "Datatypes.h", ��ֹ�ظ�����
struct Exchange;
struct InvestorPositionDetail;

class SERVICE_API IService
{
public:
	IService();
	virtual ~IService();
	virtual bool login(const string& user_name, const string& password, const ServerInfo& broker_id)=0;
	virtual bool logout()=0;
	virtual bool islogined() = 0;
	virtual bool request(int request_id,  void* param)=0;
	virtual bool get_basic_data(vector<BaseData>& base_data)=0;	//��ȡ��Լ��������
	virtual bool get_exchanges(vector<Exchange>& exchanges)=0;	//��ȡ��������Ϣ
	virtual bool get_all_InvestorPositionDetails(vector<InvestorPositionDetail>& positiondetails)=0;	//��ȡ�ֲ���ϸ����
	virtual std::string get_username() = 0;		//��ȡ�û���
	virtual ServerInfo get_brokerinfo() = 0;		//��ȡ��������Ϣ
};

#endif

