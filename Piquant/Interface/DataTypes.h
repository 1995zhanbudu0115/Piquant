#ifndef _DATATYPES_H_
#define _DATATYPES_H_

#include "../../sdk/ThostFtdcUserApiDataType.h"
#include <string>
using namespace std;

///�������
struct _Tick
{
	///������
	string   TradingDay;
	///��Լ����
	string  InstrumentID;
	///����������
	string ExchangeID;
	///���¼�
	double  LastPrice;
	///�ϴν����
	double  PreSettlementPrice;
	///������
	double  PreClosePrice;
	///��ֲ���
	double   PreOpenInterest;
	///����
	double  OpenPrice;
	///��߼�
	double  HighestPrice;
	///��ͼ�
	double  LowestPrice;
	///����
	int Volume;
	///�ɽ����
	double  Turnover;
	///�ֲ���
	double   OpenInterest;
	///������
	double  ClosePrice;
	///���ν����
	double  SettlementPrice;
	///��ͣ���
	double  UpperLimitPrice;
	///��ͣ���
	double  LowerLimitPrice;
	///����޸�ʱ��
	string   UpdateTime;
	///����޸ĺ���
	int   UpdateMillisec;
	///�����һ
	double  BidPrice1;
	///������һ
	int BidVolume1;
	///������һ
	double  AskPrice1;
	///������һ
	int AskVolume1;
};

///����
struct _Order
{
	///��Լ����
	string InstrumentID;
	///����������
	string ExchangeID;
	///��������
	string OrderRef;
	///��������
	string InsertDate;
	///ί��ʱ��
	string InsertTime;
	///�����۸�����
	string OrderPriceType;
	///��������
	string Direction;
	///��Ͽ�ƽ��־
	string CombOffsetFlag;
	///�۸�
	double LimitPrice;
	///����
	int VolumeTotalOriginal;
	///��ɽ�����
	int VolumeTraded;
	///ʣ������
	int VolumeTotal;
	///��������
	string ContingentCondition;
	///������
	double StopPrice;
	///�����ύ״̬, Ϊ���������յĶ������б������,���۵��ı��������ctp��,���Ҫ����
	string OrderSubmitStatus;
	///����״̬
	string OrderStatus;
	///״̬��Ϣ
	string StatusMsg;
	///��������ɽ���������,��ǳ���Ҫ,ͨ����¼���ȶԱ������,����ȷ��ĳ���Ƿ�����Ӧ���Խӿڷ���
	string OrderSysID;
};

///�ɽ�
struct _Trade
{
	///��Լ����
	string InstrumentID;
	///����������
	string ExchangeID;
	///��������
	string OrderRef;
	///�ɽ����
	string TradeID;
	///��������
	string Direction;
	///������ž���Ψһ��,�ǳ���Ҫ
	string OrderSysID;
	///��ƽ��־
	string OffsetFlag;
	///Ͷ���ױ���־
	string HedgeFlag;
	///�۸�
	double Price;
	///����
	int  Volume;
	///�ɽ�ʱ��
	string TradeDate;
	///�ɽ�ʱ��
	string TradeTime;
};

#endif