#ifndef _CTP_SERVICE_H_
#define _CTP_SERVICE_H_
#include "IService.h"
#include "TradeSpi.h"
#include "MdSpi.h"
#include "DataTypes.h"

class TradeSpi;//ǰ������
class CMdSpi;

/*
	CtpService��UI�����Ψһ�ɵ��õ�����󣬲��ɸ������ȥ����TradeSpi* pCtpTradeApi����api����CMdSpi* pCtpMdApi����api����
	����֮����:
	UI�����ȫ�ֹ�����GlobalMgr->CtpService
	CtpService���������������󲿷�,���ں�pCtpTradeApi��pCtpMdApi����������Ա������ctp�Ļص�����
*/
class SERVICE_API CtpService : public IService
{
public:
	CtpService();
	virtual ~CtpService();
	virtual bool login(const string& user_name, const string& password, const ServerInfo& bs);
	virtual bool logout();
	virtual bool islogined();
	virtual bool request(int request_id, void* param);//ͨ�����󷽷�
	virtual bool get_basic_data(vector<BaseData>& base_data); //���ڻ�ȡȫ����Լ�Ļ�������
	virtual bool get_exchanges(vector<Exchange>& exchanges);//���ڻ�ȡȫ������������Ϣ����
	virtual bool get_all_InvestorPositionDetails(vector<InvestorPositionDetail>& positiondetails);//���ڻ�ȡȫ���ֲ���ϸ����
	virtual std::string get_username();
	virtual ServerInfo get_brokerinfo();

protected:
	//�޸Ľ����˻����ʽ��˻������룬�ڲ�����ReqUserPasswordUpdate���½����˻����롢ReqTradingAccountPasswordUpdate�����ʽ��˻�����
	bool update_password(UpdatePasswordParam& param);

	//�µ�,�ڲ�����ReqOrderInsert�������޼۵����м۵����������Ƚ�����֧�ֵ�ָ��
	bool make_order(InputOrderParam& param);

	//��������δ�ɽ�ǰ����,�ڲ�����ReqOrderAction
	bool cancel_order(ActionOrderParam& param);

	//��ѯ�˻��ʽ�,�ڲ�����ReqQryTradingAccount
	bool query_account(TradeAccountParam& param);

	//��ѯ����,�ڲ�����ReqQryDepthMarketData
	bool query_market_data(vector<string>& instrumentid = vector<string>());

	//��ѯ�ֲ֣��ڲ�����ReqQryInvestorPosition��ѯ�ֲ�
	bool query_position(PositionParam& param);

	//�����ڲ�����ReqQueryBankAccountMoneyByFuture
	bool query_money();

	//����ת�ˣ��ڲ�����ReqFromBankToFutureByFuture���������ʽ�ת�ڻ�����ReqFromFutureToBankByFuture�����ڻ��ʽ�ת��������
	bool Bank_Future_Transfer(BankFutureTransParam& param);

	//���Լ
	bool query_instruments(vector<string>& instrument_vector, bool qry_all=false);

	//��ѯ�ض�����������Ϣ,�β�Ϊ������ID
	bool query_exchanges(const string& eid);

	//���ض�Ʒ�ֵ�������,�ڲ�����ReqQryInstrumentCommissionRate
	bool query_CommissionRate(InstrumentCommissionRateParam& param);

	//��ѯί��,�ڲ�����ReqQryOrder
	bool query_Order(QryOrderParam& param);

	//��ѯ�ɽ�,�ڲ�����ReqQryTrade
	bool query_Trade(TradeParam& param);

	//��ѯ��֤����
	bool query_MarginRate(QryInstrumentMarginRateParam& param);

	//��ѯ���ʣ��ڲ�����ReqQryExchangeRate
	bool query_ExchangeRate(QryExchangeRateParam& param);

	//��ѯ����������,�ڲ�����ReqQryInstrumentOrderCommRate
	bool query_InstrumentOrderCommRate(QryInstrumentOrderCommRateParam& param);

	//��ѯ��Ʒ,�ڲ�����ReqQryProduct
	bool query_Product(QryProductParam& param);

	//��ѯ��Ʒ����
	bool query_DepthMarketData(QryDepthMarketData��aram& param);

	//��ֲ���ϸ
	bool query_InvestorPositionDetail(QryInvestorPositionDetailParam& param);

	//��������
	bool batchorder_action(BatchOrderActionParam& param);

	//��ѯ�����̿ͻ�֪ͨ
	bool query_notice(QryNoticeParam& param);

	//��ѯ����֪ͨ
	bool query_tradingnotice(QryTradingNoticeParam& param);

	//��Ԥ��,�ڲ�����ReqParkedOrderInsert
	bool make_parkedorder(InputParkedOrderParam& param);

	//��ѯԤ��
	bool query_parkedorder(QryParkedOrderParam& param);

	//Ԥ�񵥳���
	bool remove_parkedorder(RemoveParkedOrderParam& param);

	//Ԥ�񳷵�
	bool cancel_parkedorder(ParkedOrderActionParam& param);

	//��ѯԤ�񳷵�
	bool query_parkedorderaction(QryParkedOrderActionParam& param);

	//Ԥ�񳷵�����
	bool remove_parkedorderaction(RemoveParkedOrderActionParam& param);

protected:
	int T_iReqestID;
	TradeSpi* pCtpTradeApi; //����api����loginʱ����
	CMdSpi* pCtpMdApi;     //����api����loginʱ����
	string user_name;
	ServerInfo broker;
};


#endif
