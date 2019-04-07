#ifndef _TEST_STRATEGY_H_
#define _TEST_STRATEGY_H_
#include "../Interface/IStrategy.h"
#include <set>

#ifdef _WIN32
#ifdef STRATEGYEXAMPLE_EXPORTS
#define STRATEGYEXAMPLE_API _declspec(dllexport)
#else
#define STRATEGYEXAMPLE_API _declspec(dllimport)
#endif
#else
#define STRATEGYEXAMPLE_API 
#endif


class STRATEGYEXAMPLE_API TestStrategy :public IStrategy
{
public:
	TestStrategy();
	virtual ~TestStrategy();

	virtual void OnTick(_Tick& market_data);
	virtual void OnOrder(_Order& order);
	virtual void OnTrade(_Trade& trade);

protected:
	vector<_Order> UnfilledOrderVector;                   //���ڼ�¼��ò�����ص�δ��ί��, ����ordersysID��orderstatus

	//������ر�����δ�������Զ�ȡ�����ļ�����ʽ��ȡ
protected:
	int count;				//ǿ�Ƽ���
	int lastDirection;	//��һǿ�Ʒ���
	double lastPrice;	//��һ���¼�
	int vol;					//Ĭ�Ͽ�����
};

#endif