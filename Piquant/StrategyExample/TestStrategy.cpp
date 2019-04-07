#include "stdafx.h"
#include "TestStrategy.h"
#include "../Tool/rwJson.h"


TestStrategy::TestStrategy()
{
	//���Ա�дģ���Դ�(����)
	this->StrategyDescription = "��������(������)";
	this->StrategyName = "StrategyExample";
	this->instrument_id = "cu1905";//���ö��ĵ�Ʒ��, ע�����ִ�Сд
	//��json�ж�ȡ�ֲ�����
	get_rwJson().getPosition(this->StrategyName, this->position_buy, this->position_sell);
	
	//��ʼ�����Բ���
	count = 0;
	lastDirection = 0;
	lastPrice = 0;
	vol = 1;//Ĭ�Ͽ�һ��
}


TestStrategy::~TestStrategy()
{
	//�ڴ˴�д�ֲ�����Ч�ģ�����ر�ʱ������ִ�и�����
}

void TestStrategy::OnTick(_Tick& market_data)
{
	//::MessageBox(NULL, _T("OnTick����"), _T("����"), MB_OK);//������
	if (lastPrice==0)
	{//��¼���¼�
		lastPrice = market_data.LastPrice;
		lastDirection = 0;//����ǰ�۱ȶ�,���Բ������򣬶�ͷǿ��Ϊ1����ͷǿ��Ϊ-1
	}
	else
	{
		if (market_data.LastPrice>lastPrice)
		{//���¼�>��һ���¼�
			if (lastDirection!=0)
			{
				if (lastDirection == 1)
				{//�ɳ����Ķ�ͷǿ��
					count++;
					lastPrice = market_data.LastPrice;
				}
				else
				{//��һΪ��ͷǿ�ƣ����Ϊ��ͷǿ�ƣ����ü���
					count = 1;
					lastDirection = 1;
					lastPrice = market_data.LastPrice;
				}
			}
			else
			{//�״γ�ʼ��ǿ�Ʒ���
				count++;
				lastDirection = 1;
				lastPrice = market_data.LastPrice;
			}
		}

		if (market_data.LastPrice < lastPrice)
		{//���¼�<��һ���¼�
			if (lastDirection != 0)
			{
				if (lastDirection == -1)
				{//�ɳ����Ŀ�ͷǿ��
					count++;
					lastPrice = market_data.LastPrice;
				}
				else
				{//��һΪ��ͷǿ�ƣ����Ϊ��ͷǿ�ƣ����ü���
					count = 1;
					lastDirection = -1;
					lastPrice = market_data.LastPrice;
				}
			}
			else
			{//�״γ�ʼ��ǿ�Ʒ���
				count++;
				lastDirection = -1;
				lastPrice = market_data.LastPrice;
			}
		}
	}

	if (position_buy==0 && position_sell==0)
	{
		if (count >= 3 && lastDirection == 1)
		{//����3��tick���ֶ�ͷǿ�ƣ��򿪲֣���ֻ��һ��
			make_order(
				market_data.InstrumentID,
				THOST_FTDC_D_Buy,
				THOST_FTDC_OF_Open,
				market_data.AskPrice1, vol);
		}

		if (count >= 3 && lastDirection == -1)
		{//����3��tick���ֿ�ͷǿ�ƣ��򿪲֣���ֻ��һ��
			make_order(
				market_data.InstrumentID,
				THOST_FTDC_D_Sell,
				THOST_FTDC_OF_Open,
				market_data.BidPrice1, vol);
		}
	}
}


void TestStrategy::OnOrder(_Order& order)
{//��ȡ���д��б�����ŵĶ����������������һ�µİ����µĸ��ǣ���ȡ״̬��Ϣ���Զ������з��౾�ػ���
	std::vector<std::string> orderref_strs=split(order.OrderRef, "_");	//ע��ָ��Ϊ"_",����CTP��װ���make_order()�趨��
	if (orderref_strs[0]==this->StrategyName)
	{//��ǰ׺ƥ��,˵�����ɸò��Է�����ί�в����Ļص�
		if (order.OrderStatus == "���ֳɽ����ڶ�����" ||
			order.OrderStatus == "δ�ɽ����ڶ�����")
		{//����״̬Ϊ�������͵�Ӧ���ػ�����Щδ��ί����Ϣ, �ؼ������䱨����ţ������������������
			for (vector<_Order>::iterator it = UnfilledOrderVector.begin(); it != UnfilledOrderVector.end();)
			{//ע�ⲿ�ֳɽ�����£���������������ʱ�ر�����δ��ί�еı��������һֱһ�µģ�ֻ�Ƿ����ɽ��³ɽ���Ÿ��β�ͬ, Ϊ��Ӧ���ǲ�����ί����Ϣ
				if (strcmp((it->OrderSysID).c_str(), (order.OrderSysID).c_str()) == 0)
				{//���ǣ�����ɾ������ѹ��
					it = UnfilledOrderVector.erase(it); //ɾ��������ָ���Ԫ��,erase�������������ָ�����Ч���˴���������
					UnfilledOrderVector.push_back(order);
					return; //ˢ�����ֱ��return
				}
				else
				{
					++it;
				}
			}
			UnfilledOrderVector.push_back(order);//���������������,ѹ�뼴��
		}

		if (order.OrderStatus == "����")
		{//���Ѿ������������vector���޳�
			for (vector<_Order>::iterator it = UnfilledOrderVector.begin(); it != UnfilledOrderVector.end();)
			{
				if (strcmp((it->OrderSysID).c_str(), (order.OrderSysID).c_str()) == 0)
				{//ƥ�䱨�����
					it = UnfilledOrderVector.erase(it);
					return;
				}
				else
				{
					++it;
				}
			}
		}

		if (order.OrderStatus == "ȫ���ɽ�")
		{//��ȫ���ɽ��������vector���޳�
			for (vector<_Order>::iterator it = UnfilledOrderVector.begin(); it != UnfilledOrderVector.end();)
			{
				if (strcmp((it->OrderSysID).c_str(), (order.OrderSysID).c_str()) == 0)
				{//ƥ�䱨�����
					it = UnfilledOrderVector.erase(it);
					return;
				}
				else
				{
					++it;
				}
			}
		}
	}
}

void TestStrategy::OnTrade(_Trade& trade)
{
	std::vector<std::string> orderref_strs = split(trade.OrderRef, "_");	//ע��ָ��Ϊ"_",����CTP��װ���make_order()�趨��
	if (orderref_strs[0] == this->StrategyName)
	{//��ǰ׺ƥ��,˵�����ɸò��Է�����ί�в����ĳɽ��ص�
		//���ֶ࿪/��ƽ/�տ�/��ƽ��ˢ�³ֲֻ��漰�־û�
		if (trade.Direction == "��")
		{
			if (trade.OffsetFlag == "����")
			{
				position_buy += 1;
				get_rwJson().storePosition(this->StrategyName, this->position_buy, this->position_sell);	//��json��д��ֲ�����
			}
			if (trade.OffsetFlag == "ƽ��")
			{
				position_buy -= 1;
				get_rwJson().storePosition(this->StrategyName, this->position_buy, this->position_sell);	//��json��д��ֲ�����
			}
			if (trade.OffsetFlag == "ƽ��")
			{
				position_buy -= 1;
				get_rwJson().storePosition(this->StrategyName, this->position_buy, this->position_sell);	//��json��д��ֲ�����
			}
		}
		if (trade.Direction == "��")
		{
			if (trade.OffsetFlag == "����")
			{
				position_sell += 1;
				get_rwJson().storePosition(this->StrategyName, this->position_buy, this->position_sell);	//��json��д��ֲ�����
			}
			if (trade.OffsetFlag == "ƽ��")
			{
				position_sell -= 1;
				get_rwJson().storePosition(this->StrategyName, this->position_buy, this->position_sell);	//��json��д��ֲ�����
			}
			if (trade.OffsetFlag == "ƽ��")
			{
				position_sell -= 1;
				get_rwJson().storePosition(this->StrategyName, this->position_buy, this->position_sell);	//��json��д��ֲ�����
			}
		}
	}
}


#ifdef __cplusplus
extern "C"
{
#endif
	STRATEGYEXAMPLE_API void* CreateStrategy()
	{
		static TestStrategy* pStrategy;
		if (pStrategy==NULL)
		{
			pStrategy = new TestStrategy();
		}
		return pStrategy;
	}
#ifdef __cplusplus
}
#endif

