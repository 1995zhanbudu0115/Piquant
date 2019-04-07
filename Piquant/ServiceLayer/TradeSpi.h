#ifndef _TRADE_SPI_H_
#define _TRADE_SPI_H_
#include "stdafx.h"
#include <tchar.h>//���ڴ���unicode�ַ����е�_T()��ͷ�ļ�
#include "../../sdk/ThostFtdcTraderApi.h"
#include "../../sdk/ThostFtdcUserApiDataType.h"
#include "../../sdk/ThostFtdcUserApiStruct.h"
#include "DataTypes.h"
#include "ServerInfo.h"
#include "MdSpi.h"

//TradeSpi�̳���CThostFtdcTraderSpi���������еĻص��麯���������أ�
class TradeSpi : public CThostFtdcTraderSpi
{
public:
	TradeSpi();
	virtual ~TradeSpi();

public://������������
	void Init(const ServerInfo& info);
	bool Login(const string& username, const string& password);
	CThostFtdcTraderApi* GetTraderApi();//��ȡm_pTradeApi�õ��ⲿ�ӿ�
	const ErrorMessage& LastError() const{ return last_error; }//��ȡlast_error�õ��ⲿ�ӿ�,Ϊ��������
	vector<BaseData>& get_all_instruments(){ return all_instruments; } //��ȡ��ѯ��Լ���ⲿ�ӿ�
	vector<Exchange>& get_all_exchanges(){ return all_exchanges; }	//��ȡ��ѯ���������ⲿ�ӿ�
	ServerInfo& get_brokerinfo(){ return m_BrokerInfo; } //��ȡ��������Ϣ���ⲿ�ӿ�
	string& get_username(){ return username; }//��ȡ�û������ⲿ�ӿ�
	UserInfo& get_userinfo(){ return user_info; }	//��ȡ��¼��Ϣ�ṹ��
	vector<InvestorPosition>& get_all_InvestorPositions(){ return all_InvestorPositions; } //��ȡ���гֲֵ��ⲿ�ӿ�
	vector<InvestorPositionDetail>& get_all_InvestorPositionDetails(){ return all_InvestorPositionsDetail; } //��ȡ���гֲ���ϸ���ⲿ�ӿ�

protected:
	bool IsErrorRspInfo(CThostFtdcRspInfoField* pRspInfo);//���ص��Ƿ񷵻ش����Դ���ȷ���������Ĳ����Ƿ�ɹ�,Ϊͨ���ͺ���
	void SetLastError(CThostFtdcRspInfoField *pRspInfo);

public:
	virtual void OnFrontConnected();
	virtual void OnFrontDisconnected(int nReason);
	virtual void OnHeartBeatWarning(int nTimeLapse);
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
	////Ͷ���߽�����ȷ����Ӧ
	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//����¼��������Ӧ��¼�����ʱ��Ӧ��Ӧ����ִ��ReqOrderInsert�����ֶ���д����֮���CTP������ͨ���˽ӿڷ���
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//��������������Ӧ
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//�����ѯͶ���ֲ߳���Ӧ
	virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//�����ѯ�ʽ��˻���Ӧ
	virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//�����ѯ��Լ��Ӧ
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//����Ӧ��
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//����֪ͨ,������ȷʱ��Ӧ
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);
	//�ɽ�֪ͨ��������ȷʱ��Ӧ
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);
	//�Ƿ�ȷ�Ϲ�������Ϣ�ķ���
	virtual void OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//��ѯ��������ķ���
	virtual void OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//��ѯĳ����Ʒ�ֵķ���
	virtual void OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//����¼��������Ӧ��¼�����ʱ��Ӧ��Ӧ
	virtual void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);
	//��ѯί����Ӧ
	virtual void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//��ѯ�ɽ���Ӧ
	virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//���ظ�����Ա�ĳ���������Ӧ��������
	virtual void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo);
	//��ѯ��֤�𷵻�
	virtual void OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//��ѯ���ʷ���
	virtual void OnRspQryExchangeRate(CThostFtdcExchangeRateField *pExchangeRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//��ѯ���������ѷ���
	virtual void OnRspQryInstrumentOrderCommRate(CThostFtdcInstrumentOrderCommRateField *pInstrumentOrderCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//��ѯ��Ʒ��Ч��Լ����
	virtual void OnRspQryProduct(CThostFtdcProductField *pProduct, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//��ѯ�������
	virtual void OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//��ѯ�ֲ���ϸ����
	virtual void OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//�����������󣬹�̨����
	virtual void OnRspBatchOrderAction(CThostFtdcInputBatchOrderActionField *pInputBatchOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//�����������󣬽���������
	virtual void OnErrRtnBatchOrderAction(CThostFtdcBatchOrderActionField *pBatchOrderAction, CThostFtdcRspInfoField *pRspInfo);
	//��ѯ�ڻ���˾�ͻ�֪ͨ
	virtual void OnRspQryNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//��ѯ����֪ͨ
	virtual void OnRspQryTradingNotice(CThostFtdcTradingNoticeField *pTradingNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//��Ԥ��ص�
	virtual void OnRspParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//��ѯԤ�񵥻ص�
	virtual void OnRspQryParkedOrder(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//Ԥ�񳷵�
	virtual void OnRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//Ԥ�񳷵��ص�
	virtual void OnRspParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//��ѯԤ�񳷵��ص�
	virtual void OnRspQryParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//Ԥ�񳷵�����
	virtual void OnRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//�ǳ��ص�
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);


protected:
	int m_iReqestID;//������
	CThostFtdcTraderApi* m_pTradeApi;
	bool m_bInited;//�Ƿ񱻳�ʼ�����ļ�¼����
	UserInfo user_info; //�����¼�󷵻صĵ�¼��Ϣ�ṹ��
	ServerInfo m_BrokerInfo;//�����͹�˾��Ϣ��������
	string username;
	string password;
	string tradingday;//���ڱȽ��Ƿ����ȷ�Ϲ�
	ErrorMessage last_error;//����Ĵ�����Ϣ�ṹ��
	vector<Exchange> all_exchanges;//���ڱ��潻������ϸ��Ϣ��vector����
	vector<BaseData> all_instruments; //���ڱ����Լ��vector����
	vector<InvestorPosition> all_InvestorPositions; //���ڱ���ֲֵ�vector����
	vector<InvestorPositionDetail> all_InvestorPositionsDetail; //���ڱ���ֲ���ϸ��vector����

	//�Ƿ��Ѳ�ѯ�������ڴ���������ѯ��������ѯ���ܻᴥ����һ�ֵı���������ѯ��ͨ����ǿ��Ʊ���������ѯ���жϣ���ֹ��Դ�˷�
	bool m_AlreadyQryExchange;
	bool m_AlreadyQryPosition;
	bool m_AlreadyQryCommissionRate;  
	bool m_AlreadyQryOrder; 
	bool m_AlreadyQryParkedOrder; 
	bool m_AlreadyQryParkedOrderAction; 
};
#endif // _TRADE_SPI_H_

