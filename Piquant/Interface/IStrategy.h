#ifndef _ISTRATEGY_H_
#define _ISTRATEGY_H_

#ifdef _WIN32
#ifdef STRATEGY_EXPORTS
#define STRATEGY_API _declspec(dllexport)
#else
#define STRATEGY_API _declspec(dllimport)
#endif
#else
#define STRATEGY_API 
#endif

//���Կ����,δ�����в��Զ���������̳�����

#include "../ServiceLayer/CtpService.h"
#include "../ServiceLayer/CtpMsgDistributor.h"
#include "../ServiceLayer/DataTypes.h"
#include <vector>
#include "StrategyMessage.h" 
#include "DataTypes.h"
using namespace std;

class STRATEGY_API StrategyMsgDistributor
{//������Ϣ�ַ���
protected:
	StrategyMsgDistributor();
	virtual ~StrategyMsgDistributor();
public:
	void  OnResponse(int response_id, LPARAM lParam); //������Ϣ�ַ�
	virtual void OnReceiveData(WPARAM wParam, LPARAM lParam);	//�����������ڽ��ղ�����Ϣ
	void AddReceiver(StrategyMsgDistributor* receiver);
	void DelReceiver(StrategyMsgDistributor* receiver);
	friend STRATEGY_API StrategyMsgDistributor*  get_strategymsg_distributor();//��Ԫ����
protected:
	vector<StrategyMsgDistributor*> receivers;
};



class STRATEGY_API IStrategy : public CtpMsgReceiver
{
public:
	IStrategy();
	virtual ~IStrategy();

	void set_ApiInterface(IService* pApiInterface); //�����Ժ󲻽��Խ�CTP,Ҳ�ɶԽ�IB�������ӿ�
	void start(); //��������
	void stop(); //����ֹͣ
	bool isRunning(); //��¼����״̬,������������,������ֹͣ
	void OnReceiveData(WPARAM wParam, LPARAM lParam); //���ո��ֻر�
	
	//��������������
	virtual void OnTick(_Tick& market_data);
	virtual void OnOrder(_Order& order);
	virtual void OnTrade(_Trade& trade);
	

	//API�������ṩ�����Ա�д���ã�δ�����
	bool make_order(const std::string& instrumentID, //���ĺ�ԼID,�˴�ֻ֧�ֵ�Ʒ�����鶩��,ע������������ƽ��ƽ��
							  char Direction,
							  char OpenCloseType,
							  double price,
							  int volume
							  );
	bool cancel_order(const std::string& InstrumentID, const std::string& ExchangeID, const std::string& OrderSysID);
	bool PostMessage_StrategyStatus(StrategyStatus msg); //����״̬��Ϣ���ݺ���
	bool PostMessage_StrategyMsg(StrategyMsg msg);//������Ϣ���ݺ���
	std::vector<std::string> split(std::string str, std::string pattern);

	
protected:
	IService* ApiInterface;	    //�ӿڶ���
	string     instrument_id;    //��Լ��,ֻ֧�ֵ���Լ���ģ�һ�����Զ�Ӧһ��Ʒ��
	string StrategyName;
	string StrategyDescription;
	int position_buy;
	int position_sell;
	bool running;	//�Ƿ���������
};


#endif

