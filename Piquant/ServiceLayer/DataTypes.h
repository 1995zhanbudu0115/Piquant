#ifndef _DATA_TYPES_H_
#define _DATA_TYPES_H_
#include <string>
#include "IService.h"
using namespace std;


/*
	��β���
*/
struct RemoveParkedOrderActionParam
{
	///���͹�˾����
	string BrokerID;
	///Ͷ���ߴ���
	string InvestorID;
	///Ԥ�񳷵����
	string ParkedOrderActionID;
	///Ͷ�ʵ�Ԫ����
	string InvestUnitID;
};

struct QryParkedOrderActionParam
{
	///���͹�˾����
	string BrokerID;
	///Ͷ���ߴ���
	string InvestorID;
	///��Լ����
	string InstrumentID;
	///����������
	string ExchangeID;
	///Ͷ�ʵ�Ԫ����
	string InvestUnitID;
};

struct ParkedOrderActionParam
{
	///���͹�˾����
	string BrokerID;
	///Ͷ���ߴ���
	string InvestorID;
	///������������
	int OrderActionRef;
	///��������
	string OrderRef;
	///������
	int RequestID;
	///ǰ�ñ��
	int FrontID;
	///�Ự���
	int SessionID;
	///����������
	string ExchangeID;
	///�������
	string OrderSysID;
	///������־
	char ActionFlag;
	///�۸�
	double LimitPrice;
	///�����仯
	int VolumeChange;
	///�û�����
	string UserID;
	///��Լ����
	string InstrumentID;
	///Ԥ�񳷵������
	string ParkedOrderActionID;
	///�û�����
	char UserType;
	///Ԥ�񳷵�״̬
	char Status;
	///�������
	int ErrorID;
	///������Ϣ
	string ErrorMsg;
	///Ͷ�ʵ�Ԫ����
	string InvestUnitID;
	///IP��ַ
	string IPAddress;
	///Mac��ַ
	string MacAddress;
};

struct RemoveParkedOrderParam
{
	///���͹�˾����
	string BrokerID;
	///Ͷ���ߴ���
	string InvestorID;
	///Ԥ�񱨵����
	string ParkedOrderID;
	///Ͷ�ʵ�Ԫ����
	string InvestUnitID;
};

struct QryParkedOrderParam
{
	///���͹�˾����
	string BrokerID;
	///Ͷ���ߴ���
	string InvestorID;
	///��Լ����
	string InstrumentID;
	///����������
	string ExchangeID;
	///Ͷ�ʵ�Ԫ����
	string InvestUnitID;
};

struct InputParkedOrderParam
{
	///���͹�˾����
	string	BrokerID;
	///Ͷ���ߴ���
	string	InvestorID;
	///��Լ����
	string	InstrumentID;
	///��������
	string	OrderRef;
	///�û�����
	string	UserID;
	///�����۸�����
	char	OrderPriceType;
	///��������
	char	Direction;
	///��Ͽ�ƽ��־
	char	CombOffsetFlag[5];
	///���Ͷ���ױ���־
	char	CombHedgeFlag[5];
	///�۸�
	double	LimitPrice;
	///����
	int	VolumeTotalOriginal;
	///��Ч������
	char	TimeCondition;
	///GTD����
	string	GTDDate;
	///�ɽ�������
	char	VolumeCondition;
	///��С�ɽ���
	int	MinVolume;
	///��������
	char	ContingentCondition;
	///ֹ���
	double	StopPrice;
	///ǿƽԭ��
	char	ForceCloseReason;//��ͨ�û��µ���дTHOST_FTDC_FCC_NotForceClose ��ǿƽ
	///�Զ������־
	int	IsAutoSuspend;
	///ҵ��Ԫ
	string	BusinessUnit;
	///������
	int	RequestID;
	///�û�ǿ����־
	int	UserForceClose;
	///��������־
	int	IsSwapOrder;//�ݲ�֧�ֻ���������0
	///����������
	string	ExchangeID;
	///Ͷ�ʵ�Ԫ����
	string	InvestUnitID;
	///�ʽ��˺�
	string	AccountID;
	///���ִ���
	string	CurrencyID;
	///���ױ���
	string	ClientID;
	///IP��ַ
	string	IPAddress;
	///Mac��ַ
	string	MacAddress;
	///Ԥ�񱨵����
	string ParkedOrderID;
	///�û�����
	char UserType;
	///Ԥ��״̬
	char Status;
	///�������
	int ErrorID;
	///������Ϣ
	string ErrorMsg;
};

struct QryTradingNoticeParam
{
	///���͹�˾����
	string BrokerID;
	///Ͷ���ߴ���
	string InvestorID;
	///Ͷ�ʵ�Ԫ����
	string InvestUnitID;
};

struct QryNoticeParam
{
	///���͹�˾����
	string BrokerID;
};

struct BatchOrderActionParam
{
	///���͹�˾����
	string BrokerID;
	///Ͷ���ߴ���
	string InvestorID;
	///������������
	int OrderActionRef;
	///������
	int RequestID;
	///ǰ�ñ��
	int FrontID;
	///�Ự���
	int SessionID;
	///����������
	string ExchangeID;
	///�û�����
	string UserID;
	///Ͷ�ʵ�Ԫ����
	string InvestUnitID;
	///IP��ַ
	string IPAddress;
	///Mac��ַ
	string MacAddress;
};

struct QryInvestorPositionDetailParam
{
	///���͹�˾����
	string BrokerID;
	///Ͷ���ߴ���
	string InvestorID;
	///��Լ����
	string InstrumentID;
	///����������
	string ExchangeID;
	///Ͷ�ʵ�Ԫ����
	string InvestUnitID;
};

struct QryDepthMarketData��aram
{
	///��Լ����
	string InstrumentID;
	///����������
	string ExchangeID;
};

struct QryProductParam
{
	///��Ʒ����
	string ProductID;
	///��Ʒ����
	char ProductClass;
	///����������
	string ExchangeID;
};

struct QryInstrumentOrderCommRateParam
{
	///���͹�˾����
	string BrokerID;
	///Ͷ���ߴ���
	string InvestorID;
	///��Լ����
	string InstrumentID;
};

struct QryExchangeRateParam
{
	///���͹�˾����
	string BrokerID;
	///Դ����
	string FromCurrencyID;
	///Ŀ�����
	string ToCurrencyID;
};

struct QryInstrumentMarginRateParam
{
	///���͹�˾����
	string BrokerID;
	///Ͷ���ߴ���
	string InvestorID;
	///��Լ����
	string InstrumentID;
	///Ͷ���ױ���־
	char HedgeFlag;
	///����������
	string ExchangeID;
	///Ͷ�ʵ�Ԫ����
	string InvestUnitID;
};

struct QryOrderParam
{
	///���͹�˾����
	string BrokerID;
	///Ͷ���ߴ���
	string InvestorID; 
	///��Լ����
	string InstrumentID;
	///����������
	string ExchangeID;
	///�������
	string OrderSysID;
	///��ʼʱ��
	string InsertTimeStart;
	///����ʱ��
	string InsertTimeEnd;
	///Ͷ�ʵ�Ԫ����
	string InvestUnitID;
};


struct InstrumentCommissionRateParam
{
	///���͹�˾����
	char	BrokerID[11];
	///Ͷ���ߴ���
	char	InvestorID[13];
	///��Լ����
	char	InstrumentID[31];
	///����������
	char	ExchangeID[9];
	///Ͷ�ʵ�Ԫ����
	char	InvestUnitID[17];
};

struct BankFutureTransParam
{
	///ҵ������
	char	TradeCode[7];
	///���д���
	char	BankID[4];
	///���з�֧��������
	char	BankBranchID[5];
	///���̴���
	char	BrokerID[11];
	///���̷�֧��������
	char	BrokerBranchID[31];
	///��������
	char	TradeDate[9];
	///����ʱ��
	char	TradeTime[9];
	///������ˮ��
	char	BankSerial[13];
	///����ϵͳ���� 
	char	TradingDay[9];
	///����ƽ̨��Ϣ��ˮ��
	int	PlateSerial;
	///����Ƭ��־
	char	LastFragment;
	///�Ự��
	int	SessionID;
	///�ͻ�����
	char	CustomerName[51];
	///֤������
	char	IdCardType;
	///֤������
	char	IdentifiedCardNo[51];
	///�ͻ�����
	char	CustType;
	///�����ʺ�
	char	BankAccount[41];
	///��������
	char	BankPassWord[41];
	///Ͷ�����ʺ�
	char	AccountID[13];
	///�ڻ�����
	char	Password[41];
	///��װ���
	int	InstallID;
	///�ڻ���˾��ˮ��
	int	FutureSerial;
	///�û���ʶ
	char	UserID[16];
	///��֤�ͻ�֤�������־
	char	VerifyCertNoFlag;
	///���ִ���
	char	CurrencyID[4];
	///ת�ʽ��
	double	TradeAmount;
	///�ڻ���ȡ���
	double	FutureFetchAmount;
	///����֧����־
	char	FeePayFlag;
	///Ӧ�տͻ�����
	double	CustFee;
	///Ӧ���ڻ���˾����
	double	BrokerFee;
	///���ͷ������շ�����Ϣ
	char	Message[129];
	///ժҪ
	char	Digest[36];
	///�����ʺ�����
	char	BankAccType;
	///������־
	char	DeviceID[3];
	///�ڻ���λ�ʺ�����
	char	BankSecuAccType;
	///�ڻ���˾���б���
	char	BrokerIDByBank[33];
	///�ڻ���λ�ʺ�
	char	BankSecuAcc[41];
	///���������־
	char	BankPwdFlag;
	///�ڻ��ʽ�����˶Ա�־
	char	SecuPwdFlag;
	///���׹�Ա
	char	OperNo[17];
	///������
	int	RequestID;
	///����ID
	int	TID;
	///ת�˽���״̬
	char	TransferStatus;
	///���ͻ�����
	char	LongCustomerName[161];
};

struct PositionParam
{
	int PositionType;//���ͣ���ѯ���ǳֲ֣��ֲ���ϸ������ϳֲ�
	///���͹�˾����
	char	BrokerID[11];
	///Ͷ���ߴ���
	char	InvestorID[13];
	///��Լ����
	char	InstrumentID[31];
	///����������
	char	ExchangeID[9];
	///Ͷ�ʵ�Ԫ����
	char InvestUnitID[17];
};

struct TradeAccountParam
{
	///���͹�˾����
	char	BrokerID[11];
	///Ͷ���ߴ���
	char	InvestorID[11];
	///���ִ���
	char	CurrencyID[4];
	///ҵ������
	char BizType;
	///Ͷ�����ʺ�
	char	AccountID[13];
};

struct TradeParam
{
	///���͹�˾����
	string BrokerID;
	///Ͷ���ߴ���
	string InvestorID;
	///��Լ����
	string InstrumentID;
	///����������
	string ExchangeID;
	///�ɽ����
	string TradeID;
	///��ʼʱ��
	string TradeTimeStart;
	///����ʱ��
	string TradeTimeEnd;
	///Ͷ�ʵ�Ԫ����
	string InvestUnitID;
};

struct ActionOrderParam
{
	///���͹�˾����
	char	BrokerID[11];
	///Ͷ���ߴ���
	char	InvestorID[13];
	///������������
	int	OrderActionRef;
	///��������
	char	OrderRef[13];
	///������
	int	RequestID;
	///ǰ�ñ��
	int	FrontID;
	///�Ự���
	int	SessionID;
	///����������
	char	ExchangeID[9];
	///�������
	char	OrderSysID[21];
	///������־
	char	ActionFlag;
	///�۸�
	double	LimitPrice;
	///�����仯
	int	VolumeChange;
	///�û�����
	char	UserID[16];
	///��Լ����
	char	InstrumentID[31];
	///Ͷ�ʵ�Ԫ����
	char	InvestUnitID[17];
	///IP��ַ
	char	IPAddress[16];
	///Mac��ַ
	char	MacAddress[21];
};

struct InputOrderParam
{
	///���͹�˾����
	string	BrokerID;
	///Ͷ���ߴ���
	string	InvestorID;
	///��Լ����
	string	InstrumentID;
	///��������
	string	OrderRef;
	///�û�����
	string	UserID;
	///�����۸�����
	char	OrderPriceType;
	///��������
	char	Direction;
	///��Ͽ�ƽ��־
	char	CombOffsetFlag[5];
	///���Ͷ���ױ���־
	char	CombHedgeFlag[5];
	///�۸�
	double	LimitPrice;
	///����
	int	VolumeTotalOriginal;
	///��Ч������
	char	TimeCondition;
	///GTD����
	string	GTDDate;
	///�ɽ�������
	char	VolumeCondition;
	///��С�ɽ���
	int	MinVolume;
	///��������
	char	ContingentCondition;
	///ֹ���
	double	StopPrice;
	///ǿƽԭ��
	char	ForceCloseReason;//��ͨ�û��µ���дTHOST_FTDC_FCC_NotForceClose ��ǿƽ
	///�Զ������־
	int	IsAutoSuspend;
	///ҵ��Ԫ
	string	BusinessUnit;
	///������
	int	RequestID;
	///�û�ǿ����־
	int	UserForceClose;
	///��������־
	int	IsSwapOrder;//�ݲ�֧�ֻ���������0
	///����������
	string	ExchangeID;
	///Ͷ�ʵ�Ԫ����
	string	InvestUnitID;
	///�ʽ��˺�
	string	AccountID;
	///���ִ���
	string	CurrencyID;
	///���ױ���
	string	ClientID;
	///IP��ַ
	string	IPAddress;
	///Mac��ַ
	string	MacAddress;
};

struct UpdatePasswordParam
{
	int PasswordType;//���ͣ����ʽ��˻����ǽ����˻��������
	///���͹�˾����
	string	BrokerID;
	///Ͷ�����ʺ�
	string	AccountID;
	///ԭ���Ŀ���
	string	OldPassword;
	///�µĿ���
	string	NewPassword;
	///���ִ���
	string	CurrencyID;
};


/*
	����ֵ
*/
struct  UserInfo
{//�û���Ϣ
	///������
	char	TradingDay[9];
	///��¼�ɹ�ʱ��
	char	LoginTime[9];
	///���͹�˾����
	char	BrokerID[11];
	///�û�����
	char	UserID[16];
	///����ϵͳ����
	char	SystemName[41];
	///ǰ�ñ��
	int	FrontID;
	///�Ự���
	int	SessionID;
	///��󱨵�����
	char	MaxOrderRef[13];
	///������ʱ��
	char	ExchangeTime[41];
};

struct MarketData
{//�Զ����������ṹ��
	int trade_day;
	///��Լ����
	char	code[31];
	///����������
	char 	ExchangeID[9];
	///������
	char	TradingDay[9];
	///���¼�
	double	LastPrice;
	///����
	double	OpenPrice;
	///��߼�
	double	HighestPrice;
	///��ͼ�
	double	LowestPrice;
	///������
	double	ClosePrice;
	///����
	int	Volume;
	///�ɽ����
	double	Turnover;
	///�ϴν����
	double	PreSettlementPrice;
	///���ν����
	double	SettlementPrice;
	///������
	double	PreClosePrice;
	///��ֲ���
	double	PreOpenInterest;
	///�ֲ���
	double	OpenInterest;
	///����޸�ʱ��
	char	UpdateTime[9];
	///����޸ĺ���
	int	UpdateMillisec;
	///����һ��
	double	BidPrice;
	///����һ��
	int	BidVolume;
	///����һ��
	double AskPrice;
	///����һ��
	int	AskVolume;
	///���վ���
	double	AveragePrice;
	///��ͣ���
	double	UpperLimitPrice;
	///��ͣ���
	double	LowerLimitPrice;
	///����ʵ��
	//TThostFtdcRatioType	PreDelta;
	///����ʵ��
	//TThostFtdcRatioType	CurrDelta;
};

struct BaseData
{//��Լ�������ݽṹ
	///��Լ����
	char	code[31];
	///����������
	char 	ExchangeID[9];
	///��Լ����
	char	InstrumentName[21];
	///��Ʒ����
	char	ProductID[33];
	///��Ʒ����
	char	ProductClass;
	///�м۵�����µ���
	int	MaxMarketOrderVolume;
	///�м۵���С�µ���
	int 	MinMarketOrderVolume;
	///�޼۵�����µ���
	int	MaxLimitOrderVolume;
	///�޼۵���С�µ���
	int    MinLimitOrderVolume;
	///��Լ��������
	int	VolumeMultiple;
	///��С�䶯��λ
	double	PriceTick;
	///������
	char	CreateDate[9];
	///������
	char	OpenDate[9];
	///������
	char	ExpireDate[9];
	///��ʼ������
	char	StartDelivDate[9];
	///����������
	char	EndDelivDate[9];
	///��ǰ�Ƿ���
	int	IsTrading;
	///�ֲ�����
	char	PositionType;
	///��ͷ��֤����
	double	LongMarginRatio;
	///��ͷ��֤����
	double	ShortMarginRatio;
	///��Լ�ڽ������Ĵ���
	char	ExchangeInstID[31];
	///�������
	int	DeliveryYear;
	///������
	int	DeliveryMonth;
	///��Լ��������״̬
	char	InstLifePhase;
	///�ֲ���������
	char	PositionDateType;
	///�Ƿ�ʹ�ô��߱�֤���㷨
	char	MaxMarginSideAlgorithm;
	///������Ʒ����
	char	UnderlyingInstrID[31];
	///ִ�м�
	double	StrikePrice;
	///��Ȩ����
	char	OptionsType;
	///��Լ������Ʒ����
	double	UnderlyingMultiple;
	///�������
	char	CombinationType;
};

//���صĳֲ�����
struct InvestorPosition
{
	///��Լ����
	string InstrumentID;
	///���͹�˾����
	string BrokerID;
	///Ͷ���ߴ���
	string InvestorID;
	///�ֲֶ�շ���
	char PosiDirection;
	///Ͷ���ױ���־
	char HedgeFlag;
	///�ֲ���������
	char PositionDate;
	///���ճֲ�
	int YdPosition;
	///���ճֲ�
	int Position;
	///��ͷ����
	int LongFrozen;
	///��ͷ����
	int ShortFrozen;
	///���ֶ�����
	double LongFrozenAmount;
	///���ֶ�����
	double ShortFrozenAmount;
	///������
	int OpenVolume;
	///ƽ����
	int CloseVolume;
	///���ֽ��
	double OpenAmount;
	///ƽ�ֽ��
	double CloseAmount;
	///�ֲֳɱ�
	double PositionCost;
	///�ϴ�ռ�õı�֤��
	double PreMargin;
	///ռ�õı�֤��
	double UseMargin;
	///����ı�֤��
	double FrozenMargin;
	///������ʽ�
	double FrozenCash;
	///�����������
	double FrozenCommission;
	///�ʽ���
	double CashIn;
	///������
	double Commission;
	///ƽ��ӯ��
	double CloseProfit;
	///�ֲ�ӯ��
	double PositionProfit;
	///�ϴν����
	double PreSettlementPrice;
	///���ν����
	double SettlementPrice;
	///������
	string TradingDay;
	///������
	int SettlementID;
	///���ֳɱ�
	double OpenCost;
	///��������֤��
	double ExchangeMargin;
	///��ϳɽ��γɵĳֲ�
	int CombPosition;
	///��϶�ͷ����
	int CombLongFrozen;
	///��Ͽ�ͷ����
	int CombShortFrozen;
	///���ն���ƽ��ӯ��
	double CloseProfitByDate;
	///��ʶԳ�ƽ��ӯ��
	double CloseProfitByTrade;
	///���ճֲ�
	int TodayPosition;
	///��֤����
	double MarginRateByMoney;
	///��֤����(������)
	double MarginRateByVolume;
	///ִ�ж���
	int StrikeFrozen;
	///ִ�ж�����
	double StrikeFrozenAmount;
	///����ִ�ж���
	int AbandonFrozen;
	///����������
	string ExchangeID;
	///ִ�ж�������
	int YdStrikeFrozen;
	///Ͷ�ʵ�Ԫ����
	string InvestUnitID;
};

struct Order
{
	///���͹�˾����
	char BrokerID[11];
	///Ͷ���ߴ���
	char InvestorID[13];
	///��Լ����
	char InstrumentID[31];
	///��������
	char OrderRef[13];
	///�û�����
	char UserID[16];
	///�����۸�����
	char OrderPriceType;
	///��������
	char Direction;
	///��Ͽ�ƽ��־
	char CombOffsetFlag[5];
	///���Ͷ���ױ���־
	char CombHedgeFlag[5];
	///�۸�
	double LimitPrice;
	///����
	int VolumeTotalOriginal;
	///��Ч������
	char TimeCondition;
	///GTD����
	char GTDDate[9];
	///�ɽ�������
	char VolumeCondition;
	///��С�ɽ���
	int MinVolume;
	///��������
	char ContingentCondition;
	///ֹ���
	double StopPrice;
	///ǿƽԭ��
	char ForceCloseReason;
	///�Զ������־
	int IsAutoSuspend;
	///ҵ��Ԫ
	char BusinessUnit[21];
	///������
	int RequestID;
	///���ر������
	char OrderLocalID[13];
	///����������
	char ExchangeID[9];
	///��Ա����
	char ParticipantID[11];
	///�ͻ�����
	char ClientID[11];
	///��Լ�ڽ������Ĵ���
	char ExchangeInstID[31];
	///����������Ա����
	char TraderID[21];
	///��װ���
	int InstallID;
	///�����ύ״̬
	char OrderSubmitStatus;
	///������ʾ���
	int NotifySequence;
	///������
	char TradingDay[9];
	///������
	int SettlementID;
	///�������
	char OrderSysID[21];
	///������Դ
	char OrderSource;
	///����״̬
	char OrderStatus;
	///��������
	char OrderType;
	///��ɽ�����
	int VolumeTraded;
	///ʣ������
	int VolumeTotal;
	///��������
	char InsertDate[9];
	///ί��ʱ��
	char InsertTime[9];
	///����ʱ��
	char ActiveTime[9];
	///����ʱ��
	char SuspendTime[9];
	///����޸�ʱ��
	char UpdateTime[9];
	///����ʱ��
	char CancelTime[9];
	///����޸Ľ���������Ա����
	char ActiveTraderID[21];
	///�����Ա���
	char ClearingPartID[11];
	///���
	int SequenceNo;
	///ǰ�ñ��
	int FrontID;
	///�Ự���
	int SessionID;
	///�û��˲�Ʒ��Ϣ
	char UserProductInfo[11];
	///״̬��Ϣ
	char StatusMsg[81];
	///�û�ǿ����־
	int UserForceClose;
	///�����û�����
	char ActiveUserID[16];
	///���͹�˾�������
	int BrokerOrderSeq;
	///��ر���
	char RelativeOrderSysID[21];
	///֣�����ɽ�����
	int ZCETotalTradedVolume;
	///��������־
	int IsSwapOrder;
	///Ӫҵ�����
	char BranchID[9];
	///Ͷ�ʵ�Ԫ����
	char InvestUnitID[17];
	///�ʽ��˺�
	char AccountID[13];
	///���ִ���
	char CurrencyID[4];
	///IP��ַ
	char IPAddress[16];
	///Mac��ַ
	char MacAddress[21];
};

struct Trade
{
	///���͹�˾����
	char BrokerID[11];
	///Ͷ���ߴ���
	char InvestorID[13];
	///��Լ����
	char InstrumentID[31];
	///��������
	char OrderRef[13];
	///�û�����
	char UserID[16];
	///����������
	char ExchangeID[9];
	///�ɽ����
	char TradeID[21];
	///��������
	char Direction;
	///�������
	char OrderSysID[21];
	///��Ա����
	char ParticipantID[11];
	///�ͻ�����
	char ClientID[11];
	///���׽�ɫ
	char TradingRole;
	///��Լ�ڽ������Ĵ���
	char ExchangeInstID[31];
	///��ƽ��־
	char OffsetFlag;
	///Ͷ���ױ���־
	char HedgeFlag;
	///�۸�
	double Price;
	///����
	int Volume;
	///�ɽ�ʱ��
	char TradeDate[9];
	///�ɽ�ʱ��
	char TradeTime[9];
	///�ɽ�����
	char TradeType;
	///�ɽ�����Դ
	char PriceSource;
	///����������Ա����
	char TraderID[21];
	///���ر������
	char OrderLocalID[13];
	///�����Ա���
	char ClearingPartID[11];
	///ҵ��Ԫ
	char BusinessUnit[21];
	///���
	int SequenceNo;
	///������
	char TradingDay[9];
	///������
	int SettlementID;
	///���͹�˾�������
	int BrokerOrderSeq;
	///�ɽ���Դ
	char TradeSource;
	///Ͷ�ʵ�Ԫ����
	char InvestUnitID[17];
};

///��Լ��������
struct InstrumentCommissionRate
{
	///��Լ����
	char InstrumentID[31];
	///Ͷ���߷�Χ
	char InvestorRange;
	///���͹�˾����
	char BrokerID[11];
	///Ͷ���ߴ���
	char InvestorID[13];
	///������������
	double OpenRatioByMoney;
	///����������
	double OpenRatioByVolume;
	///ƽ����������
	double CloseRatioByMoney;
	///ƽ��������
	double CloseRatioByVolume;
	///ƽ����������
	double CloseTodayRatioByMoney;
	///ƽ��������
	double CloseTodayRatioByVolume;
	///����������
	char ExchangeID[9];
	///ҵ������
	char BizType;
	///Ͷ�ʵ�Ԫ����
	char InvestUnitID[17];
};

struct MarginRate
{
	///��Լ����
	char InstrumentID[31];
	///Ͷ���߷�Χ
	char InvestorRange;
	///���͹�˾����
	char BrokerID[11];
	///Ͷ���ߴ���
	char InvestorID[13];
	///Ͷ���ױ���־
	char HedgeFlag;
	///��ͷ��֤����
	double LongMarginRatioByMoney;
	///��ͷ��֤���
	double LongMarginRatioByVolume;
	///��ͷ��֤����
	double ShortMarginRatioByMoney;
	///��ͷ��֤���
	double ShortMarginRatioByVolume;
	///�Ƿ���Խ�������ȡ
	int IsRelative;
	///����������
	char ExchangeID[9];
	///Ͷ�ʵ�Ԫ����
	char InvestUnitID[17];
};

struct _ExchangeRate
{
	///���͹�˾����
	char BrokerID[11];
	///Դ����
	char FromCurrencyID[4];
	///Դ���ֵ�λ����
	double FromCurrencyUnit;
	///Ŀ�����
	char ToCurrencyID[4];
	///����
	double ExchangeRate;
};

struct OrderCommRate
{
	///��Լ����
	char InstrumentID[31];
	///���͹�˾����
	char BrokerID[11];
	///����������
	double OrderCommByVolume;
	///����������
	double OrderActionCommByVolume;
};

struct Product_InvaildInstrument
{
	///��Ʒ����
	char ProductID[31];
	///��Ʒ����
	char ProductName[21];
	///����������
	char ExchangeID[9];
	///��Ʒ����
	char ProductClass;
	///��Լ��������
	int VolumeMultiple;
	///��С�䶯��λ
	double PriceTick;
	///�м۵�����µ���
	int MaxMarketOrderVolume;
	///�м۵���С�µ���
	int MinMarketOrderVolume;
	///�޼۵�����µ���
	int MaxLimitOrderVolume;
	///�޼۵���С�µ���
	int MinLimitOrderVolume;
	///�ֲ�����
	char PositionType;
	///�ֲ���������
	char PositionDateType;
	///ƽ�ִ�������
	char CloseDealType;
	///���ױ�������
	char TradeCurrencyID[4];
	///��Ѻ�ʽ���÷�Χ
	char MortgageFundUseRange;
	///��������Ʒ����
	char ExchangeProductID[31];
	///��Լ������Ʒ����
	double UnderlyingMultiple;
};

struct DepthMarketData
{
	///������
	char TradingDay[9];
	///��Լ����
	char InstrumentID[31];
	///����������
	char ExchangeID[9];
	///��Լ�ڽ������Ĵ���
	char ExchangeInstID[31];
	///���¼�
	double LastPrice;
	///�ϴν����
	double PreSettlementPrice;
	///������
	double PreClosePrice;
	///��ֲ���
	double PreOpenInterest;
	///����
	double OpenPrice;
	///��߼�
	double HighestPrice;
	///��ͼ�
	double LowestPrice;
	///����
	int Volume;
	///�ɽ����
	double Turnover;
	///�ֲ���
	double OpenInterest;
	///������
	double ClosePrice;
	///���ν����
	double SettlementPrice;
	///��ͣ���
	double UpperLimitPrice;
	///��ͣ���
	double LowerLimitPrice;
	///����ʵ��
	double PreDelta;
	///����ʵ��
	double CurrDelta;
	///����޸�ʱ��
	char UpdateTime[9];
	///����޸ĺ���
	int UpdateMillisec;
	///�����һ
	double BidPrice1;
	///������һ
	int BidVolume1;
	///������һ
	double AskPrice1;
	///������һ
	int AskVolume1;
	///���վ���
	double AveragePrice;
	///ҵ������
	char ActionDay[9];
};

struct InvestorPositionDetail
{
	///��Լ����
	char InstrumentID[31];
	///���͹�˾����
	char BrokerID[11];
	///Ͷ���ߴ���
	char InvestorID[13];
	///Ͷ���ױ���־
	char HedgeFlag;
	///����
	char Direction;
	///��������
	char OpenDate[9];
	///�ɽ����
	char TradeID[21];
	///����
	int Volume;
	///���ּ�
	double OpenPrice;
	///������
	char TradingDay[9];
	///������
	int SettlementID;
	///�ɽ�����
	char TradeType;
	///��Ϻ�Լ����
	char CombInstrumentID[31];
	///����������
	char ExchangeID[9];
	///���ն���ƽ��ӯ��
	double CloseProfitByDate;
	///��ʶԳ�ƽ��ӯ��
	double CloseProfitByTrade;
	///���ն��гֲ�ӯ��
	double PositionProfitByDate;
	///��ʶԳ�ֲ�ӯ��
	double PositionProfitByTrade;
	///Ͷ���߱�֤��
	double Margin;
	///��������֤��
	double ExchMargin;
	///��֤����
	double MarginRateByMoney;
	///��֤����(������)
	double MarginRateByVolume;
	///������
	double LastSettlementPrice;
	///�����
	double SettlementPrice;
	///ƽ����
	int CloseVolume;
	///ƽ�ֽ��
	double CloseAmount;
	///Ͷ�ʵ�Ԫ����
	char InvestUnitID[17];
};

struct Notice
{
	///���͹�˾����
	char BrokerID[11];
	///��Ϣ����
	char Content[501];
	///���͹�˾֪ͨ�������к�
	char SequenceLabel[2];
};

struct TradingNotice
{
	///���͹�˾����
	char BrokerID[11];
	///Ͷ���߷�Χ
	char InvestorRange;
	///Ͷ���ߴ���
	char InvestorID[13];
	///����ϵ�к�
	short SequenceSeries;
	///�û�����
	char UserID[16];
	///����ʱ��
	char SendTime[9];
	///���к�
	int SequenceNo;
	///��Ϣ����
	char FieldContent[501];
	///Ͷ�ʵ�Ԫ����
	char InvestUnitID[17];
};

///�ʽ��˻�
struct TradingAccount
{
	///���͹�˾����
	char	BrokerID[11];
	///Ͷ�����ʺ�
	char	AccountID[13];
	///�ϴ���Ѻ���
	double	PreMortgage;
	///�ϴ����ö��
	double	PreCredit;
	///�ϴδ���
	double	PreDeposit;
	///�ϴν���׼����
	double	PreBalance;
	///�ϴ�ռ�õı�֤��
	double	PreMargin;
	///��Ϣ����
	double	InterestBase;
	///��Ϣ����
	double	Interest;
	///�����
	double	Deposit;
	///������
	double	Withdraw;
	///����ı�֤��
	double	FrozenMargin;
	///������ʽ�
	double	FrozenCash;
	///�����������
	double	FrozenCommission;
	///��ǰ��֤���ܶ�
	double	CurrMargin;
	///�ʽ���
	double	CashIn;
	///������
	double	Commission;
	///ƽ��ӯ��
	double	CloseProfit;
	///�ֲ�ӯ��
	double	PositionProfit;
	///�ڻ�����׼����
	double	Balance;
	///�����ʽ�
	double	Available;
	///��ȡ�ʽ�
	double	WithdrawQuota;
	///����׼����
	double	Reserve;
	///������
	char	TradingDay[9];
	///������
	int	SettlementID;
	///���ö��
	double	Credit;
	///��Ѻ���
	double	Mortgage;
	///��������֤��
	double	ExchangeMargin;
	///Ͷ���߽��֤��
	double	DeliveryMargin;
	///���������֤��
	double	ExchangeDeliveryMargin;
	///�����ڻ�����׼����
	double	ReserveBalance;
	///���ִ���
	char	CurrencyID[4];
	///�ϴλ���������
	double	PreFundMortgageIn;
	///�ϴλ����ʳ����
	double	PreFundMortgageOut;
	///����������
	double	FundMortgageIn;
	///�����ʳ����
	double	FundMortgageOut;
	///������Ѻ���
	double	FundMortgageAvailable;
	///����Ѻ���ҽ��
	double	MortgageableFund;
	///�����Ʒռ�ñ�֤��
	double	SpecProductMargin;
	///�����Ʒ���ᱣ֤��
	double	SpecProductFrozenMargin;
	///�����Ʒ������
	double	SpecProductCommission;
	///�����Ʒ����������
	double	SpecProductFrozenCommission;
	///�����Ʒ�ֲ�ӯ��
	double	SpecProductPositionProfit;
	///�����Ʒƽ��ӯ��
	double	SpecProductCloseProfit;
	///���ݳֲ�ӯ���㷨����������Ʒ�ֲ�ӯ��
	double	SpecProductPositionProfitByAlg;
	///�����Ʒ��������֤��
	double	SpecProductExchangeMargin;
	///ҵ������
	char BizType;
	///��ʱ���㶳����
	double	FrozenSwap;
	///ʣ�໻����
	double	RemainSwap;
};

struct ParkedOrder
{
	///���͹�˾����
	char BrokerID[11];
	///Ͷ���ߴ���
	char InvestorID[13];
	///��Լ����
	char InstrumentID[31];
	///��������
	char OrderRef[13];
	///�û�����
	char UserID[16];
	///�����۸�����
	char OrderPriceType;
	///��������
	char Direction;
	///��Ͽ�ƽ��־
	char CombOffsetFlag[5];
	///���Ͷ���ױ���־
	char CombHedgeFlag[5];
	///�۸�
	double LimitPrice;
	///����
	int VolumeTotalOriginal;
	///��Ч������
	char TimeCondition;
	///GTD����
	char GTDDate[9];
	///�ɽ�������
	char VolumeCondition;
	///��С�ɽ���
	int MinVolume;
	///��������
	char ContingentCondition;
	///ֹ���
	double StopPrice;
	///ǿƽԭ��
	char ForceCloseReason;
	///�Զ������־
	int IsAutoSuspend;
	///ҵ��Ԫ
	char BusinessUnit[21];
	///������
	int RequestID;
	///����������
	char ExchangeID[9];
	///�û�ǿ����־
	int UserForceClose;
	///Ԥ�񱨵����
	char ParkedOrderID[13];
	///�û�����
	char UserType;
	///Ԥ��״̬
	char Status;
	///�������
	int ErrorID;
	///������Ϣ
	char ErrorMsg[81];
	///��������־
	int IsSwapOrder;
	///�ͻ�����
	char ClientID[11];
	///Ͷ�ʵ�Ԫ����
	char InvestUnitID[17];
	///�ʽ��˺�
	char AccountID[13];
	///���ִ���
	char CurrencyID[4];
	///IP��ַ
	char IPAddress[16];
	///Mac��ַ
	char MacAddress[21];
};

struct Remove_ParkedOrder
{
	///���͹�˾����
	char BrokerID[11];
	///Ͷ���ߴ���
	char InvestorID[13];
	///Ԥ�񱨵����
	char ParkedOrderID[13];
	///Ͷ�ʵ�Ԫ����
	char InvestUnitID[17];
};

struct ParkedOrderAction
{
	///���͹�˾����
	char BrokerID[11];
	///Ͷ���ߴ���
	char InvestorID[13];
	///������������
	int OrderActionRef;
	///��������
	char OrderRef[13];
	///������
	int RequestID;
	///ǰ�ñ��
	int FrontID;
	///�Ự���
	int SessionID;
	///����������
	char ExchangeID[9];
	///�������
	char OrderSysID[21];
	///������־
	char ActionFlag;
	///�۸�
	double LimitPrice;
	///�����仯
	int VolumeChange;
	///�û�����
	char UserID[16];
	///��Լ����
	char InstrumentID[31];
	///Ԥ�񳷵������
	char ParkedOrderActionID[13];
	///�û�����
	char UserType;
	///Ԥ�񳷵�״̬
	char Status;
	///�������
	int ErrorID;
	///������Ϣ
	char ErrorMsg[81];
	///Ͷ�ʵ�Ԫ����
	char InvestUnitID[17];
	///IP��ַ
	char IPAddress[16];
	///Mac��ַ
	char MacAddress[21];
};

struct Remove_ParkedOrderAction
{
	///���͹�˾����
	char BrokerID[11];
	///Ͷ���ߴ���
	char InvestorID[13];
	///Ԥ�񳷵����
	char ParkedOrderActionID[13];
	///Ͷ�ʵ�Ԫ����
	char InvestUnitID[17];
};

//��������Ϣ�ṹ��
struct Exchange
{
	///����������
	string	ExchangeID;
	///����������
	string	ExchangeName;
	///����������
	char	ExchangeProperty;
};

//������Ӧ��Ϣ
struct ErrorMessage
{
	///�������
	int	ErrorID;
	///������Ϣ
	string	ErrorMsg;
};



enum FIELD_ID_UserInfo
{
	FIELD_code = 100,
	FIELD_name = 101,
	FIELD_TradingDay = 102,
	FIELD_LoginTime = 103,
	FIELD_BrokerID = 104,
	FIELD_UserID = 105,
	FIELD_SystemName = 106,
	FIELD_FrontID = 107,
	FIELD_SessionID = 108,
	FIELD_MaxOrderRef = 109,
	FIELD_ExchangeTime = 110,
};

enum FIELD_ID_BaseData
{
	FIELD_Code = 100,
	FIELD_ExchangeID = 101,
	FIELD_InstrumentName = 102,
	FIELD_ExchangeInstID = 103,
	FIELD_ProductID = 104,
	FIELD_ProductClass = 105,
	FIELD_DeliveryYear = 106,
	FIELD_DeliveryMonth = 107,
	FIELD_MaxMarketOrderVolume = 108,
	FIELD_MinMarketOrderVolume = 109,
	FIELD_MaxLimitOrderVolume = 110,
	FIELD_MinLimitOrderVolume = 111,
	FIELD_VolumeMultiple = 112,
	FIELD_PriceTick = 113,
	FIELD_CreateDate = 114,
	FIELD_OpenDate = 115,
	FIELD_ExpireDate = 116,
	FIELD_StartDelivDate = 117,
	FIELD_EndDelivDate = 118,
	FIELD_InstLifePhase = 119,
	FIELD_IsTrading = 120,
	FIELD_PositionType = 121,
	FIELD_PositionDateType = 122,
	FIELD_LongMarginRatio = 123,
	FIELD_ShortMarginRatio = 124,
	FIELD_MaxMarginSideAlgorithm = 125,
	FIELD_UnderlyingInstrID = 126,
	FIELD_StrikePrice = 127,
	FIELD_OptionsType = 128,
	FIELD_UnderlyingMultiple = 129,
	FIELD_CombinationType = 130,
};

enum FIELD_ID_InvestorPosition
{
	FIELD_InstrumentID_Position = 100,
	FIELD_BrokerID_Position = 101,
	FIELD_InvestorID_Position = 102,
	FIELD_PosiDirection_Position = 103,
	FIELD_HedgeFlag_Position = 104,
	FIELD_PositionDate_Position = 105,
	FIELD_YdPosition_Position = 106,
	FIELD_Position_Position = 107,
	FIELD_LongFrozen_Position = 108,
	FIELD_ShortFrozen_Position = 109,
	FIELD_LongFrozenAmount_Position = 110,
	FIELD_ShortFrozenAmount_Position = 111,
	FIELD_OpenVolume_Position = 112,
	FIELD_CloseVolume_Position = 113,
	FIELD_OpenAmount_Position = 114,
	FIELD_CloseAmount_Position = 115,
	FIELD_PositionCost_Position = 116,
	FIELD_PreMargin_Position = 117,
	FIELD_UseMargin_Position = 118,
	FIELD_FrozenMargin_Position = 119,
	FIELD_FrozenCash_Position = 120,
	FIELD_FrozenCommission_Position = 121,
	FIELD_CashIn_Position = 122,
	FIELD_Commission_Position = 123,
	FIELD_CloseProfit_Position = 124,
	FIELD_PositionProfit_Position = 125,
	FIELD_PreSettlementPrice_Position = 126,
	FIELD_SettlementPrice_Position = 127,
	FIELD_TradingDay_Position = 128,
	FIELD_SettlementID_Position = 129,
	FIELD_OpenCost_Position = 130,
	FIELD_ExchangeMargin_Position = 131,
	FIELD_CombPosition_Position = 132,
	FIELD_CombLongFrozen_Position = 133,
	FIELD_CombShortFrozen_Position = 134,
	FIELD_CloseProfitByDate_Position = 135,
	FIELD_CloseProfitByTrade_Position = 136,
	FIELD_TodayPosition_Position = 137,
	FIELD_MarginRateByMoney_Position = 138,
	FIELD_MarginRateByVolume_Position = 139,
	FIELD_StrikeFrozen_Position = 140,
	FIELD_StrikeFrozenAmount_Position = 141,
	FIELD_AbandonFrozen_Position = 142,
	FIELD_ExchangeID_Position = 143,
	FIELD_YdStrikeFrozen_Position = 144,
	FIELD_InvestUnitID_Position = 145,
	FIELD_FloatProfit_Position = 146, //�Զ����
	FIELD_AvgPrice_Position = 147, //�Զ����
};

enum FIELD_ID_Order
{
	FIELD_BrokerID_Order = 100,
	FIELD_InvestorID_Order = 101,
	FIELD_InstrumentID_Order = 102,
	FIELD_OrderRef_Order = 103,
	FIELD_UserID_Order = 104,
	FIELD_OrderPriceType_Order = 105,
	FIELD_Direction_Order = 106,
	FIELD_CombOffsetFlag_Order = 107,
	FIELD_CombHedgeFlag_Order = 108,
	FIELD_LimitPrice_Order = 109,
	FIELD_VolumeTotalOriginal_Order = 110,
	FIELD_TimeCondition_Order = 111,
	FIELD_GTDDate_Order = 112,
	FIELD_VolumeCondition_Order = 113,
	FIELD_MinVolume_Order = 114,
	FIELD_ContingentCondition_Order = 115,
	FIELD_StopPrice_Order = 116,
	FIELD_ForceCloseReason_Order = 117,
	FIELD_IsAutoSuspend_Order = 118,
	FIELD_BusinessUnit_Order = 119,
	FIELD_RequestID_Order = 120,
	FIELD_OrderLocalID_Order = 121,
	FIELD_ExchangeID_Order = 122,
	FIELD_ParticipantID_Order = 123,
	FIELD_ClientID_Order = 124,
	FIELD_ExchangeInstID_Order = 125,
	FIELD_TraderID_Order = 126,
	FIELD_InstallID_Order = 127,
	FIELD_OrderSubmitStatus_Order = 128,
	FIELD_NotifySequence_Order = 129,
	FIELD_TradingDay_Order = 130,
	FIELD_SettlementID_Order = 131,
	FIELD_OrderSysID_Order = 132,
	FIELD_OrderSource_Order = 133,
	FIELD_OrderStatus_Order = 134,
	FIELD_OrderType_Order = 135,
	FIELD_VolumeTraded_Order = 136,
	FIELD_VolumeTotal_Order = 137,
	FIELD_InsertDate_Order = 138,
	FIELD_InsertTime_Order = 139,
	FIELD_ActiveTime_Order = 140,
	FIELD_SuspendTime_Order = 141,
	FIELD_UpdateTime_Order = 142,
	FIELD_CancelTime_Order = 143,
	FIELD_ActiveTraderID_Order = 144,
	FIELD_ClearingPartID_Order = 145,
	FIELD_SequenceNo_Order = 146,
	FIELD_FrontID_Order = 147,
	FIELD_SessionID_Order = 148,
	FIELD_UserProductInfo_Order = 149,
	FIELD_StatusMsg_Order = 150,
	FIELD_UserForceClose_Order = 151,
	FIELD_ActiveUserID_Order = 152,
	FIELD_BrokerOrderSeq_Order = 153,
	FIELD_RelativeOrderSysID_Order = 154,
	FIELD_ZCETotalTradedVolume_Order = 155,
	FIELD_IsSwapOrder_Order = 156,
	FIELD_BranchID_Order = 157,
	FIELD_InvestUnitID_Order = 158,
	FIELD_AccountID_Order = 159,
	FIELD_CurrencyID_Order = 160,
	FIELD_IPAddress_Order = 161,
	FIELD_MacAddress_Order = 162,
	FIELD_OrderLocalType_Order = 163,
};

enum FIELD_ID_Trade
{
	FIELD_BrokerID_Trade = 100,
	FIELD_InvestorID_Trade = 101,
	FIELD_InstrumentID_Trade = 102,
	FIELD_OrderRef_Trade = 103,
	FIELD_UserID_Trade = 104,
	FIELD_ExchangeID_Trade = 105,
	FIELD_TradeID_Trade = 106,
	FIELD_Direction_Trade = 107,
	FIELD_OrderSysID_Trade = 108,
	FIELD_ParticipantID_Trade = 109,
	FIELD_ClientID_Trade = 110,
	FIELD_TradingRole_Trade = 111,
	FIELD_ExchangeInstID_Trade = 112,
	FIELD_OffsetFlag_Trade = 113,
	FIELD_HedgeFlag_Trade = 114,
	FIELD_Price_Trade = 115,
	FIELD_Volume_Trade = 116,
	FIELD_TradeDate_Trade = 117,
	FIELD_TradeTime_Trade = 118,
	FIELD_TradeType_Trade = 119,
	FIELD_PriceSource_Trade = 120,
	FIELD_TraderID_Trade = 121,
	FIELD_OrderLocalID_Trade = 122,
	FIELD_ClearingPartID_Trade = 123,
	FIELD_BusinessUnit_Trade = 124,
	FIELD_SequenceNo_Trade = 125,
	FIELD_TradingDay_Trade = 126,
	FIELD_SettlementID_Trade = 127,
	FIELD_BrokerOrderSeq_Trade = 128,
	FIELD_TradeSource_Trade = 129,
	FIELD_InvestUnitID_Trade = 130,
};

enum FIELD_ID_InvestorPositionDetail
{
	FIELD_InstrumentID_PositionDetail = 100,
	FIELD_BrokerID_PositionDetail = 101,
	FIELD_InvestorID_PositionDetail = 102,
	FIELD_Direction_PositionDetail = 103,
	FIELD_HedgeFlag_PositionDetail = 104,
	FIELD_OpenDate_PositionDetail = 105,
	FIELD_TradeID_PositionDetail = 106,
	FIELD_Volume_PositionDetail = 107,
	FIELD_OpenPrice_PositionDetail = 108,
	FIELD_TradingDay_PositionDetail = 109,
	FIELD_SettlementID_PositionDetail = 110,
	FIELD_TradeType_PositionDetail = 111,
	FIELD_CombInstrumentID_PositionDetail = 112,
	FIELD_ExchangeID_PositionDetail = 113,
	FIELD_CloseProfitByDate_PositionDetail = 114,
	FIELD_CloseProfitByTrade_PositionDetail = 115,
	FIELD_PositionProfitByDate_PositionDetail = 116,
	FIELD_PositionProfitByTrade_PositionDetail = 117,
	FIELD_Margin_PositionDetail = 118,
	FIELD_ExchMargin_PositionDetail = 119,
	FIELD_MarginRateByMoney_PositionDetail = 120,
	FIELD_MarginRateByVolume_PositionDetail = 121,
	FIELD_LastSettlementPrice_PositionDetail = 122,
	FIELD_SettlementPrice_PositionDetail = 123,
	FIELD_CloseVolume_PositionDetail = 124,
	FIELD_CloseAmount_PositionDetail = 125,
	FIELD_InvestUnitID_PositionDetail = 126,
};


string SERVICE_API GetTextById(UserInfo& ui, int field_id);							//����ԭ������,SERVICE_API˵���ú�������Ҫ������
string SERVICE_API GetTextById(BaseData& bd, int field_id);
string SERVICE_API GetTextById(InvestorPosition& ip, int field_id);
string SERVICE_API GetTextById(Order& o, int field_id);
string SERVICE_API GetTextById(Trade& t, int field_id);
string SERVICE_API GetTextById(InvestorPositionDetail& ipd, int field_id);
#endif