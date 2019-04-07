#include "stdafx.h"
#include "IStrategy.h"
#include "../ServiceLayer/CtpMsg.h"

IStrategy::IStrategy()
	:ApiInterface(NULL)
	, instrument_id("")
	, StrategyName("")
	, StrategyDescription("")
	, position_sell(0)
	, position_buy(0)
	, running(false)
{
}

IStrategy::~IStrategy()
{
}


void IStrategy::OnReceiveData(WPARAM wParam, LPARAM lParam)
{
	//��������;
	//MessageBoxA(NULL, ("�յ�����"), ("����"), MB_OK); //������
	if (wParam == ctp_msg::RtnMarketData)
	{//tick��������
		MarketData* pMarketData = reinterpret_cast<MarketData*>(lParam);
		if (NULL != pMarketData)
		{
			if (strcmp(pMarketData->code, (this->instrument_id).c_str())==0) //����Ƿ����������������
			{//�ٶ�������һ�ּ򻯷�װ
				static _Tick tick;
				tick.TradingDay = pMarketData->TradingDay;
				tick.InstrumentID = pMarketData->code;
				tick.ExchangeID = pMarketData->ExchangeID;
				tick.LastPrice = pMarketData->LastPrice;
				tick.PreSettlementPrice = pMarketData->PreSettlementPrice;
				tick.PreClosePrice = pMarketData->PreClosePrice;
				tick.PreOpenInterest = pMarketData->PreOpenInterest;
				tick.OpenPrice = pMarketData->OpenPrice;
				tick.HighestPrice = pMarketData->HighestPrice;
				tick.LowestPrice = pMarketData->LowestPrice;
				tick.Volume = pMarketData->Volume;
				tick.Turnover = pMarketData->Turnover;
				tick.OpenInterest = pMarketData->OpenInterest;
				tick.ClosePrice = pMarketData->ClosePrice;
				tick.SettlementPrice = pMarketData->SettlementPrice;
				tick.UpperLimitPrice = pMarketData->UpperLimitPrice;
				tick.LowerLimitPrice = pMarketData->LowerLimitPrice;
				tick.UpdateTime = pMarketData->UpdateTime;
				tick.UpdateMillisec = pMarketData->UpdateMillisec;
				tick.BidPrice1 = pMarketData->BidPrice;
				tick.BidVolume1 = pMarketData->BidVolume;
				tick.AskPrice1 = pMarketData->AskPrice;
				tick.AskVolume1 = pMarketData->AskVolume;
				OnTick(tick);
			}
		}
	}
	else if (wParam == ctp_msg::RtnOrder)
	{//��������
		Order *pOrder = reinterpret_cast<Order*>(lParam);
		if (NULL != pOrder)
		{
			if (strcmp(pOrder->InstrumentID, instrument_id.c_str()) == 0)
			{
				static _Order order;
				order.InstrumentID = pOrder->InstrumentID;

				if (pOrder->OrderPriceType == THOST_FTDC_OPT_AnyPrice)		order.OrderPriceType ="�����" ;
				else if (pOrder->OrderPriceType == THOST_FTDC_OPT_LimitPrice)		order.OrderPriceType = "�޼�";
				else order.OrderPriceType = "����";		//������������,�ݲ�����

				if (pOrder->Direction == THOST_FTDC_D_Buy)		order.Direction = "��";
				else 	order.Direction = "��";

				if (pOrder->CombOffsetFlag[0] == '0')		order.CombOffsetFlag = "����";
				else if (pOrder->CombOffsetFlag[0] == '1')		order.CombOffsetFlag = "ƽ��";
				else if (pOrder->CombOffsetFlag[0] == '3')		order.CombOffsetFlag = "ƽ��";
				else 	order.CombOffsetFlag = "����";	//�����ݲ�����

				order.LimitPrice = pOrder->LimitPrice;
				order.VolumeTotalOriginal = pOrder->VolumeTotalOriginal;
				order.StopPrice = pOrder->StopPrice;
				order.ExchangeID = pOrder->ExchangeID;
				order.VolumeTraded = pOrder->VolumeTraded;
				order.VolumeTotal = pOrder->VolumeTotal;
				order.InsertDate = pOrder->InsertDate;
				order.InsertTime = pOrder->InsertTime;
				order.StatusMsg = pOrder->StatusMsg;
				order.OrderSysID = pOrder->OrderSysID;
				order.OrderRef = pOrder->OrderRef;

				if (pOrder->ContingentCondition == THOST_FTDC_CC_Immediately)		order.ContingentCondition = "����";
				else if (pOrder->ContingentCondition == THOST_FTDC_CC_LastPriceGreaterEqualStopPrice)		order.ContingentCondition = "���¼۴��ڵ���������";
				else if (pOrder->ContingentCondition == THOST_FTDC_CC_LastPriceLesserEqualStopPrice)		order.ContingentCondition = "���¼�С�ڵ���������";
				else order.ContingentCondition = "����";		//������������,�ݲ�����

				if (pOrder->OrderSubmitStatus == THOST_FTDC_OSS_InsertSubmitted)		order.OrderSubmitStatus = "�Ѿ��ύ";
				else if (pOrder->OrderSubmitStatus == THOST_FTDC_OSS_CancelSubmitted)		order.OrderSubmitStatus = "�����Ѿ��ύ";
				else if (pOrder->OrderSubmitStatus == THOST_FTDC_OSS_ModifySubmitted)		order.OrderSubmitStatus = "�޸��Ѿ��ύ";
				else if (pOrder->OrderSubmitStatus == THOST_FTDC_OSS_Accepted)		order.OrderSubmitStatus = "�Ѿ�����";
				else if (pOrder->OrderSubmitStatus == THOST_FTDC_OSS_InsertRejected)		order.OrderSubmitStatus = "�����Ѿ����ܾ�";
				else if (pOrder->OrderSubmitStatus == THOST_FTDC_OSS_CancelRejected)		order.OrderSubmitStatus = "�����Ѿ����ܾ�";
				else order.OrderSubmitStatus = "�ĵ��Ѿ����ܾ�";		//������������,�ݲ�����

				if (pOrder->OrderStatus == THOST_FTDC_OST_AllTraded)		order.OrderStatus = "ȫ���ɽ�";
				else if (pOrder->OrderStatus == THOST_FTDC_OST_PartTradedQueueing)		order.OrderStatus = "���ֳɽ����ڶ�����";
				else if (pOrder->OrderStatus == THOST_FTDC_OST_PartTradedNotQueueing)		order.OrderStatus = "���ֳɽ����ڶ�����";
				else if (pOrder->OrderStatus == THOST_FTDC_OST_NoTradeQueueing)		order.OrderStatus = "δ�ɽ����ڶ�����";
				else if (pOrder->OrderStatus == THOST_FTDC_OST_NoTradeNotQueueing)		order.OrderStatus = "δ�ɽ����ڶ�����";
				else if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled)		order.OrderStatus = "����";
				else if (pOrder->OrderStatus == THOST_FTDC_OST_Unknown)		order.OrderStatus = "δ֪";
				else if (pOrder->OrderStatus == THOST_FTDC_OST_NotTouched)		order.OrderStatus = "��δ����";
				else order.OrderStatus = "�Ѵ���";		//������������,�ݲ�����

				OnOrder(order);
			}
		}
	}
	else if (wParam == ctp_msg::RtnTrade)
	{//�ɽ�����
		Trade *pTrade = reinterpret_cast<Trade*>(lParam);
		if (NULL != pTrade)
		{
			if (strcmp(pTrade->InstrumentID, instrument_id.c_str()) == 0)
			{
				static _Trade trade;
				trade.InstrumentID = pTrade->InstrumentID;
				trade.ExchangeID = pTrade->ExchangeID;
				trade.TradeID = pTrade->TradeID;
				trade.OrderSysID = pTrade->OrderSysID;
				trade.Price = pTrade->Price;
				trade.Volume = pTrade->Volume;
				trade.TradeDate = pTrade->TradeDate;
				trade.TradeTime = pTrade->TradeTime;
				trade.OrderRef = pTrade->OrderRef;

				if (pTrade->Direction == THOST_FTDC_D_Buy)	trade.Direction = "��";
				else trade.Direction = "��";

				if (pTrade->OffsetFlag == THOST_FTDC_OF_Open)	trade.OffsetFlag = "����";
				else if (pTrade->OffsetFlag == THOST_FTDC_OF_Close)	trade.OffsetFlag = "ƽ��";
				else if (pTrade->OffsetFlag == THOST_FTDC_OF_ForceClose)	trade.OffsetFlag = "ǿƽ";
				else if (pTrade->OffsetFlag == THOST_FTDC_OF_CloseToday)	trade.OffsetFlag = "ƽ��";
				else if (pTrade->OffsetFlag == THOST_FTDC_OF_CloseYesterday)	trade.OffsetFlag = "ƽ��";
				else if (pTrade->OffsetFlag == THOST_FTDC_OF_ForceOff)	trade.OffsetFlag = "ǿ��";
				else trade.OffsetFlag = "����ǿƽ";

				if (pTrade->HedgeFlag == THOST_FTDC_HF_Speculation)	trade.HedgeFlag = "Ͷ��";
				else if (pTrade->HedgeFlag == THOST_FTDC_HF_Arbitrage)	trade.HedgeFlag = "����";
				else if (pTrade->HedgeFlag == THOST_FTDC_HF_Hedge)	trade.HedgeFlag = "�ױ�";
				else trade.HedgeFlag = "������";

				OnTrade(trade);
			}
		}
	}
	else
	{
		//�������������ݲ�������
	}
}

void IStrategy::OnTick(_Tick& market_data)
{//���麯��Ӧ��������̳�����,��������ִ�в��Ե��߼��������ڴ˴�
}

void IStrategy::OnOrder(_Order& order)
{//���麯��Ӧ��������̳�����
}

void IStrategy::OnTrade(_Trade& trade)
{//���麯��Ӧ��������̳�����
}

void IStrategy::start()
{//ע��Ϊ������
	get_CtpMsgDistributor()->AddReceiver(this);
	StrategyStatus msg;
	msg.strategyName = this->StrategyName;
	msg.position_buy = this->position_buy;
	msg.position_sell = this->position_sell;
	msg.status = "������";
	msg.description = this->StrategyDescription;
	PostMessage_StrategyStatus(msg);

	StrategyMsg msg_strategy;
	msg_strategy.strategyName = this->StrategyName;
	msg_strategy.loggingLevel = ORDINARY;
	msg_strategy.loggingContent = "����������,����������/����/�ɽ���";
	PostMessage_StrategyMsg(msg_strategy);

	running = true; //�����������״̬
}

void IStrategy::stop()
{//�ӽ������������޳������ٽ������顢�������ɽ��ر���
	get_CtpMsgDistributor()->DelReceiver(this);
	StrategyStatus msg;
	msg.strategyName = this->StrategyName;
	msg.position_buy = -1;	//-1��ζ�Ÿ�ֵ������
	msg.position_sell = -1;	//-1��ζ�Ÿ�ֵ������
	msg.status = "��ֹͣ";
	msg.description = this->StrategyDescription;
	PostMessage_StrategyStatus(msg);

	StrategyMsg msg_strategy;
	msg_strategy.strategyName = this->StrategyName;
	msg_strategy.loggingLevel = ORDINARY;
	msg_strategy.loggingContent = "������ֹͣ";
	PostMessage_StrategyMsg(msg_strategy);

	running = false; //���ֹͣ����״̬
}

//���ز���״̬, ��״̬������start��stop���л�
bool IStrategy::isRunning()
{
	if (running==true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void IStrategy::set_ApiInterface(IService* pApiInterface)
{
	ApiInterface = pApiInterface;
}

bool IStrategy::PostMessage_StrategyStatus(StrategyStatus msg)
{//msg�ṹ�������п��ֶΣ�˵���˴���Ϣ����û�и��丳ֵ��������Ϣ�����жϲ��Թ����ֶ�
	static StrategyStatus content;
	content = msg;
	get_strategymsg_distributor()->OnResponse(WM_UPDATESTRATEGYSTATUS, (LPARAM)&content);
	return true;
}

bool IStrategy::PostMessage_StrategyMsg(StrategyMsg msg)
{
	static StrategyMsg content;
	content = msg;
	get_strategymsg_distributor()->OnResponse(WM_UPDATESTRATEGYMSG, (LPARAM)&content);
	return true;
}

//C++��string�ַ����ָ��
std::vector<std::string> IStrategy::split(std::string str, std::string pattern)
{
	//patternΪ�ָ��
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;//��չ�ַ����Է������
	int size = str.size();
	for (int i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}

bool IStrategy::make_order(const std::string& instrumentID, char Direction, char OpenCloseType, double price, int volume)
{//����
	InputOrderParam order_param;
	if (ApiInterface)
	{
		order_param.InstrumentID = instrumentID;
		order_param.Direction = Direction;
		order_param.CombOffsetFlag[0] = OpenCloseType;
		order_param.OrderPriceType = THOST_FTDC_OPT_LimitPrice;  //�۸�����Ĭ���Ƿ��޼۵������������֧��
		order_param.VolumeTotalOriginal = volume;
		order_param.LimitPrice = price;
		order_param.TimeCondition = THOST_FTDC_TC_GFD;
		order_param.OrderRef = this->StrategyName; //���Խӿڷ���Ҫ��OrderRef��ǰ׺, �����ֲ��Խӿ��µ����ֶ��µ�
		ApiInterface->request(ctp_msg::MakeOrder, &order_param);
	}
	return true;
}


bool IStrategy::cancel_order(const std::string& InstrumentID, const std::string& ExchangeID, const std::string& OrderSysID)
{//����
	ActionOrderParam action_order;
	memset(&action_order, 0, sizeof(action_order));
	if (ApiInterface)
	{
		strcpy_s(action_order.InstrumentID, InstrumentID.c_str());
		strcpy_s(action_order.ExchangeID, ExchangeID.c_str());
		strcpy_s(action_order.OrderSysID, OrderSysID.c_str());
		ApiInterface->request(ctp_msg::CancelOrder, &action_order);
	}
	return true;
}



//StrategyDistributor��ʵ�ֲ���
StrategyMsgDistributor::StrategyMsgDistributor()
{

}

StrategyMsgDistributor::~StrategyMsgDistributor()
{

}

void StrategyMsgDistributor::OnResponse(int msgID, LPARAM lParam)
{
	for (int i = 0; i < receivers.size(); ++i)
	{
		if (NULL != receivers[i])
		{//lParam����Ϣ�ṹ��ĵ�ַ
			receivers[i]->OnReceiveData(msgID, lParam);
		}
	}
}

void StrategyMsgDistributor::OnReceiveData(WPARAM wParam, LPARAM lParam)
{//��������
}

void StrategyMsgDistributor::AddReceiver(StrategyMsgDistributor* receiver)
{
	vector<StrategyMsgDistributor*>::iterator it = std::find(receivers.begin(), receivers.end(), receiver);
	if (it == receivers.end())
	{//�����ڳ�β������,˵��û�ҵ�,��ѹ�������������
		receivers.push_back(receiver);
	}
}

void StrategyMsgDistributor::DelReceiver(StrategyMsgDistributor* receiver)
{
	//��ĳ�������ߴ�vector���޳���ȥ
	for (vector<StrategyMsgDistributor*>::iterator it = receivers.begin(); it != receivers.end(); ++it)
	{
		if (*it == receiver)//����ַ��ͬ
		{
			it = receivers.erase(it);//����ָ���ĵ�����,ɾ��ָ��������
			break;
		}
	}
}

StrategyMsgDistributor* get_strategymsg_distributor()
{
	static StrategyMsgDistributor* pDistributor = NULL;
	if (NULL == pDistributor)
	{
		pDistributor = new StrategyMsgDistributor();
	}
	return pDistributor;
}