#include "stdafx.h"
#include "CtpService.h"
#include<ThostFtdcTraderApi.h>
#include<ThostFtdcUserApiDataType.h>
#include<ThostFtdcUserApiStruct.h>
#include <string>
#include "CtpMsg.h"
using namespace std;


CtpService::CtpService()
	:T_iReqestID(0),
	pCtpTradeApi(NULL),
	pCtpMdApi(NULL)
{

}


CtpService::~CtpService()
{
	if (pCtpTradeApi != NULL)
	{
		delete pCtpTradeApi;
		pCtpTradeApi = NULL;
	}
	if (pCtpMdApi != NULL)
	{
		delete pCtpMdApi;
		pCtpMdApi = NULL;
	}
}

bool CtpService::login(const string& user_name, const string& password, const ServerInfo& bs)
{//�ɵ�¼�Ի�����ã�ʵ�ֽ��׼������ǰ������
	if (user_name.empty() || password.empty() || bs.id.empty() || bs.protocol.empty() || bs.market_server_front.empty() || bs.trade_server_front.empty())
	{//��μ��
		return false;
	}
	/*��ʼ��ʼ��ǰ������*/
	if (NULL == pCtpTradeApi)
	{
		pCtpTradeApi = new TradeSpi();
	}
	pCtpTradeApi->Init(bs);	//������api,ע��spi��ע�ύ��ǰ�÷�������ַ���ѣ���δinit����

	if (NULL == pCtpMdApi)
	{
		pCtpMdApi = new CMdSpi();
	}
	pCtpMdApi->Init(bs);
	
	this->user_name = user_name;	//��¼��¼�˺�
	this->broker = bs;					//��¼��������Ϣ

	pCtpTradeApi->Login(user_name, password);	//���뽻��ǰ������Init
	pCtpMdApi->Login(user_name, password);		//��������ǰ������Init
	return true;
}

bool CtpService::logout()
{
	//CThostFtdcUserLogoutField req = { 0 };
	//strcpy_s(req.BrokerID, broker.id.c_str());
	//strcpy_s(req.UserID, user_name.c_str());
	if (pCtpMdApi == NULL || pCtpTradeApi == NULL)
	{//��δ��¼���ܵǳ������쳣���
		::MessageBox(NULL, _T("��δ��¼���ɵǳ�"), _T("����"), MB_OK);
		return false;
	}
	//******************************����API�˳���¼**************************************//
	//�˶����ĺ�Լ��
	if (pCtpMdApi != NULL)
	{
		vector<BaseData> all_instruments = pCtpTradeApi->get_all_instruments();
		vector<string> instruments_unsubscribe;
		for (int i = 0; i < all_instruments.size(); ++i)
		{
			instruments_unsubscribe.push_back(all_instruments[i].code);//��ȡbasedata�ṹ���еĺ�Լ���������ȡ
		}
		pCtpMdApi->UnSubscribeMarketData(instruments_unsubscribe);//�˶���Լ
	}
	//pCtpMdApi->GetMdApi()->ReqUserLogout(&req, 2);	//ctp���˳���¼��Щ���⣬ֱ�����ٶ������
	if (pCtpMdApi->GetMdApi() != NULL)
	{//��������api����
		pCtpMdApi->GetMdApi()->RegisterSpi(NULL);
		pCtpMdApi->GetMdApi()->Release();
		CThostFtdcMdApi* pMdapi = pCtpMdApi->GetMdApi();
		pMdapi = NULL;
	}
	if (pCtpMdApi != NULL)
	{//��������ص�spi����
		delete pCtpMdApi;
		pCtpMdApi = NULL;
	}
	//*****************************����API�˳���¼**************************************//
	//pCtpTradeApi->GetTraderApi()->ReqUserLogout(&req, 1);//�˴���nRequestID�޵�������,����ά��,ctp���˳���¼��Щ���⣬ֱ�����ٶ������
	if (pCtpTradeApi->GetTraderApi() != NULL)
	{//������api����
		pCtpTradeApi->GetTraderApi()->RegisterSpi(NULL);
		pCtpTradeApi->GetTraderApi()->Release();
		CThostFtdcTraderApi* pTdapi = pCtpTradeApi->GetTraderApi();
		pTdapi = NULL;
	}
	if (pCtpTradeApi != NULL)
	{//��������ص�spi����
		delete pCtpTradeApi;
		pCtpTradeApi = NULL;
	}
	return(true);
}

bool CtpService::get_basic_data(vector<BaseData>& base_data)
{
	base_data.clear();
	if (NULL==pCtpTradeApi)
	{
		return false;
	}
	base_data = pCtpTradeApi->get_all_instruments();  //ͨ������api������ⲿ���нӿ�get_all_instruments()����ȡ���Ա����all_instruments������Լ��Ϣ����
	return true;
}

bool CtpService::get_exchanges(vector<Exchange>& exchanges)
{
	exchanges.clear();
	if (NULL == pCtpTradeApi)
	{
		return false;
	}
	exchanges = pCtpTradeApi->get_all_exchanges();  //ͨ������api������ⲿ���нӿ�get_all_exchanges()����ȡ���Ա����all_exchanges������������Ϣ����
	return true;
}

bool CtpService::get_all_InvestorPositionDetails(vector<InvestorPositionDetail>& positiondetails)
{
	positiondetails.clear();
	if (NULL == pCtpTradeApi)
	{
		return false;
	}
	positiondetails = pCtpTradeApi->get_all_InvestorPositionDetails();
	return true;
}

std::string CtpService::get_username()
{
	return user_name;
}

ServerInfo CtpService::get_brokerinfo()
{
	return broker;
}

bool CtpService::islogined()
{
	if (pCtpMdApi != NULL && pCtpTradeApi !=NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CtpService::request(int request_id, void* param)
{//��Ӧ�ò�ʹ��
	bool bRet = false;
	switch (request_id)
	{
		case QryTradingAccount:
		{
			TradeAccountParam* pTradeAccount = reinterpret_cast<TradeAccountParam*>(param);
			if (NULL == pTradeAccount)	return false; //���ת���Ƿ�ɹ�,������NULL������
			bRet = query_account(*pTradeAccount);
			break;
		}
		case QryMarketData:
		{
			vector<string> * pInstruments = reinterpret_cast<vector<string>*>(param);
			if (NULL == pInstruments)	return false;
			bRet = query_market_data(*pInstruments);	
			break;
		}
		case QryInstruments:
		{
			vector<string> * pInstruments = reinterpret_cast<vector<string>*>(param);
			if (NULL == pInstruments)	return false;
			bRet = query_instruments(*pInstruments, true);	//Ĭ�϶�����������,��ͨ���޸�query_instruments�ĵڶ�������ʵ�ֲ��ֶ���
			break;
		}
		case ctp_msg::MakeOrder:
		{
			InputOrderParam* pInputOrderParam = reinterpret_cast<InputOrderParam*>(param);
			if (NULL == pInputOrderParam)	return false;
			bRet = make_order(*pInputOrderParam);
			break;
		}
		case ctp_msg::Make_ParkedOrder:
		{
			InputParkedOrderParam* pInputParkedOrderParam = reinterpret_cast<InputParkedOrderParam*>(param);
			if (NULL == pInputParkedOrderParam)	return false;
			bRet = make_parkedorder(*pInputParkedOrderParam);
			break;
		}
		case ctp_msg::QryPosition:
		{
			PositionParam* pPositionParam = reinterpret_cast<PositionParam*>(param);
			if (NULL == pPositionParam)	return false;
			bRet = query_position(*pPositionParam);
			break;
		}
		case ctp_msg::QryCommissionRate:
		{
			InstrumentCommissionRateParam* pInstrumentCommissionRate = reinterpret_cast<InstrumentCommissionRateParam*>(param);
			if (NULL == pInstrumentCommissionRate)	return false;
			bRet = query_CommissionRate(*pInstrumentCommissionRate);
			break;
		}
		case ctp_msg::QryOrder:
		{
			QryOrderParam* pQryOrder = reinterpret_cast<QryOrderParam*>(param);
			if (NULL == pQryOrder)	return false;
			bRet = query_Order(*pQryOrder);
			break;
		}
		case ctp_msg::CancelOrder:
		{
			ActionOrderParam* pCancelOrder = reinterpret_cast<ActionOrderParam*>(param);
			if (NULL == pCancelOrder)	return false;
			bRet = cancel_order(*pCancelOrder);
			break;
		}
		case ctp_msg::QryTrade:
		{
			TradeParam* pTrade = reinterpret_cast<TradeParam*>(param);
			if (NULL == pTrade)	return false;
			bRet = query_Trade(*pTrade);
			break;
		}
		case ctp_msg::QryMarginRate:
		{
			QryInstrumentMarginRateParam* pMarginRate = reinterpret_cast<QryInstrumentMarginRateParam*>(param);
			if (NULL == pMarginRate)	return false;
			bRet = query_MarginRate(*pMarginRate);
			break;
		}
		case ctp_msg::QryExchangeRate:
		{
			QryExchangeRateParam* pExchangeRate = reinterpret_cast<QryExchangeRateParam*>(param);
			if (NULL == pExchangeRate)	return false;
			bRet = query_ExchangeRate(*pExchangeRate);
			break;
		}
		case ctp_msg::QryInstrumentOrderCommRate:
		{
			QryInstrumentOrderCommRateParam* pOrderCommRate = reinterpret_cast<QryInstrumentOrderCommRateParam*>(param);
			if (NULL == pOrderCommRate)	return false;
			bRet = query_InstrumentOrderCommRate(*pOrderCommRate);
			break;
		}
		case ctp_msg::QryProduct:
		{
			QryProductParam* pProduct = reinterpret_cast<QryProductParam*>(param);
			if (NULL == pProduct)	return false;
			bRet = query_Product(*pProduct);
			break;
		}
		case ctp_msg::QryDepthMarketData:
		{
			QryDepthMarketData��aram* pDepthMarketData = reinterpret_cast<QryDepthMarketData��aram*>(param);
			if (NULL == pDepthMarketData)	return false;
			bRet = query_DepthMarketData(*pDepthMarketData);
			break;
		}
		case ctp_msg::QryInvestorPositionDetail:
		{
			QryInvestorPositionDetailParam* pInvestorPositionDetail = reinterpret_cast<QryInvestorPositionDetailParam*>(param);
			if (NULL == pInvestorPositionDetail)	return false;
			bRet = query_InvestorPositionDetail(*pInvestorPositionDetail);
			break;
		}
		case ctp_msg::BatchOrderAction:
		{
			BatchOrderActionParam* pBatchOrderActionParam = reinterpret_cast<BatchOrderActionParam*>(param);
			if (NULL == pBatchOrderActionParam)	return false;
			bRet = batchorder_action(*pBatchOrderActionParam);
			break;
		}
		case ctp_msg::RspQryNotice:
		{
			QryNoticeParam* pQryNotice = reinterpret_cast<QryNoticeParam*>(param);
			if (NULL == pQryNotice)	return false;
			bRet = query_notice(*pQryNotice);
			break;
		}
		case ctp_msg::RspQryTradingNotice:
		{
			QryTradingNoticeParam* pQryTradingNotice = reinterpret_cast<QryTradingNoticeParam*>(param);
			if (NULL == pQryTradingNotice)	return false;
			bRet = query_tradingnotice(*pQryTradingNotice);
			break;
		}
		case ctp_msg::QryParkedOrder:
		{
			QryParkedOrderParam* pQryParkedOrder = reinterpret_cast<QryParkedOrderParam*>(param);
			if (NULL == pQryParkedOrder)	return false;
			bRet = query_parkedorder(*pQryParkedOrder);
			break;
		}
		case ctp_msg::RemoveParkedOrder:
		{
			RemoveParkedOrderParam* pRemoveParkedOrder = reinterpret_cast<RemoveParkedOrderParam*>(param);
			if (NULL == pRemoveParkedOrder)	return false;
			bRet = remove_parkedorder(*pRemoveParkedOrder);
			break;
		}
		case ctp_msg::Cancel_ParkedOrder:
		{
			ParkedOrderActionParam* pParkedOrderAction = reinterpret_cast<ParkedOrderActionParam*>(param);
			if (NULL == pParkedOrderAction)	return false;
			bRet = cancel_parkedorder(*pParkedOrderAction);
			break;
		}
		case ctp_msg::QryParkedOrderAction :
		{
			QryParkedOrderActionParam* pQryParkedOrderAction = reinterpret_cast<QryParkedOrderActionParam*>(param);
			if (NULL == pQryParkedOrderAction)	return false;
			bRet = query_parkedorderaction(*pQryParkedOrderAction);
			break;
		}
		case ctp_msg::RemoveParkedOrderAction:
		{
			RemoveParkedOrderActionParam* pRemoveParkedOrderAction = reinterpret_cast<RemoveParkedOrderActionParam*>(param);
			if (NULL == pRemoveParkedOrderAction)	return false;
			bRet = remove_parkedorderaction(*pRemoveParkedOrderAction);
			break;
		}
		default:
		{
			break;
		}
	}
return bRet;
}

/*
	pCtpTradeApi == NULL˵����װ��Ľ���api������δ����
	pCtpTradeApi->GetTraderApi() == NULL˵��CThostFtdcTraderApi���Ͷ�����δ����
*/
bool CtpService::query_account(TradeAccountParam& param)
{
	if (pCtpTradeApi == NULL || pCtpTradeApi->GetTraderApi() == NULL)
	{
		return false;
	}
	CThostFtdcQryTradingAccountField req = { 0 };
	if (strlen(param.BrokerID) != 0)	strcpy_s(req.BrokerID, param.BrokerID);
	else                                          strcpy_s(req.BrokerID, broker.id.c_str());
	if (strlen(param.InvestorID) != 0)  strcpy_s(req.InvestorID, param.InvestorID);
	else                                              strcpy_s(req.InvestorID, user_name.c_str());
	if (strlen(param.CurrencyID) != 0) strcpy_s(req.CurrencyID, param.CurrencyID);
	else                                                strcpy_s(req.CurrencyID, "CNY");
	int ret_code = pCtpTradeApi->GetTraderApi()->ReqQryTradingAccount(&req, ++T_iReqestID);
	while (ret_code != 0)
	{
		Sleep(1000);
		ret_code = pCtpTradeApi->GetTraderApi()->ReqQryTradingAccount(&req, ++T_iReqestID);
		//::MessageBox(NULL, _T("��������,�ȴ�����"), _T("���˻�"), MB_OK);//������
	}
	return true;
}

bool CtpService::query_instruments(vector<string>& instrument_vector, bool qry_all)
{
	if (pCtpTradeApi==NULL || pCtpTradeApi->GetTraderApi()==NULL)
	{
		return false;
	}
	CThostFtdcQryInstrumentField  instrument_field = { 0 };
	vector<string> Instrument_Exchange_strs;//���ڴ洢�и������ַ���,�ֱ�Ϊ��Լ���Ͷ�Ӧ��������
	if (qry_all == true)
	{//��instrument_fieldΪ��,��Ĭ�ϲ�ѯ����
		Sleep(1000);//ctp��ѯÿ��һ��
		int ret_code = pCtpTradeApi->GetTraderApi()->ReqQryInstrument(&instrument_field, ++T_iReqestID);
		while (ret_code != 0)
		{
			Sleep(1000);
			ret_code = pCtpTradeApi->GetTraderApi()->ReqQryInstrument(&instrument_field, ++T_iReqestID);
		}
		return true;
	}
	for (int i = 0; i < instrument_vector.size(); ++i)
	{
		memset(&instrument_field, 0, sizeof(instrument_field));
		Instrument_Exchange_strs.clear();//ÿ���µĲ�ѯ����ǰ,����������

		char * strs = new char[instrument_vector[i].length() + 1];
		strcpy(strs, instrument_vector[i].c_str());
		char * delim = "/";
		char *p = strtok(strs, delim);
		while (p) {
			string s = p;			//�ָ�õ����ַ�����ת��Ϊstring����
			Instrument_Exchange_strs.push_back(s); //����������
			p = strtok(NULL, delim);
		}

		strcpy_s(instrument_field.InstrumentID, Instrument_Exchange_strs[0].c_str());	//����Լ��
		strcpy_s(instrument_field.ExchangeID, Instrument_Exchange_strs[1].c_str());		//����Ӧ��������
		int ret_code=pCtpTradeApi->GetTraderApi()->ReqQryInstrument(&instrument_field, ++T_iReqestID);
		while (ret_code !=0)
		{
			Sleep(1000);
			ret_code = pCtpTradeApi->GetTraderApi()->ReqQryInstrument(&instrument_field, ++T_iReqestID);
		}
	}
	return true;
}

bool CtpService::query_exchanges(const string& eid)
{
	if (pCtpTradeApi == NULL || pCtpTradeApi->GetTraderApi() == NULL)
	{
		return false;
	}
	//��ѯ���н�������Ϣ
	CThostFtdcQryExchangeField  req = { 0 };//��ctpԼ���У������սṹ���ȥ��Ĭ�ϲ�ѯ����
	strcpy_s(req.ExchangeID, eid.c_str());
	int ret_code = pCtpTradeApi->GetTraderApi()->ReqQryExchange(&req, ++T_iReqestID);
	while (ret_code != 0)
	{
		Sleep(1000);
		ret_code = pCtpTradeApi->GetTraderApi()->ReqQryExchange(&req, ++T_iReqestID);
	}
	return true;
}

bool CtpService::query_position(PositionParam& param)
{
	if (pCtpTradeApi == NULL || pCtpTradeApi->GetTraderApi() == NULL)
	{
		return false;
	}
	CThostFtdcQryInvestorPositionField req;
	memset(&req, 0, sizeof(req));
	if (strlen(param.BrokerID) !=0)	strcpy_s(req.BrokerID, param.BrokerID);
	else                                          strcpy_s(req.BrokerID, broker.id.c_str());
	if (strlen(param.InvestorID) != 0)  strcpy_s(req.InvestorID, param.InvestorID);
	else                                              strcpy_s(req.InvestorID, user_name.c_str());
	if (strlen(param.InstrumentID) != 0)		strcpy_s(req.InstrumentID, param.InstrumentID);
	else                                                      strcpy_s(req.InstrumentID, "");//����д��Լ�򷵻����гֲ�
	int ret_code = pCtpTradeApi->GetTraderApi()->ReqQryInvestorPosition(&req, ++T_iReqestID);
	while (ret_code != 0)
	{
		Sleep(1000);
		ret_code = pCtpTradeApi->GetTraderApi()->ReqQryInvestorPosition(&req, ++T_iReqestID);
	}
	return true;
}

bool CtpService::query_CommissionRate(InstrumentCommissionRateParam& param)
{
	if (pCtpTradeApi == NULL || pCtpTradeApi->GetTraderApi() == NULL)
	{
		return false;
	}
	CThostFtdcQryInstrumentCommissionRateField a = { 0 };
	if (strlen(param.BrokerID) != 0)		strcpy(a.BrokerID, param.BrokerID);
	else                                              strcpy(a.BrokerID, broker.id.c_str());
	if (strlen(param.InvestorID) != 0)	strcpy(a.InvestorID, param.InvestorID);
	else                                              strcpy(a.InvestorID, user_name.c_str());
	if (strlen(param.InstrumentID) != 0)	strcpy(a.InstrumentID, param.InstrumentID);
	else                                                  strcpy(a.InstrumentID, "");//���InstrumentID��գ��򷵻سֲֶ�Ӧ�ĺ�Լ�������ʡ�
	int ret_code = pCtpTradeApi->GetTraderApi()->ReqQryInstrumentCommissionRate(&a, ++T_iReqestID);
	while (ret_code != 0)
	{
		Sleep(2000);
		ret_code = pCtpTradeApi->GetTraderApi()->ReqQryInstrumentCommissionRate(&a, ++T_iReqestID);
	}
	return true;
}

bool CtpService::query_Order(QryOrderParam& param)
{
	if (pCtpTradeApi == NULL || pCtpTradeApi->GetTraderApi() == NULL)
	{
		return false;
	}
	CThostFtdcQryOrderField req = {0};
	if (strlen(param.BrokerID.c_str()) != 0)		strcpy_s(req.BrokerID, param.BrokerID.c_str());
	else															strcpy_s(req.BrokerID, broker.id.c_str());
	if (strlen(param.InvestorID.c_str()) != 0)	strcpy_s(req.InvestorID, param.InvestorID.c_str());
	else														strcpy_s(req.InvestorID, user_name.c_str());
	int ret_code = pCtpTradeApi->GetTraderApi()->ReqQryOrder(&req, ++T_iReqestID);
	while (ret_code != 0)
	{
		Sleep(2000);
		ret_code = pCtpTradeApi->GetTraderApi()->ReqQryOrder(&req, ++T_iReqestID);
	}
	return true;
}

bool CtpService::cancel_order(ActionOrderParam& param)
{
	if (pCtpTradeApi == NULL || pCtpTradeApi->GetTraderApi() == NULL)
	{
		return false;
	}
	CThostFtdcInputOrderActionField req = { 0 };
	if (strlen(param.BrokerID) != 0)		strcpy_s(req.BrokerID, param.BrokerID);
	else												strcpy_s(req.BrokerID, broker.id.c_str());
	if (strlen(param.InvestorID) != 0)	strcpy_s(req.InvestorID, param.InvestorID);
	else												strcpy_s(req.InvestorID, user_name.c_str());
	if (strlen(param.UserID) != 0)	strcpy_s(req.UserID, param.UserID);
	else											strcpy_s(req.UserID, user_name.c_str());
	strcpy_s(req.OrderSysID, param.OrderSysID);  //��ӦҪ������OrderSysID
	strcpy_s(req.ExchangeID, param.ExchangeID);
	strcpy_s(req.InstrumentID, param.InstrumentID);
	req.ActionFlag = THOST_FTDC_AF_Delete; //ָ������
	int ret_code = pCtpTradeApi->GetTraderApi()->ReqOrderAction(&req, ++T_iReqestID);
	while (ret_code != 0)
	{
		Sleep(2000);
		ret_code = pCtpTradeApi->GetTraderApi()->ReqOrderAction(&req, ++T_iReqestID);
	}
	return true;
}

bool CtpService::query_Trade(TradeParam& param)
{
	if (pCtpTradeApi == NULL || pCtpTradeApi->GetTraderApi() == NULL)
	{
		return false;
	}
	CThostFtdcQryTradeField req = { 0 };
	if (strlen(param.BrokerID.c_str()) != 0)		strcpy_s(req.BrokerID, param.BrokerID.c_str());
	else															strcpy_s(req.BrokerID, broker.id.c_str());
	if (strlen(param.InvestorID.c_str()) != 0)		strcpy_s(req.InvestorID, param.InvestorID.c_str());
	else															strcpy_s(req.InvestorID, user_name.c_str());
	int ret_code = pCtpTradeApi->GetTraderApi()->ReqQryTrade(&req, ++T_iReqestID);
	while (ret_code != 0)
	{
		Sleep(1000);
		ret_code = pCtpTradeApi->GetTraderApi()->ReqQryTrade(&req, ++T_iReqestID);
	}
	return true;
}

bool CtpService::query_MarginRate(QryInstrumentMarginRateParam& param)
{
	if (pCtpTradeApi == NULL || pCtpTradeApi->GetTraderApi() == NULL)
	{
		return false;
	}
	CThostFtdcQryInstrumentMarginRateField req = { 0 };
	if (strlen(param.BrokerID.c_str()) != 0)		strcpy_s(req.BrokerID, param.BrokerID.c_str());
	else															strcpy_s(req.BrokerID, broker.id.c_str());
	if (strlen(param.InvestorID.c_str()) != 0)		strcpy_s(req.InvestorID, param.InvestorID.c_str());
	else															strcpy_s(req.InvestorID, user_name.c_str());
	if (strlen(param.InstrumentID.c_str()) != 0)	strcpy(req.InstrumentID, param.InstrumentID.c_str());
	else                                                  strcpy(req.InstrumentID, "");//���InstrumentID��գ��򷵻سֲֶ�Ӧ�ı�֤����
	req.HedgeFlag=param.HedgeFlag;
	int ret_code = pCtpTradeApi->GetTraderApi()->ReqQryInstrumentMarginRate(&req, ++T_iReqestID);
	while (ret_code != 0)
	{
		Sleep(1000);
		ret_code = pCtpTradeApi->GetTraderApi()->ReqQryInstrumentMarginRate(&req, ++T_iReqestID);
	}
	return true;
}

bool CtpService::query_market_data(vector<string>& instrumentid /*= vector<string>()*/)
{
	//1.�������������������ѯ�������ݣ����ܿ��ܵ����ǽ��յ�����ʱ�����������Ѿ������˸���
	//int ret_code = pCtpApi->GetTraderApi()->ReqQryDepthMarketData(&a, ++T_iReqestID);
	//2.��÷�����ͨ������api����������
	if (pCtpMdApi == NULL)
	{
		return false;
	}
	pCtpMdApi->SubscribeMarketData(instrumentid);
	return true;
}

bool CtpService::make_order(InputOrderParam& param)
{
	if (pCtpTradeApi == NULL || pCtpTradeApi->GetTraderApi() == NULL)
	{
		return false;
	}

	CThostFtdcInputOrderField req;
	memset(&req, 0, sizeof(req));
	//����û���Ϣ
	if (param.BrokerID.empty())	param.BrokerID = this->pCtpTradeApi->get_brokerinfo().id;
	strcpy_s(req.BrokerID, param.BrokerID.c_str());
	if (param.InvestorID.empty())	param.InvestorID = this->pCtpTradeApi->get_username();
	strcpy_s(req.InvestorID, param.InvestorID.c_str());
	strcpy_s(req.UserID, param.InvestorID.c_str());
	//��غ�Լ��Ϣ
	strcpy_s(req.ExchangeID, param.ExchangeID.c_str());
	strcpy_s(req.InstrumentID, param.InstrumentID.c_str());
	//�µ���Ϣ
	req.Direction = param.Direction;	//���ַ���
	req.CombOffsetFlag[0] = param.CombOffsetFlag[0];	 //�������ͣ��ǿ���/ƽ�ֻ���ƽ��
	req.VolumeTotalOriginal = param.VolumeTotalOriginal; //��������
	req.MinVolume = 1; //��С�ɽ���Ĭ����1
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;		 //���Ͷ���ױ���־, Ĭ�Ͼ�ѡͶ����
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;  //ǿƽԭ��: Ĭ�Ϸ�ǿƽ
	req.IsAutoSuspend = 0; //�Զ������־: Ĭ��Ϊ��
	req.UserForceClose = 0;  //�û�ǿ����־: Ĭ��Ϊ��
	//����ʵ�ֱ������õĵ���, ʵ�ֶ����ı���Ψһ�Թ���
	int cur_order_ref = atoi(this->pCtpTradeApi->get_userinfo().MaxOrderRef); //�ӵ�¼�ر���Ϣ�ṹ���л����󱨵�����
	cur_order_ref++;
	if (param.OrderRef=="")
	{//��OrderRefΪ��,˵�����ֶ��µ���
		sprintf(req.OrderRef, "%d", cur_order_ref);	//�������ı����������req.OrderRef��ȥ
	}
	else
	{//����Ϊ��,˵���ǲ��Խӿ��µ���
		sprintf(req.OrderRef, "%s_%d", param.OrderRef, cur_order_ref);
	}
	strcpy_s(this->pCtpTradeApi->get_userinfo().MaxOrderRef, to_string(cur_order_ref).c_str());	//�޸ĵ�¼��Ϣ�ṹ���е���󱨵�����,  ʵ���´�ʹ��ʱȷ���ǵ�������
	if (param.OrderPriceType==THOST_FTDC_OPT_LimitPrice)	 
	{//�Ĵ�������֧���޼۵���������֧���м۵���Ϊ�˽���ȫ�����޼�THOST_FTDC_OPT_LimitPrice
		//�޼�
		if (param.ContingentCondition == THOST_FTDC_CC_Immediately)//Ϊ�����޼۵�
		{
			req.ContingentCondition = THOST_FTDC_CC_Immediately; //��������;
			req.TimeCondition = THOST_FTDC_TC_GFD;//������Ч
			req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			req.VolumeCondition = THOST_FTDC_VC_AV;	//��������
			req.LimitPrice = param.LimitPrice;
		}
		else //�������޼۵������޼۴��۵�
		{
			req.ContingentCondition = param.ContingentCondition;
			req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			req.TimeCondition = param.TimeCondition;
			req.VolumeCondition = THOST_FTDC_VC_AV;	//��������
			req.LimitPrice = param.LimitPrice; //�����۸񣬼��Ǵ���������Ч���������ļ۸�ɽ�
			req.StopPrice = param.StopPrice; //��Ϊ�����۸�
		}
	}
	if (param.OrderPriceType == THOST_FTDC_OPT_AnyPrice)
	{
		//�м�;
		if (param.ContingentCondition == THOST_FTDC_CC_Immediately) 
		{//Ϊ�����м۵�
			req.ContingentCondition = THOST_FTDC_CC_Immediately;		//��������;
			req.TimeCondition = THOST_FTDC_TC_IOC;								//������ɣ�������, �����м۵��ص�
			req.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
			req.VolumeCondition = THOST_FTDC_VC_AV;							//��������
			req.LimitPrice = 0;																	//�м۵�, �۸���0����
		}
		else //�������м۵��������ۺ�ת�м۵Ĵ��۵�
		{
			req.ContingentCondition = param.ContingentCondition;
			req.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
			req.TimeCondition = param.TimeCondition;
			req.VolumeCondition = THOST_FTDC_VC_AV;							//��������
			req.LimitPrice = 0;																	//�����۸�Ϊ�мۣ���0����
			req.StopPrice = param.StopPrice;												//��Ϊ�����۸�
		}
	}
	//req.StopPrice = 0; //ֹ����趨
	int iResult = pCtpTradeApi->GetTraderApi()->ReqOrderInsert(&req, ++T_iReqestID);
	while (iResult != 0)
	{
		Sleep(2000);
		iResult = pCtpTradeApi->GetTraderApi()->ReqOrderInsert(&req, ++T_iReqestID);
	}
	return true;
}


bool CtpService::query_ExchangeRate(QryExchangeRateParam& param)
{
	if (pCtpTradeApi == NULL || pCtpTradeApi->GetTraderApi() == NULL)
	{
		return false;
	}
	CThostFtdcQryExchangeRateField req = { 0 };
	if (strlen(param.BrokerID.c_str()) != 0)		strcpy_s(req.BrokerID, param.BrokerID.c_str());
	else															strcpy_s(req.BrokerID, broker.id.c_str());
	strcpy_s(req.FromCurrencyID, param.FromCurrencyID.c_str());
	strcpy_s(req.ToCurrencyID, param.ToCurrencyID.c_str());
	int iResult = pCtpTradeApi->GetTraderApi()->ReqQryExchangeRate(&req, ++T_iReqestID);
	while (iResult != 0)
	{
		Sleep(1000);
		iResult = pCtpTradeApi->GetTraderApi()->ReqQryExchangeRate(&req, ++T_iReqestID);
	}
	return true;
}

bool CtpService::query_InstrumentOrderCommRate(QryInstrumentOrderCommRateParam& param)
{
	if (pCtpTradeApi == NULL || pCtpTradeApi->GetTraderApi() == NULL)
	{
		return false;
	}
	CThostFtdcQryInstrumentOrderCommRateField req = { 0 };
	if (strlen(param.BrokerID.c_str()) != 0)		strcpy_s(req.BrokerID, param.BrokerID.c_str());
	else															strcpy_s(req.BrokerID, broker.id.c_str());
	if (strlen(param.InvestorID.c_str()) != 0)		strcpy_s(req.InvestorID, param.InvestorID.c_str());
	else															strcpy_s(req.InvestorID, user_name.c_str());
	strcpy_s(req.InstrumentID, param.InstrumentID.c_str());
	int iResult = pCtpTradeApi->GetTraderApi()->ReqQryInstrumentOrderCommRate(&req, ++T_iReqestID);
	while (iResult != 0)
	{
		Sleep(1000);
		iResult = pCtpTradeApi->GetTraderApi()->ReqQryInstrumentOrderCommRate(&req, ++T_iReqestID);
	}
	return true;
}

bool CtpService::query_Product(QryProductParam& param)
{
	if (pCtpTradeApi == NULL || pCtpTradeApi->GetTraderApi() == NULL)
	{
		return false;
	}
	CThostFtdcQryProductField req = { 0 };
	strcpy_s(req.ProductID, param.ProductID.c_str());
	req.ProductClass = THOST_FTDC_PC_Futures;
	strcpy_s(req.ExchangeID, param.ExchangeID.c_str());
	int iResult = pCtpTradeApi->GetTraderApi()->ReqQryProduct(&req, ++T_iReqestID);
	while (iResult != 0)
	{
		Sleep(1000);
		iResult = pCtpTradeApi->GetTraderApi()->ReqQryProduct(&req, ++T_iReqestID);
	}
	return true;
}

bool CtpService::query_DepthMarketData(QryDepthMarketData��aram& param)
{
	if (pCtpTradeApi == NULL || pCtpTradeApi->GetTraderApi() == NULL)
	{
		return false;
	}
	CThostFtdcQryDepthMarketDataField req = { 0 };
	strcpy_s(req.InstrumentID, param.InstrumentID.c_str());
	strcpy_s(req.ExchangeID, param.ExchangeID.c_str());
	int iResult = pCtpTradeApi->GetTraderApi()->ReqQryDepthMarketData(&req, ++T_iReqestID);
	while (iResult != 0)
	{
		Sleep(1000);
		iResult = pCtpTradeApi->GetTraderApi()->ReqQryDepthMarketData(&req, ++T_iReqestID);
	}
	return true;
}

bool CtpService::query_InvestorPositionDetail(QryInvestorPositionDetailParam& param)
{
	if (pCtpTradeApi == NULL || pCtpTradeApi->GetTraderApi() == NULL)
	{
		return false;
	}
	CThostFtdcQryInvestorPositionDetailField req = { 0 };
	if (strlen(param.BrokerID.c_str()) != 0)		strcpy_s(req.BrokerID, param.BrokerID.c_str());
	else															strcpy_s(req.BrokerID, broker.id.c_str());
	if (strlen(param.InvestorID.c_str()) != 0)		strcpy_s(req.InvestorID, param.InvestorID.c_str());
	else															strcpy_s(req.InvestorID, user_name.c_str());
	int iResult = pCtpTradeApi->GetTraderApi()->ReqQryInvestorPositionDetail(&req, ++T_iReqestID);
	while (iResult != 0)
	{
		Sleep(1000);
		iResult = pCtpTradeApi->GetTraderApi()->ReqQryInvestorPositionDetail(&req, ++T_iReqestID);
	}
	return true;
}

bool CtpService::batchorder_action(BatchOrderActionParam& param)
{
	if (pCtpTradeApi == NULL || pCtpTradeApi->GetTraderApi() == NULL)
	{
		return false;
	}
	CThostFtdcInputBatchOrderActionField req = { 0 };
	if (strlen(param.BrokerID.c_str()) != 0)		strcpy_s(req.BrokerID, param.BrokerID.c_str());
	else															strcpy_s(req.BrokerID, broker.id.c_str());
	if (strlen(param.InvestorID.c_str()) != 0)		strcpy_s(req.InvestorID, param.InvestorID.c_str());
	else															strcpy_s(req.InvestorID, user_name.c_str());
	int iResult = pCtpTradeApi->GetTraderApi()->ReqBatchOrderAction(&req, ++T_iReqestID);
	while (iResult != 0)
	{
		Sleep(1000);
		iResult = pCtpTradeApi->GetTraderApi()->ReqBatchOrderAction(&req, ++T_iReqestID);
	}
	return true;
}

bool CtpService::query_notice(QryNoticeParam& param)
{
	if (pCtpTradeApi == NULL || pCtpTradeApi->GetTraderApi() == NULL)
	{
		return false;
	}
	CThostFtdcQryNoticeField req = { 0 };
	if (strlen(param.BrokerID.c_str()) != 0)		strcpy_s(req.BrokerID, param.BrokerID.c_str());
	else															strcpy_s(req.BrokerID, broker.id.c_str());
	int iResult = pCtpTradeApi->GetTraderApi()->ReqQryNotice(&req, ++T_iReqestID);
	while (iResult != 0)
	{
		Sleep(1000);
		iResult = pCtpTradeApi->GetTraderApi()->ReqQryNotice(&req, ++T_iReqestID);
	}
	return true;
}

bool CtpService::query_tradingnotice(QryTradingNoticeParam& param)
{
	if (pCtpTradeApi == NULL || pCtpTradeApi->GetTraderApi() == NULL)
	{
		return false;
	}
	CThostFtdcQryTradingNoticeField req = { 0 };
	if (strlen(param.BrokerID.c_str()) != 0)		strcpy_s(req.BrokerID, param.BrokerID.c_str());
	else															strcpy_s(req.BrokerID, broker.id.c_str());
	if (strlen(param.InvestorID.c_str()) != 0)		strcpy_s(req.InvestorID, param.InvestorID.c_str());
	else															strcpy_s(req.InvestorID, user_name.c_str());
	int iResult = pCtpTradeApi->GetTraderApi()->ReqQryTradingNotice(&req, ++T_iReqestID);
	while (iResult != 0)
	{
		Sleep(1000);
		iResult = pCtpTradeApi->GetTraderApi()->ReqQryTradingNotice(&req, ++T_iReqestID);
	}
	return true;
}

bool CtpService::make_parkedorder(InputParkedOrderParam& param)
{
	if (pCtpTradeApi == NULL || pCtpTradeApi->GetTraderApi() == NULL)
	{
		return false;
	}

	CThostFtdcParkedOrderField req;
	memset(&req, 0, sizeof(req));
	//����û���Ϣ
	if (param.BrokerID.empty())	param.BrokerID = this->pCtpTradeApi->get_brokerinfo().id;
	strcpy_s(req.BrokerID, param.BrokerID.c_str());
	if (param.InvestorID.empty())	param.InvestorID = this->pCtpTradeApi->get_username();
	strcpy_s(req.InvestorID, param.InvestorID.c_str());
	strcpy_s(req.UserID, param.InvestorID.c_str());
	//��غ�Լ��Ϣ
	strcpy_s(req.ExchangeID, param.ExchangeID.c_str());
	strcpy_s(req.InstrumentID, param.InstrumentID.c_str());
	//�µ���Ϣ
	req.Direction = param.Direction;	//���ַ���
	req.CombOffsetFlag[0] = param.CombOffsetFlag[0];	 //�������ͣ��ǿ���/ƽ�ֻ���ƽ��
	req.VolumeTotalOriginal = param.VolumeTotalOriginal; //��������
	req.MinVolume = 1; //��С�ɽ���Ĭ����1
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;		 //���Ͷ���ױ���־, Ĭ��Ͷ��
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;  //ǿƽԭ��: Ĭ�Ϸ�ǿƽ
	req.IsAutoSuspend = 0;   //�Զ������־: Ĭ��Ϊ��
	req.UserForceClose = 0;  //�û�ǿ����־: Ĭ��Ϊ��
	//����ʵ�ֱ������õĵ���, ���ǲ�δʵ�ֶ����ı���Ψһ�Թ���
	int cur_order_ref = atoi(this->pCtpTradeApi->get_userinfo().MaxOrderRef); //�ӵ�¼�ر���Ϣ�ṹ���л����󱨵�����
	cur_order_ref++;
	sprintf(req.OrderRef, "%d", cur_order_ref);	//�������ı����������req.OrderRef��ȥ
	strcpy_s(this->pCtpTradeApi->get_userinfo().MaxOrderRef, req.OrderRef);	//�޸ĵ�¼��Ϣ�ṹ���е���󱨵�����,  ʵ���´�ʹ��ʱȷ���ǵ�������,��Ȼ�������ӻỰ��MaxOrderRef�ᱻ����Ϊ0
	if (param.OrderPriceType == THOST_FTDC_OPT_LimitPrice)	 
	{//�Ĵ�������֧���޼۵���������֧���м۵���Ϊ�˽���ȫ�����޼�THOST_FTDC_OPT_LimitPrice
		//�޼�
		if (param.ContingentCondition == THOST_FTDC_CC_Immediately)
		{//Ϊ�����޼۵�
			req.ContingentCondition = THOST_FTDC_CC_Immediately; //��������;
			req.TimeCondition = THOST_FTDC_TC_GFD;//������Ч
			strcpy_s(req.GTDDate, "");
			req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			req.VolumeCondition = THOST_FTDC_VC_AV;	//��������
			req.LimitPrice = param.LimitPrice;
		}
		else //�������޼۵�
		{
			req.ContingentCondition = param.ContingentCondition;
			req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			req.TimeCondition = param.TimeCondition;	//ctpĿǰֻ֧�ֵ�����Ч,Ϊ��UIӦ�ò�˴�Ĭ����THOST_FTDC_TC_GFD
			strcpy(req.GTDDate, param.GTDDate.c_str());
			req.VolumeCondition = THOST_FTDC_VC_AV;	//��������
			req.LimitPrice = param.LimitPrice; //�����۸񣬼��Ǵ���������Ч���������ļ۸�ɽ�
			req.StopPrice = param.StopPrice; //��Ϊ�����۸�
		}
	}
	if (param.OrderPriceType == THOST_FTDC_OPT_AnyPrice)
	{
		//�м�;
		if (param.ContingentCondition == THOST_FTDC_CC_Immediately) 
		{//Ϊ�����м۵�
			req.ContingentCondition = THOST_FTDC_CC_Immediately;	//��������;
			req.TimeCondition = THOST_FTDC_TC_IOC;							//������ɣ�������, �����м۵��ص�
			strcpy_s(req.GTDDate, "");
			req.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
			req.VolumeCondition = THOST_FTDC_VC_AV;						//��������
			req.LimitPrice = 0;																//�м۵�, �۸���0����
		}
		else //�������м۵�
		{
			req.ContingentCondition = param.ContingentCondition;
			req.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
			req.TimeCondition = param.TimeCondition;
			strcpy(req.GTDDate, param.GTDDate.c_str());
			req.VolumeCondition = THOST_FTDC_VC_AV;			//��������
			req.LimitPrice = 0;													//�����۸�Ϊ�мۣ���0����
			req.StopPrice = param.StopPrice;								//��Ϊ�����۸�
		}
	}
	int iResult = pCtpTradeApi->GetTraderApi()->ReqParkedOrderInsert(&req, ++T_iReqestID);
	while (iResult != 0)
	{
		Sleep(2000);
		iResult = pCtpTradeApi->GetTraderApi()->ReqParkedOrderInsert(&req, ++T_iReqestID);
	}
	return true;
}

bool CtpService::query_parkedorder(QryParkedOrderParam& param)
{
	if (pCtpTradeApi == NULL || pCtpTradeApi->GetTraderApi() == NULL)
	{
		return false;
	}
	CThostFtdcQryParkedOrderField req = { 0 };
	if (strlen(param.BrokerID.c_str()) != 0)		strcpy_s(req.BrokerID, param.BrokerID.c_str());
	else															strcpy_s(req.BrokerID, broker.id.c_str());
	if (strlen(param.InvestorID.c_str()) != 0)		strcpy_s(req.InvestorID, param.InvestorID.c_str());
	else															strcpy_s(req.InvestorID, user_name.c_str());
	int iResult = pCtpTradeApi->GetTraderApi()->ReqQryParkedOrder(&req, ++T_iReqestID);
	while (iResult != 0)
	{
		Sleep(1000);
		iResult = pCtpTradeApi->GetTraderApi()->ReqQryParkedOrder(&req, ++T_iReqestID);
	}
	return true;
}

bool CtpService::remove_parkedorder(RemoveParkedOrderParam& param)
{
	if (pCtpTradeApi == NULL || pCtpTradeApi->GetTraderApi() == NULL)
	{
		return false;
	}
	CThostFtdcRemoveParkedOrderField  req = { 0 };
	if (strlen(param.BrokerID.c_str()) != 0)		strcpy_s(req.BrokerID, param.BrokerID.c_str());
	else															strcpy_s(req.BrokerID, broker.id.c_str());
	if (strlen(param.InvestorID.c_str()) != 0)		strcpy_s(req.InvestorID, param.InvestorID.c_str());
	else															strcpy_s(req.InvestorID, user_name.c_str());
	strcpy_s(req.ParkedOrderID, param.ParkedOrderID.c_str());
	int iResult = pCtpTradeApi->GetTraderApi()->ReqRemoveParkedOrder(&req, ++T_iReqestID);
	while (iResult != 0)
	{
		Sleep(2000);
		iResult = pCtpTradeApi->GetTraderApi()->ReqRemoveParkedOrder(&req, ++T_iReqestID);
	}
	return true;
}

bool CtpService::cancel_parkedorder(ParkedOrderActionParam& param)
{
	if (pCtpTradeApi == NULL || pCtpTradeApi->GetTraderApi() == NULL)
	{
		return false;
	}
	CThostFtdcParkedOrderActionField  req = { 0 };
	if (strlen(param.BrokerID.c_str()) != 0)		strcpy_s(req.BrokerID, param.BrokerID.c_str());
	else															strcpy_s(req.BrokerID, broker.id.c_str());
	if (strlen(param.InvestorID.c_str()) != 0)
	{
		strcpy_s(req.InvestorID, param.InvestorID.c_str());
		strcpy_s(req.UserID, param.InvestorID.c_str());
	}
	else
	{
		strcpy_s(req.InvestorID, user_name.c_str());
		strcpy_s(req.UserID, param.InvestorID.c_str());
	}
	strcpy_s(req.ExchangeID, param.ExchangeID.c_str());
	strcpy_s(req.InstrumentID, param.InstrumentID.c_str());
	strcpy_s(req.OrderSysID, param.OrderSysID.c_str());
	req.ActionFlag = THOST_FTDC_AF_Delete;
	int iResult = pCtpTradeApi->GetTraderApi()->ReqParkedOrderAction(&req, ++T_iReqestID);
	while (iResult != 0)
	{
		Sleep(2000);
		iResult = pCtpTradeApi->GetTraderApi()->ReqParkedOrderAction(&req, ++T_iReqestID);
	}
	return true;
}

bool CtpService::query_parkedorderaction(QryParkedOrderActionParam& param)
{
	if (pCtpTradeApi == NULL || pCtpTradeApi->GetTraderApi() == NULL)
	{
		return false;
	}
	CThostFtdcQryParkedOrderActionField  req = { 0 };
	if (strlen(param.BrokerID.c_str()) != 0)		strcpy_s(req.BrokerID, param.BrokerID.c_str());
	else															strcpy_s(req.BrokerID, broker.id.c_str());
	if (strlen(param.InvestorID.c_str()) != 0)		strcpy_s(req.InvestorID, param.InvestorID.c_str());
	else															strcpy_s(req.InvestorID, user_name.c_str());
	int iResult = pCtpTradeApi->GetTraderApi()->ReqQryParkedOrderAction(&req, ++T_iReqestID);
	while (iResult != 0)
	{
		Sleep(1000);
		iResult = pCtpTradeApi->GetTraderApi()->ReqQryParkedOrderAction(&req, ++T_iReqestID);
	}
	return true;
}

bool CtpService::remove_parkedorderaction(RemoveParkedOrderActionParam& param)
{
	if (pCtpTradeApi == NULL || pCtpTradeApi->GetTraderApi() == NULL)
	{
		return false;
	}
	CThostFtdcRemoveParkedOrderActionField req = { 0 };
	if (strlen(param.BrokerID.c_str()) != 0)		strcpy_s(req.BrokerID, param.BrokerID.c_str());
	else															strcpy_s(req.BrokerID, broker.id.c_str());
	if (strlen(param.InvestorID.c_str()) != 0)		strcpy_s(req.InvestorID, param.InvestorID.c_str());
	else															strcpy_s(req.InvestorID, user_name.c_str());
	strcpy_s(req.ParkedOrderActionID, param.ParkedOrderActionID.c_str());
	int iResult = pCtpTradeApi->GetTraderApi()->ReqRemoveParkedOrderAction(&req, ++T_iReqestID);
	while (iResult != 0)
	{
		Sleep(2000);
		iResult = pCtpTradeApi->GetTraderApi()->ReqRemoveParkedOrderAction(&req, ++T_iReqestID);
	}
	return true;
}



