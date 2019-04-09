#include "stdafx.h"
#include"TradeSpi.h"
#include"CtpMsgDistributor.h"
#include "CtpService.h"
#include <iostream>
#include <atlstr.h>//ʹ��cstring��Ҫ��ͷ�ļ�
#include <string>
#include "CtpMsg.h"
using namespace std;


extern vector<BaseData> instruments_data;//����DataTypes.cpp�е�ȫ�ֱ��������丳ֵ

TradeSpi::TradeSpi()
{
	m_pTradeApi = NULL;
	m_bInited = false;
	m_iReqestID = 0;

	m_AlreadyQryExchange = false;
	m_AlreadyQryCommissionRate = false;  
	m_AlreadyQryOrder = false;
	m_AlreadyQryPosition = false;
	m_AlreadyQryParkedOrder = false;
	m_AlreadyQryParkedOrderAction = false;
};

TradeSpi::~TradeSpi(){};

CThostFtdcTraderApi* TradeSpi::GetTraderApi()
{
	return m_pTradeApi;
}

void TradeSpi::OnFrontConnected()
{
	get_CtpMsgDistributor()->OnResponse(ctp_msg::RspFrontConnected, 100);//��Ϣ�ź���Я����ӦΪ��Ϣ,Ϊ���Ի���Ľ��������ݽ������ݣ������Ѿ����ӳɹ��������100��
	CThostFtdcReqUserLoginField login_field;
	memset(&login_field, 0, sizeof(login_field));
	strcpy_s(login_field.BrokerID, m_BrokerInfo.id.c_str());
	strcpy_s(login_field.UserID, username.c_str());
	strcpy_s(login_field.Password, password.c_str());
	int ret_code = m_pTradeApi->ReqUserLogin(&login_field, ++m_iReqestID);//�����¼
	while (ret_code !=0)
	{
		Sleep(1000);
		ret_code = m_pTradeApi->ReqUserLogin(&login_field, ++m_iReqestID);
	}
}
void TradeSpi::OnFrontDisconnected(int nReason)
{
	//���Ͽ����ӵ���Ϣ��֪Ӧ�ò�
	m_AlreadyQryExchange = false;
	m_AlreadyQryCommissionRate = false;  //���ߺ���Զ���������ʱҪ����һ�ֲ�ѯѭ����Ϊ����Ϊfalse�����ٴβ�ѯ������
	m_AlreadyQryOrder=false;
	m_AlreadyQryParkedOrder = false;
	::MessageBox(NULL, _T("�Ͽ�����"), _T("�Ͽ�����"), MB_OK);//������
	get_CtpMsgDistributor()->OnResponse(ctp_msg::RspFrontDisconnected, (LPARAM)NULL);
}

void TradeSpi::OnHeartBeatWarning(int nTimeLapse)
{
	//cerr << "--->>> " << "OnHeartBeatWarning" << endl;
	//cerr << "--->>> nTimerLapse = " << nTimeLapse << endl;//���������ʱ����
}

void TradeSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	tradingday = pRspUserLogin->TradingDay;//��¼��ǰ�����գ�Ϊ��ȷ���Ƿ������ıȽ�
	if (IsErrorRspInfo(pRspInfo))
	{
		SetLastError(pRspInfo);//��������Ϣ�����ϲ�
		CString error_msg = pRspInfo->ErrorMsg;
		::MessageBox(NULL, error_msg, _T("ServiceLayer"), MB_OK);//������
		return;//����¼���ɹ�,�ص����طǿյĴ�����Ϣ�ṹ��pRspInfo,��ֱ��return��,��Ҫ����֮�µĲ���
	}
	static UserInfo user_info;//����Ϊstaticȷ���䲻����ʱ����
	strcpy_s(user_info.UserID, pRspUserLogin->UserID);
	strcpy_s(user_info.BrokerID, pRspUserLogin->BrokerID);
	strcpy_s(user_info.TradingDay, pRspUserLogin->TradingDay);
	strcpy_s(user_info.LoginTime, pRspUserLogin->LoginTime);
	strcpy_s(user_info.ExchangeTime, pRspUserLogin->SHFETime);
	user_info.FrontID = pRspUserLogin->FrontID;
	user_info.SessionID = pRspUserLogin->SessionID;
	strcpy_s(user_info.MaxOrderRef, pRspUserLogin->MaxOrderRef);
	strcpy_s(user_info.SystemName, pRspUserLogin->SystemName);
	this->user_info = user_info; //�����Ա����user_info��ֵ
	if (bIsLast)
	{
		get_CtpMsgDistributor()->OnResponse(ctp_msg::RspUserLogined, (LPARAM)&user_info);
	}

	//�����ѯ������Ϣȷ�ϣ����ǿ���û��ȷ�Ͻ����
	CThostFtdcQrySettlementInfoConfirmField req = { 0 };
	memset(&req, 0, sizeof(req));
	strcpy_s(req.BrokerID, m_BrokerInfo.id.c_str());
	strcpy_s(req.InvestorID, username.c_str());
	int iResult=m_pTradeApi->ReqQrySettlementInfoConfirm(&req, ++m_iReqestID);
	while (iResult != 0)
	{
		Sleep(1000);
		iResult = m_pTradeApi->ReqQrySettlementInfoConfirm(&req, ++m_iReqestID);
	}
}

void TradeSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//::MessageBox(NULL, _T("���׵ǳ�"), _T("�ǳ�"), MB_OK);//������
}

void TradeSpi::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{//��ѯ�Ƿ�ȷ�Ϲ�������Ϣ�Ļص�
	if (IsErrorRspInfo(pRspInfo))
	{
		SetLastError(pRspInfo);//��������Ϣ�����ϲ�
		CString error_msg = pRspInfo->ErrorMsg;
		::MessageBox(NULL, error_msg, _T("����"), MB_OK);//������
	}
	else
	{
		if (bIsLast)//��������һ�η���
		{
			get_CtpMsgDistributor()->OnResponse(ctp_msg::RspQrySettlementInfoConfirm, (LPARAM)NULL);
		}
		if (pSettlementInfoConfirm->BrokerID != NULL && strcmp(pSettlementInfoConfirm->ConfirmDate, tradingday.c_str()) == 0)
		{//��δ����ǰ��pSettlementInfoConfirm->BrokerIDΪNULL����ʱҪ���㣻�������pSettlementInfoConfirm->BrokerID��Ϊ����ConfirmDate��tradingdayһ��
			if (bIsLast)//��������һ�η���
			{
				if (m_AlreadyQryExchange == false)
				{
					//��ѯ���н�����
					Sleep(1000);
					CThostFtdcQryExchangeField  req = { 0 };//��ctpԼ���У������ǿյ���ʼ��Ϊ0�Ľṹ���ȥ����Ĭ�ϲ�ѯ����
					int Qryexchanges_ret = m_pTradeApi->ReqQryExchange(&req, ++m_iReqestID);
					while (Qryexchanges_ret != 0)
					{
						Sleep(1000);
						Qryexchanges_ret = m_pTradeApi->ReqQryExchange(&req, ++m_iReqestID);
					}
				}
			}
		}
		else
		{
			//���������Ϣȷ��
			CThostFtdcSettlementInfoConfirmField req;
			memset(&req, 0, sizeof(req));
			strcpy_s(req.BrokerID, m_BrokerInfo.id.c_str());
			strcpy_s(req.InvestorID, username.c_str());
			int ret_code = m_pTradeApi->ReqSettlementInfoConfirm(&req, ++m_iReqestID);
			while (ret_code != 0)
			{
				Sleep(1000);
				ret_code = m_pTradeApi->ReqSettlementInfoConfirm(&req, ++m_iReqestID);
			}
		}
	}
}

void TradeSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{//ȷ�Ͻ�����Ϣ��Ļص�����
	if (IsErrorRspInfo(pRspInfo))
	{
		SetLastError(pRspInfo);//��������Ϣ�����ϲ�
		CString error_msg = pRspInfo->ErrorMsg;
		::MessageBox(NULL, error_msg, _T("����"), MB_OK);//������
		return;
	}

	if (bIsLast)
	{
		//ȷ�Ͻ���ɹ��󣬽�����Ϣ��֪�ϲ�Ӧ��
		get_CtpMsgDistributor()->OnResponse(ctp_msg::RspSettlementInfoConfirm, (LPARAM)NULL);
		if (m_AlreadyQryExchange == false)
		{
			//��ѯ���н�����
			CThostFtdcQryExchangeField  req = { 0 };//��ctpԼ���У������ǿյ���ʼ��Ϊ0�Ľṹ���ȥ����Ĭ�ϲ�ѯ����
			int Qryexchanges_ret = m_pTradeApi->ReqQryExchange(&req, ++m_iReqestID);
			while (Qryexchanges_ret != 0)
			{
				Sleep(1000);
				Qryexchanges_ret = m_pTradeApi->ReqQryExchange(&req, ++m_iReqestID);
			}
		}
	}
}

void TradeSpi::OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (IsErrorRspInfo(pRspInfo))
	{
		SetLastError(pRspInfo);//��������Ϣ�����ϲ�
		CString error_msg = pRspInfo->ErrorMsg;
		::MessageBox(NULL, error_msg, _T("����"), MB_OK);//������
		return;
	}
	//����
	if (NULL != pExchange)
	{
		Exchange exchange;
		memset(&exchange, 0, sizeof(exchange));
		exchange.ExchangeID = pExchange->ExchangeID;
		exchange.ExchangeName = pExchange->ExchangeName;
		exchange.ExchangeProperty = pExchange->ExchangeProperty;
		all_exchanges.push_back(exchange);
	}
	else
	{
		return;
	}
	if (bIsLast)
	{//��ѯ�꽻��������Ϣ���������Ա����exchanges���vector������, ���ǿɽ�����һ���Ĳ���,
		get_CtpMsgDistributor()->OnResponse(ctp_msg::RspQryexchanges, (LPARAM)&all_exchanges);
		m_AlreadyQryExchange = true;
		/*//////////////////////////////////////////////

		UI������ڵ�֪��ѯ��������Ϣ�ɹ���, ��xml�����ļ��ж�ȡ�����ѯ�ĺ�Լ����, ��ctp���Ͳ�ѯ���ֺ�Լ�������ݵ�����
		vector<string> my_instruments = get_config_reader().GetMyInstruments();
		get_global_mgr().GetService()->request(QryInstruments, &my_instruments);

		*///////////////////////////////////////////////
	}
}

void TradeSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (IsErrorRspInfo(pRspInfo))
	{
		SetLastError(pRspInfo);//��������Ϣ�����ϲ�
		CString error_msg = pRspInfo->ErrorMsg;
		::MessageBox(NULL, error_msg, _T("����"), MB_OK);//������
		return;
	}
	if (NULL != pInstrument)
	{
		//�����յ��ĺ�Լ�������ݴ洢����,Ϊ��Ӧ�ò����ͼ��ʾ��
		BaseData instrument_data;
		strcpy_s(instrument_data.code, pInstrument->InstrumentID);
		strcpy_s(instrument_data.ExchangeID, pInstrument->ExchangeID);
		strcpy_s(instrument_data.InstrumentName, pInstrument->InstrumentName);
		strcpy_s(instrument_data.ExchangeInstID, pInstrument->ExchangeInstID);
		strcpy_s(instrument_data.ProductID, pInstrument->ProductID);
		instrument_data.ProductClass=pInstrument->ProductClass;
		instrument_data.DeliveryYear = pInstrument->DeliveryYear;
		instrument_data.DeliveryMonth = pInstrument->DeliveryMonth;
		instrument_data.MaxMarketOrderVolume = pInstrument->MaxMarketOrderVolume;
		instrument_data.MinMarketOrderVolume = pInstrument->MinMarketOrderVolume;
		instrument_data.MaxLimitOrderVolume = pInstrument->MaxLimitOrderVolume;
		instrument_data.MinLimitOrderVolume = pInstrument->MinLimitOrderVolume;
		instrument_data.VolumeMultiple = pInstrument->VolumeMultiple;
		instrument_data.PriceTick = pInstrument->PriceTick;
		strcpy_s(instrument_data.CreateDate, pInstrument->CreateDate);
		strcpy_s(instrument_data.OpenDate, pInstrument->OpenDate);
		strcpy_s(instrument_data.ExpireDate, pInstrument->ExpireDate);
		strcpy_s(instrument_data.StartDelivDate, pInstrument->StartDelivDate);
		strcpy_s(instrument_data.EndDelivDate, pInstrument->EndDelivDate);
		strcpy_s(instrument_data.EndDelivDate, pInstrument->EndDelivDate);
		instrument_data.InstLifePhase = pInstrument->InstLifePhase;
		instrument_data.IsTrading = pInstrument->IsTrading;
		instrument_data.PositionType = pInstrument->PositionType;
		instrument_data.PositionDateType = pInstrument->PositionDateType;
		instrument_data.LongMarginRatio = pInstrument->LongMarginRatio;
		instrument_data.ShortMarginRatio = pInstrument->ShortMarginRatio;
		instrument_data.MaxMarginSideAlgorithm = pInstrument->MaxMarginSideAlgorithm;
		strcpy_s(instrument_data.UnderlyingInstrID, pInstrument->UnderlyingInstrID);
		instrument_data.StrikePrice = pInstrument->StrikePrice;
		instrument_data.OptionsType = pInstrument->OptionsType;
		instrument_data.UnderlyingMultiple = pInstrument->UnderlyingMultiple;
		instrument_data.CombinationType = pInstrument->CombinationType;
		all_instruments.push_back(instrument_data);//�ȴ洢��һ����ʱ�ı��ػ�������ȥ
	}
	if (bIsLast)
	{
		//ע��ÿ��Ʒ�ֺ�Լ��Ϣ�ǵ�������,Ҳ����˵ÿ�η��ض���bIsLast��󷵻�,�����ǲ�ѯ����
		//CString instrument_msg = pInstrument->InstrumentID;   //������
		//::MessageBox(NULL, instrument_msg, _T("��Լ����"), MB_OK);//������
		//һ����һ���ظ���ص�ȥ��
		CThostFtdcQryTradingAccountField req = { 0 };
		strcpy_s(req.InvestorID, username.c_str());
		strcpy_s(req.BrokerID, m_BrokerInfo.id.c_str());
		strcpy_s(req.CurrencyID, "CNY");
		int ret_code=m_pTradeApi->ReqQryTradingAccount(&req, ++m_iReqestID);
		while (ret_code != 0)
		{
			Sleep(1000);
			ret_code = m_pTradeApi->ReqQryTradingAccount(&req, ++m_iReqestID);
		}
		get_CtpMsgDistributor()->OnResponse(ctp_msg::RspQryInstrument, (LPARAM)&all_instruments);
		instruments_data = all_instruments; //��ȫ�ֱ�����ֵ
	}
}

void TradeSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (IsErrorRspInfo(pRspInfo))
	{
		SetLastError(pRspInfo);//��������Ϣ�����ϲ�
		CString error_msg = pRspInfo->ErrorMsg;
		::MessageBox(NULL, error_msg, _T("����"), MB_OK);//������
		return;
	}
	if (bIsLast && pTradingAccount!=NULL)
	{
		static TradingAccount tradingaccount = { 0 };
		strcpy_s(tradingaccount.BrokerID, pTradingAccount->BrokerID);
		strcpy_s(tradingaccount.AccountID, pTradingAccount->AccountID);
		tradingaccount.PreMortgage = pTradingAccount->PreMortgage;
		tradingaccount.PreCredit = pTradingAccount->PreCredit;
		tradingaccount.PreDeposit = pTradingAccount->PreDeposit;
		tradingaccount.PreBalance = pTradingAccount->PreBalance;
		tradingaccount.PreMargin = pTradingAccount->PreMargin;
		tradingaccount.InterestBase = pTradingAccount->InterestBase;
		tradingaccount.Interest = pTradingAccount->Interest;
		tradingaccount.Deposit = pTradingAccount->Deposit;
		tradingaccount.Withdraw = pTradingAccount->Withdraw;
		tradingaccount.FrozenMargin = pTradingAccount->FrozenMargin;
		tradingaccount.FrozenCash = pTradingAccount->FrozenCash;
		tradingaccount.FrozenCommission = pTradingAccount->FrozenCommission;
		tradingaccount.CurrMargin = pTradingAccount->CurrMargin;
		tradingaccount.CashIn = pTradingAccount->CashIn;
		tradingaccount.Commission = pTradingAccount->Commission;
		tradingaccount.CloseProfit = pTradingAccount->CloseProfit;
		tradingaccount.PositionProfit = pTradingAccount->PositionProfit;
		tradingaccount.Balance = pTradingAccount->Balance;
		tradingaccount.Available = pTradingAccount->Available;
		tradingaccount.WithdrawQuota = pTradingAccount->WithdrawQuota;
		tradingaccount.Reserve = pTradingAccount->Reserve;
		strcpy_s(tradingaccount.TradingDay, pTradingAccount->TradingDay);
		tradingaccount.SettlementID = pTradingAccount->SettlementID;
		tradingaccount.Mortgage = pTradingAccount->Mortgage;
		tradingaccount.ExchangeMargin = pTradingAccount->ExchangeMargin;
		tradingaccount.DeliveryMargin = pTradingAccount->DeliveryMargin;
		tradingaccount.ReserveBalance = pTradingAccount->ReserveBalance;
		strcpy_s(tradingaccount.CurrencyID, pTradingAccount->CurrencyID);
		tradingaccount.PreFundMortgageIn = pTradingAccount->PreFundMortgageIn;
		tradingaccount.PreFundMortgageOut = pTradingAccount->PreFundMortgageOut;
		tradingaccount.FundMortgageIn = pTradingAccount->FundMortgageIn;
		tradingaccount.FundMortgageOut = pTradingAccount->FundMortgageOut;
		tradingaccount.FundMortgageAvailable = pTradingAccount->FundMortgageAvailable;
		tradingaccount.MortgageableFund = pTradingAccount->MortgageableFund;
		tradingaccount.SpecProductMargin = pTradingAccount->SpecProductMargin;
		tradingaccount.SpecProductFrozenMargin = pTradingAccount->SpecProductFrozenMargin;
		tradingaccount.SpecProductCommission = pTradingAccount->SpecProductCommission;
		tradingaccount.SpecProductFrozenCommission = pTradingAccount->SpecProductFrozenCommission;
		tradingaccount.SpecProductPositionProfit = pTradingAccount->SpecProductPositionProfit;
		tradingaccount.SpecProductCloseProfit = pTradingAccount->SpecProductCloseProfit;
		tradingaccount.SpecProductPositionProfitByAlg = pTradingAccount->SpecProductPositionProfitByAlg;
		tradingaccount.SpecProductExchangeMargin = pTradingAccount->SpecProductExchangeMargin;
		tradingaccount.BizType = pTradingAccount->BizType;
		tradingaccount.FrozenSwap = pTradingAccount->FrozenSwap;
		tradingaccount.RemainSwap = pTradingAccount->RemainSwap;
		get_CtpMsgDistributor()->OnResponse(ctp_msg::RspQryTradingAccount, (LPARAM)&tradingaccount);
		//���������˻��ֲ�
		if (m_AlreadyQryPosition == false)
		{
			Sleep(2000);
			CThostFtdcQryInvestorPositionField req;
			memset(&req, 0, sizeof(req));
			strcpy_s(req.BrokerID, m_BrokerInfo.id.c_str());
			strcpy_s(req.InvestorID, username.c_str());
			strcpy_s(req.InstrumentID, "");//����д��Լ�򷵻����гֲ�
			int ret_code = m_pTradeApi->ReqQryInvestorPosition(&req, ++m_iReqestID);
			while (ret_code != 0)
			{
				Sleep(2000);
				m_pTradeApi->ReqQryInvestorPosition(&req, ++m_iReqestID);
			}
		}
	}
}

void TradeSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (IsErrorRspInfo(pRspInfo))
	{
		SetLastError(pRspInfo);//��������Ϣ�����ϲ�
		CString error_msg = pRspInfo->ErrorMsg;
		::MessageBox(NULL, error_msg, _T("����"), MB_OK);//������
		return;
	}
	static vector<InvestorPosition> all_InvestorPosition; 
	InvestorPosition Position;
	if (pInvestorPosition!=NULL)
	{
		memset(&Position, 0, sizeof(Position));
		Position.InstrumentID = pInvestorPosition->InstrumentID;
		Position.BrokerID = pInvestorPosition->BrokerID;
		Position.InvestorID = pInvestorPosition->InvestorID;
		Position.PosiDirection = pInvestorPosition->PosiDirection;
		Position.HedgeFlag = pInvestorPosition->HedgeFlag;
		Position.PositionDate = pInvestorPosition->PositionDate;
		Position.YdPosition = pInvestorPosition->YdPosition;
		Position.Position = pInvestorPosition->Position;
		Position.LongFrozen = pInvestorPosition->LongFrozen;
		Position.ShortFrozen = pInvestorPosition->ShortFrozen;
		Position.LongFrozenAmount = pInvestorPosition->LongFrozenAmount;
		Position.ShortFrozenAmount = pInvestorPosition->ShortFrozenAmount;
		Position.OpenVolume = pInvestorPosition->OpenVolume;
		Position.CloseVolume = pInvestorPosition->CloseVolume;
		Position.OpenAmount = pInvestorPosition->OpenAmount;
		Position.CloseAmount = pInvestorPosition->CloseAmount;
		Position.PositionCost = pInvestorPosition->PositionCost;
		Position.PreMargin = pInvestorPosition->PreMargin;
		Position.UseMargin = pInvestorPosition->UseMargin;
		Position.FrozenCash = pInvestorPosition->FrozenCash;
		Position.FrozenMargin = pInvestorPosition->FrozenMargin;
		Position.FrozenCommission = pInvestorPosition->FrozenCommission;
		Position.CashIn = pInvestorPosition->CashIn;
		Position.Commission = pInvestorPosition->Commission;
		Position.CloseProfit = pInvestorPosition->CloseProfit;
		Position.PositionProfit = pInvestorPosition->PositionProfit;
		Position.PreSettlementPrice = pInvestorPosition->PreSettlementPrice;
		Position.SettlementPrice = pInvestorPosition->SettlementPrice;
		Position.TradingDay = pInvestorPosition->TradingDay;
		Position.SettlementID = pInvestorPosition->SettlementID;
		Position.OpenCost = pInvestorPosition->OpenCost;
		Position.ExchangeMargin = pInvestorPosition->ExchangeMargin;
		Position.CombPosition = pInvestorPosition->CombPosition;
		Position.CombLongFrozen = pInvestorPosition->CombLongFrozen;
		Position.CombShortFrozen = pInvestorPosition->CombShortFrozen;
		Position.CloseProfitByDate = pInvestorPosition->CloseProfitByDate;
		Position.CloseProfitByTrade = pInvestorPosition->CloseProfitByTrade;
		Position.TodayPosition = pInvestorPosition->TodayPosition;
		Position.MarginRateByMoney = pInvestorPosition->MarginRateByMoney;
		Position.MarginRateByVolume = pInvestorPosition->MarginRateByVolume;
		Position.StrikeFrozen = pInvestorPosition->StrikeFrozen;
		Position.StrikeFrozenAmount = pInvestorPosition->StrikeFrozenAmount;
		Position.AbandonFrozen = pInvestorPosition->AbandonFrozen;
		Position.ExchangeID = pInvestorPosition->ExchangeID;
		Position.YdStrikeFrozen = pInvestorPosition->YdStrikeFrozen;
		Position.InvestUnitID = pInvestorPosition->InvestUnitID;
		all_InvestorPosition.push_back(Position);
	}
	if (bIsLast)
	{
		this->all_InvestorPositions = all_InvestorPosition;
		m_AlreadyQryPosition = true;  //�ֲֲ�ѯ��������Ϊtrue����ֹ�����ٴβ����������
		Sleep(1000);
		get_CtpMsgDistributor()->OnResponse(ctp_msg::RspQryInvestorPosition, (LPARAM)&this->all_InvestorPositions);
		all_InvestorPosition.clear();
		if (m_AlreadyQryCommissionRate==false)
		{
			CThostFtdcQryInstrumentCommissionRateField req = { 0 };
			strcpy_s(req.BrokerID, m_BrokerInfo.id.c_str());
			strcpy_s(req.InvestorID, username.c_str());
			strcpy_s(req.InstrumentID, "");//���InstrumentID��գ��򷵻سֲֶ�Ӧ�ĺ�Լ��������
			int ret_code = m_pTradeApi->ReqQryInstrumentCommissionRate(&req, ++m_iReqestID);
			while (ret_code != 0)
			{
				Sleep(2000);
				ret_code = m_pTradeApi->ReqQryInstrumentCommissionRate(&req, ++m_iReqestID);
			}
		}
	}
}

void TradeSpi::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (IsErrorRspInfo(pRspInfo))
	{
		SetLastError(pRspInfo);//��������Ϣ�����ϲ�
		CString error_msg = pRspInfo->ErrorMsg;
		::MessageBox(NULL, error_msg, _T("����"), MB_OK);//������
		return;
	}
	if (bIsLast)
	{
		if (m_AlreadyQryCommissionRate==true)
		{
			//���ڲ�ѯ��������������Ժ�Լ�Ĳ�ѯ,�����ǳ����ֲֵ���������Ϸ���,���,һ����blsLast
			static InstrumentCommissionRate commission_rate = { 0 };
			strcpy_s(commission_rate.InstrumentID, pInstrumentCommissionRate->InstrumentID);
			commission_rate.InvestorRange = pInstrumentCommissionRate->InvestorRange;
			strcpy_s(commission_rate.BrokerID, pInstrumentCommissionRate->BrokerID);
			strcpy_s(commission_rate.InvestorID, pInstrumentCommissionRate->InvestorID);
			commission_rate.OpenRatioByMoney = pInstrumentCommissionRate->OpenRatioByMoney;
			commission_rate.OpenRatioByVolume = pInstrumentCommissionRate->OpenRatioByVolume;
			commission_rate.CloseRatioByMoney = pInstrumentCommissionRate->CloseRatioByMoney;
			commission_rate.OpenRatioByVolume = pInstrumentCommissionRate->OpenRatioByVolume;
			commission_rate.CloseTodayRatioByMoney = pInstrumentCommissionRate->CloseTodayRatioByMoney;
			commission_rate.CloseTodayRatioByVolume = pInstrumentCommissionRate->CloseTodayRatioByVolume;
			strcpy_s(commission_rate.ExchangeID, pInstrumentCommissionRate->ExchangeID);
			commission_rate.BizType = pInstrumentCommissionRate->BizType;
			strcpy_s(commission_rate.InvestUnitID, pInstrumentCommissionRate->InvestUnitID);
			get_CtpMsgDistributor()->OnResponse(ctp_msg::RspQryCommissionRate, (LPARAM)&commission_rate);
			return;
		}
		m_AlreadyQryCommissionRate = true;  //�����Ѳ�ѯ��������Ϊtrue����ֹ�����ٴβ����������
		if (m_AlreadyQryOrder==false)
		{
			CThostFtdcQryOrderField req = { 0 };
			strcpy_s(req.BrokerID, m_BrokerInfo.id.c_str());
			strcpy_s(req.InvestorID, username.c_str());
			int ret_code = m_pTradeApi->ReqQryOrder(&req, ++m_iReqestID);
			while (ret_code != 0)
			{
				Sleep(2000);
				ret_code = m_pTradeApi->ReqQryOrder(&req, ++m_iReqestID);
			}
		}
	}
}

void TradeSpi::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (IsErrorRspInfo(pRspInfo))
	{
		SetLastError(pRspInfo);//��������Ϣ�����ϲ�
		CString error_msg = pRspInfo->ErrorMsg;
		::MessageBox(NULL, error_msg, _T("����"), MB_OK);//������
		return;
	}
	static vector<Order> all_Order; 
	Order order;
	if (pOrder != NULL)
	{
		strcpy_s(order.BrokerID,  pOrder->BrokerID);
		strcpy_s(order.InvestorID, pOrder->InvestorID);
		strcpy_s(order.InstrumentID, pOrder->InstrumentID);
		strcpy_s(order.OrderRef, pOrder->OrderRef);
		strcpy_s(order.UserID, pOrder->UserID);
		order.OrderPriceType=pOrder->OrderPriceType;
		order.Direction = pOrder->Direction;
		strcpy_s(order.CombOffsetFlag, pOrder->CombOffsetFlag);
		strcpy_s(order.CombHedgeFlag, pOrder->CombHedgeFlag);
		order.LimitPrice = pOrder->LimitPrice;
		order.VolumeTotalOriginal = pOrder->VolumeTotalOriginal;
		order.TimeCondition = pOrder->TimeCondition;
		strcpy_s(order.GTDDate, pOrder->GTDDate);
		order.VolumeCondition = pOrder->VolumeCondition;
		order.MinVolume = pOrder->MinVolume;
		order.ContingentCondition = pOrder->ContingentCondition;
		order.StopPrice = pOrder->StopPrice;
		order.ForceCloseReason = pOrder->ForceCloseReason;
		order.IsAutoSuspend = pOrder->IsAutoSuspend;
		strcpy_s(order.BusinessUnit, pOrder->BusinessUnit);
		order.RequestID = pOrder->RequestID;
		strcpy_s(order.OrderLocalID, pOrder->OrderLocalID);
		strcpy_s(order.ExchangeID, pOrder->ExchangeID);
		strcpy_s(order.ParticipantID, pOrder->ParticipantID);
		strcpy_s(order.ClientID, pOrder->ClientID);
		strcpy_s(order.ExchangeInstID, pOrder->ExchangeInstID);
		strcpy_s(order.TraderID, pOrder->TraderID);
		order.InstallID = pOrder->InstallID;
		order.OrderSubmitStatus = pOrder->OrderSubmitStatus;
		order.NotifySequence = pOrder->NotifySequence;
		strcpy_s(order.TradingDay, pOrder->TradingDay);
		order.SettlementID = pOrder->SettlementID;
		strcpy_s(order.OrderSysID, pOrder->OrderSysID);
		order.OrderSource = pOrder->OrderSource;
		order.OrderStatus = pOrder->OrderStatus;
		order.OrderType = pOrder->OrderType;
		order.VolumeTraded = pOrder->VolumeTraded;
		order.VolumeTotal = pOrder->VolumeTotal;
		strcpy_s(order.InsertDate, pOrder->InsertDate);
		strcpy_s(order.InsertTime, pOrder->InsertTime);
		strcpy_s(order.ActiveTime, pOrder->ActiveTime);
		strcpy_s(order.SuspendTime, pOrder->SuspendTime);
		strcpy_s(order.UpdateTime, pOrder->UpdateTime);
		strcpy_s(order.CancelTime, pOrder->CancelTime);
		strcpy_s(order.ActiveTraderID, pOrder->ActiveTraderID);
		strcpy_s(order.ClearingPartID, pOrder->ClearingPartID);
		order.SequenceNo = pOrder->SequenceNo;
		order.FrontID = pOrder->FrontID;
		order.SessionID = pOrder->SessionID;
		strcpy_s(order.UserProductInfo ,pOrder->UserProductInfo);
		strcpy_s(order.StatusMsg, pOrder->StatusMsg);
		order.UserForceClose = pOrder->UserForceClose;
		strcpy_s(order.ActiveUserID, pOrder->ActiveUserID);
		order.BrokerOrderSeq = pOrder->BrokerOrderSeq;
		strcpy_s(order.RelativeOrderSysID, pOrder->RelativeOrderSysID);
		order.ZCETotalTradedVolume = pOrder->ZCETotalTradedVolume;
		order.IsSwapOrder = pOrder->IsSwapOrder;
		strcpy_s(order.BranchID,pOrder->BranchID);
		strcpy_s(order.InvestUnitID, pOrder->InvestUnitID);
		strcpy_s(order.AccountID, pOrder->AccountID);
		strcpy_s(order.CurrencyID, pOrder->CurrencyID);
		strcpy_s(order.IPAddress, pOrder->IPAddress);
		strcpy_s(order.MacAddress, pOrder->MacAddress);
		all_Order.push_back(order);
	}
	if (bIsLast)
	{
		get_CtpMsgDistributor()->OnResponse(ctp_msg::RspQryOrder, (LPARAM)&all_Order);
		m_AlreadyQryOrder = true; //ί�в�ѯ����,��¼��,��ֹ�ٴβ�ѯ
		//��������ѯ�ɽ�
		CThostFtdcQryTradeField req = { 0 };
		strcpy_s(req.BrokerID, m_BrokerInfo.id.c_str());
		strcpy_s(req.InvestorID, username.c_str());
		int ret_code = m_pTradeApi->ReqQryTrade(&req, ++m_iReqestID);
		while (ret_code != 0)
		{
			Sleep(2000);
			ret_code = m_pTradeApi->ReqQryTrade(&req, ++m_iReqestID);
		}
	}
}

void TradeSpi::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (IsErrorRspInfo(pRspInfo))
	{
		SetLastError(pRspInfo);//��������Ϣ�����ϲ�
		CString error_msg = pRspInfo->ErrorMsg;
		::MessageBox(NULL, error_msg, _T("����"), MB_OK);//������
		return;
	}
	static vector<Trade> all_Trade; //ע��Ҫ��static����ֹ��ʱ����
	Trade trade;
	if (pTrade != NULL)
	{
		strcpy_s(trade.BrokerID, pTrade->BrokerID);
		strcpy_s(trade.InvestorID, pTrade->InvestorID);
		strcpy_s(trade.InstrumentID, pTrade->InstrumentID);
		strcpy_s(trade.OrderRef, pTrade->OrderRef);
		strcpy_s(trade.UserID, pTrade->UserID);
		strcpy_s(trade.ExchangeID, pTrade->ExchangeID);
		strcpy_s(trade.TradeID, pTrade->TradeID);
		trade.Direction = pTrade->Direction;
		strcpy_s(trade.OrderSysID, pTrade->OrderSysID);
		strcpy_s(trade.ParticipantID, pTrade->ParticipantID);
		strcpy_s(trade.ClientID, pTrade->ClientID);
		trade.TradingRole = pTrade->TradingRole;
		strcpy_s(trade.ExchangeInstID, pTrade->ExchangeInstID);
		trade.OffsetFlag = pTrade->OffsetFlag; 
		trade.HedgeFlag = pTrade->HedgeFlag;
		trade.Price = pTrade->Price;
		trade.Volume = pTrade->Volume;
		strcpy_s(trade.TradeDate, pTrade->TradeDate);
		strcpy_s(trade.TradeTime, pTrade->TradeTime);
		trade.TradeType = pTrade->TradeType;
		trade.PriceSource = pTrade->PriceSource;
		strcpy_s(trade.TradeID, pTrade->TradeID);
		strcpy_s(trade.OrderLocalID, pTrade->OrderLocalID);
		strcpy_s(trade.ClearingPartID, pTrade->ClearingPartID);
		strcpy_s(trade.BusinessUnit, pTrade->BusinessUnit);
		trade.SequenceNo = pTrade->SequenceNo;
		strcpy_s(trade.TradingDay, pTrade->TradingDay);
		trade.SettlementID = pTrade->SettlementID;
		trade.BrokerOrderSeq = pTrade->BrokerOrderSeq;
		trade.TradeSource = pTrade->TradeSource;
		strcpy_s(trade.InvestUnitID, pTrade->InvestUnitID);
		all_Trade.push_back(trade);
	}
	if (bIsLast)
	{
		get_CtpMsgDistributor()->OnResponse(ctp_msg::RspQryTrade, (LPARAM)&all_Trade);
		//��ֲ���ϸ
		CThostFtdcQryInvestorPositionDetailField req = { 0 };
		strcpy_s(req.BrokerID, m_BrokerInfo.id.c_str());
		strcpy_s(req.InvestorID, username.c_str());
		int ret_code = m_pTradeApi->ReqQryInvestorPositionDetail(&req, ++m_iReqestID);
		while (ret_code != 0)
		{
			Sleep(2000);
			ret_code = m_pTradeApi->ReqQryInvestorPositionDetail(&req, ++m_iReqestID);
		}
	}
}

void TradeSpi::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (IsErrorRspInfo(pRspInfo))
	{
		SetLastError(pRspInfo);//��������Ϣ�����ϲ�
		CString error_msg = pRspInfo->ErrorMsg;
		::MessageBox(NULL, error_msg, _T("����"), MB_OK);//������
		return;
	}
	static vector<InvestorPositionDetail> all_PositionDetail; //ע��Ҫ��static����ֹ��ʱ����
	InvestorPositionDetail positionDetail;
	if (pInvestorPositionDetail != NULL)
	{
		memset(&positionDetail, 0, sizeof(positionDetail));
		strcpy_s(positionDetail.BrokerID, pInvestorPositionDetail->BrokerID);
		strcpy_s(positionDetail.InvestorID, pInvestorPositionDetail->InvestorID);
		strcpy_s(positionDetail.InstrumentID, pInvestorPositionDetail->InstrumentID);
		positionDetail.HedgeFlag = pInvestorPositionDetail->HedgeFlag;
		positionDetail.Direction = pInvestorPositionDetail->Direction;
		strcpy_s(positionDetail.OpenDate, pInvestorPositionDetail->OpenDate);
		strcpy_s(positionDetail.TradeID, pInvestorPositionDetail->TradeID);
		positionDetail.Volume = pInvestorPositionDetail->Volume;
		positionDetail.OpenPrice = pInvestorPositionDetail->OpenPrice;
		strcpy_s(positionDetail.TradingDay, pInvestorPositionDetail->TradingDay);
		positionDetail.SettlementID = pInvestorPositionDetail->SettlementID;
		positionDetail.TradeType = pInvestorPositionDetail->TradeType;
		strcpy_s(positionDetail.CombInstrumentID , pInvestorPositionDetail->CombInstrumentID);
		strcpy_s(positionDetail.ExchangeID, pInvestorPositionDetail->ExchangeID);
		positionDetail.CloseProfitByDate = pInvestorPositionDetail->CloseProfitByDate;
		positionDetail.CloseProfitByTrade = pInvestorPositionDetail->CloseProfitByTrade;
		positionDetail.PositionProfitByDate = pInvestorPositionDetail->PositionProfitByDate;
		positionDetail.PositionProfitByTrade = pInvestorPositionDetail->PositionProfitByTrade;
		positionDetail.Margin = pInvestorPositionDetail->Margin;
		positionDetail.ExchMargin = pInvestorPositionDetail->ExchMargin;
		positionDetail.MarginRateByMoney = pInvestorPositionDetail->MarginRateByMoney;
		positionDetail.MarginRateByVolume = pInvestorPositionDetail->MarginRateByVolume;
		positionDetail.LastSettlementPrice = pInvestorPositionDetail->LastSettlementPrice;
		positionDetail.SettlementPrice = pInvestorPositionDetail->SettlementPrice;
		positionDetail.CloseVolume = pInvestorPositionDetail->CloseVolume;
		positionDetail.CloseAmount = pInvestorPositionDetail->CloseAmount;
		strcpy_s(positionDetail.InvestUnitID, pInvestorPositionDetail->InvestUnitID);
		all_PositionDetail.push_back(positionDetail);
	}
	if (bIsLast)
	{
		this->all_InvestorPositionsDetail = all_PositionDetail;
		get_CtpMsgDistributor()->OnResponse(ctp_msg::RspQryInvestorPositionDetail, (LPARAM)&all_PositionDetail);
		//����ȥ����������ѯ
		if (m_AlreadyQryParkedOrder==false)
		{
			CThostFtdcQryParkedOrderField req = {0};
			strcpy_s(req.BrokerID, m_BrokerInfo.id.c_str());
			strcpy_s(req.InvestorID, username.c_str());
			int ret_code = m_pTradeApi->ReqQryParkedOrder(&req, ++m_iReqestID);
			while (ret_code != 0)
			{
				Sleep(2000);
				ret_code = m_pTradeApi->ReqQryParkedOrder(&req, ++m_iReqestID);
			}
		}
	}
}

void TradeSpi::OnRspQryParkedOrder(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (IsErrorRspInfo(pRspInfo))
	{
		SetLastError(pRspInfo);//��������Ϣ�����ϲ�
		CString error_msg = pRspInfo->ErrorMsg;
		::MessageBox(NULL, error_msg, _T("����"), MB_OK);//������
		return;
	}
	static vector<ParkedOrder> parkedorder_vector;
	ParkedOrder order;
	if (pParkedOrder!=NULL)
	{
		strcpy_s(order.BrokerID, pParkedOrder->BrokerID);
		strcpy_s(order.InvestorID, pParkedOrder->InvestorID);
		strcpy_s(order.InstrumentID, pParkedOrder->InstrumentID);
		strcpy_s(order.OrderRef, pParkedOrder->OrderRef);
		strcpy_s(order.UserID, pParkedOrder->UserID);
		order.OrderPriceType = pParkedOrder->OrderPriceType;
		order.Direction = pParkedOrder->Direction;
		strcpy_s(order.CombOffsetFlag, pParkedOrder->CombOffsetFlag);
		strcpy_s(order.CombHedgeFlag, pParkedOrder->CombHedgeFlag);
		order.LimitPrice = pParkedOrder->LimitPrice;
		order.VolumeTotalOriginal = pParkedOrder->VolumeTotalOriginal;
		order.TimeCondition = pParkedOrder->TimeCondition;
		strcpy_s(order.GTDDate, pParkedOrder->GTDDate);
		order.VolumeCondition = pParkedOrder->VolumeCondition;
		order.MinVolume = pParkedOrder->MinVolume;
		order.ContingentCondition = pParkedOrder->ContingentCondition;
		order.StopPrice = pParkedOrder->StopPrice;
		order.ForceCloseReason = pParkedOrder->ForceCloseReason;
		order.IsAutoSuspend = pParkedOrder->IsAutoSuspend;
		strcpy_s(order.BusinessUnit, pParkedOrder->BusinessUnit);
		order.RequestID = pParkedOrder->RequestID;
		strcpy_s(order.ExchangeID, pParkedOrder->ExchangeID);
		strcpy_s(order.ClientID, pParkedOrder->ClientID);
		order.UserForceClose = pParkedOrder->UserForceClose;
		strcpy_s(order.InvestUnitID, pParkedOrder->InvestUnitID);
		strcpy_s(order.AccountID, pParkedOrder->AccountID);
		strcpy_s(order.CurrencyID, pParkedOrder->CurrencyID);
		strcpy_s(order.IPAddress, pParkedOrder->IPAddress);
		strcpy_s(order.MacAddress, pParkedOrder->MacAddress);
		strcpy_s(order.ParkedOrderID, pParkedOrder->ParkedOrderID);
		order.UserType = pParkedOrder->UserType;
		order.Status = pParkedOrder->Status;
		order.ErrorID = pParkedOrder->ErrorID;
		strcpy_s(order.ErrorMsg, pParkedOrder->ErrorMsg);
		parkedorder_vector.push_back(order);
	}
	if (bIsLast)
	{
		m_AlreadyQryParkedOrder = true;//��ѯ�ɹ��ص���,��Ϊtrue;
		//��Ԥ�񵥵�errorIDΪ0,���ǳɹ���Ԥ�񱨵�,�ȴ��ɽ�
		get_CtpMsgDistributor()->OnResponse(ctp_msg::RspQryParkedOrder, (LPARAM)&parkedorder_vector);
		//����ȥ������������
		if (m_AlreadyQryParkedOrderAction == false)
		{
			CThostFtdcQryParkedOrderActionField req = { 0 };
			strcpy_s(req.BrokerID, m_BrokerInfo.id.c_str());
			strcpy_s(req.InvestorID, username.c_str());
			int ret_code = m_pTradeApi->ReqQryParkedOrderAction(&req, ++m_iReqestID);
			while (ret_code != 0)
			{
				Sleep(2000);
				ret_code = m_pTradeApi->ReqQryParkedOrderAction(&req, ++m_iReqestID);
			}
		}
	}
}

void TradeSpi::OnRspQryParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (IsErrorRspInfo(pRspInfo))
	{
		string msg_str = string(pRspInfo->ErrorMsg);
		CString msg = msg_str.c_str();
		::MessageBox(NULL, msg, _T("����"), MB_OK);//������
		return;
	}
	static vector<ParkedOrderAction> order_vector;
	ParkedOrderAction order;
	if (pParkedOrderAction != NULL)
	{
		strcpy_s(order.BrokerID, pParkedOrderAction->BrokerID);
		strcpy_s(order.InvestorID, pParkedOrderAction->InvestorID);
		order.OrderActionRef = pParkedOrderAction->OrderActionRef;
		strcpy_s(order.OrderRef, pParkedOrderAction->OrderRef);
		order.RequestID = pParkedOrderAction->RequestID;
		order.FrontID = pParkedOrderAction->FrontID;
		order.SessionID = pParkedOrderAction->SessionID;
		strcpy_s(order.ExchangeID, pParkedOrderAction->ExchangeID);
		strcpy_s(order.OrderSysID, pParkedOrderAction->OrderSysID);
		order.ActionFlag = pParkedOrderAction->ActionFlag;
		order.LimitPrice = pParkedOrderAction->LimitPrice;
		order.VolumeChange = pParkedOrderAction->VolumeChange;
		strcpy_s(order.UserID, pParkedOrderAction->UserID);
		strcpy_s(order.InstrumentID, pParkedOrderAction->InstrumentID);
		strcpy_s(order.ParkedOrderActionID, pParkedOrderAction->ParkedOrderActionID);
		order.UserType = pParkedOrderAction->UserType;
		order.Status = pParkedOrderAction->Status;
		order.ErrorID = pParkedOrderAction->ErrorID;
		strcpy_s(order.ErrorMsg, pParkedOrderAction->ErrorMsg);
		strcpy_s(order.InvestUnitID, pParkedOrderAction->InvestUnitID);
		strcpy_s(order.IPAddress, pParkedOrderAction->IPAddress);
		strcpy_s(order.MacAddress, pParkedOrderAction->MacAddress);
		order_vector.push_back(order);
	}
	if (bIsLast)
	{
		m_AlreadyQryParkedOrderAction = true;
		get_CtpMsgDistributor()->OnResponse(ctp_msg::RspQryParkedOrderAction, (LPARAM)&order_vector);
		//����ȥ������������
		return;
	}
}

void TradeSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{//��ִ��ReqOrderInsert�����ֶ���д����֮���CTP������ͨ���˽ӿڷ���
	string msg_str = string(pInputOrder->InstrumentID) + "(" + string(pInputOrder->ExchangeID) + ")" + ":" + "   " + string(pRspInfo->ErrorMsg);
	CString msg = msg_str.c_str();
	::MessageBox(NULL, msg, _T("ί�г���"), MB_OK);//������
}

void TradeSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{//��ִ��ReqOrderAction��������ʱ�����ֶ���д����֮���CTP������ͨ���˽ӿڷ���
	string msg_str = string(pInputOrderAction->InstrumentID) + "(" + string(pInputOrderAction->OrderSysID) + ")" + ":" + "   " + string(pRspInfo->ErrorMsg);
	CString msg = msg_str.c_str();
	::MessageBox(NULL, msg, _T("��������"), MB_OK);//������
}


void TradeSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//
}

void TradeSpi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{//����֪ͨ����ִ��ReqOrderInsert���ұ������յ���������ô˽ӿڣ�˽�����ر���
	//CString msg = pOrder->StatusMsg;
	//::MessageBox(NULL, msg, _T("OnRtnOrder"), MB_OK);//������
	static Order order;
	if (pOrder != NULL)
	{
		strcpy_s(order.BrokerID, pOrder->BrokerID);
		strcpy_s(order.InvestorID, pOrder->InvestorID);
		strcpy_s(order.InstrumentID, pOrder->InstrumentID);
		strcpy_s(order.OrderRef, pOrder->OrderRef);
		strcpy_s(order.UserID, pOrder->UserID);
		order.OrderPriceType = pOrder->OrderPriceType;
		order.Direction = pOrder->Direction;
		strcpy_s(order.CombOffsetFlag, pOrder->CombOffsetFlag);
		strcpy_s(order.CombHedgeFlag, pOrder->CombHedgeFlag);
		order.LimitPrice = pOrder->LimitPrice;
		order.VolumeTotalOriginal = pOrder->VolumeTotalOriginal;
		order.TimeCondition = pOrder->TimeCondition;
		strcpy_s(order.GTDDate, pOrder->GTDDate);
		order.VolumeCondition = pOrder->VolumeCondition;
		order.MinVolume = pOrder->MinVolume;
		order.ContingentCondition = pOrder->ContingentCondition;
		order.StopPrice = pOrder->StopPrice;
		order.ForceCloseReason = pOrder->ForceCloseReason;
		order.IsAutoSuspend = pOrder->IsAutoSuspend;
		strcpy_s(order.BusinessUnit, pOrder->BusinessUnit);
		order.RequestID = pOrder->RequestID;
		strcpy_s(order.OrderLocalID, pOrder->OrderLocalID);
		strcpy_s(order.ExchangeID, pOrder->ExchangeID);
		strcpy_s(order.ParticipantID, pOrder->ParticipantID);
		strcpy_s(order.ClientID, pOrder->ClientID);
		strcpy_s(order.ExchangeInstID, pOrder->ExchangeInstID);
		strcpy_s(order.TraderID, pOrder->TraderID);
		order.InstallID = pOrder->InstallID;
		order.OrderSubmitStatus = pOrder->OrderSubmitStatus;
		order.NotifySequence = pOrder->NotifySequence;
		strcpy_s(order.TradingDay, pOrder->TradingDay);
		order.SettlementID = pOrder->SettlementID;
		strcpy_s(order.OrderSysID, pOrder->OrderSysID);
		order.OrderSource = pOrder->OrderSource;
		order.OrderStatus = pOrder->OrderStatus;
		order.OrderType = pOrder->OrderType;
		order.VolumeTraded = pOrder->VolumeTraded;
		order.VolumeTotal = pOrder->VolumeTotal;
		strcpy_s(order.InsertDate, pOrder->InsertDate);
		strcpy_s(order.InsertTime, pOrder->InsertTime);
		strcpy_s(order.ActiveTime, pOrder->ActiveTime);
		strcpy_s(order.SuspendTime, pOrder->SuspendTime);
		strcpy_s(order.UpdateTime, pOrder->UpdateTime);
		strcpy_s(order.CancelTime, pOrder->CancelTime);
		strcpy_s(order.ActiveTraderID, pOrder->ActiveTraderID);
		strcpy_s(order.ClearingPartID, pOrder->ClearingPartID);
		order.SequenceNo = pOrder->SequenceNo;
		order.FrontID = pOrder->FrontID;
		order.SessionID = pOrder->SessionID;
		strcpy_s(order.UserProductInfo, pOrder->UserProductInfo);
		strcpy_s(order.StatusMsg, pOrder->StatusMsg);
		order.UserForceClose = pOrder->UserForceClose;
		strcpy_s(order.ActiveUserID, pOrder->ActiveUserID);
		order.BrokerOrderSeq = pOrder->BrokerOrderSeq;
		strcpy_s(order.RelativeOrderSysID, pOrder->RelativeOrderSysID);
		order.ZCETotalTradedVolume = pOrder->ZCETotalTradedVolume;
		order.IsSwapOrder = pOrder->IsSwapOrder;
		strcpy_s(order.BranchID, pOrder->BranchID);
		strcpy_s(order.InvestUnitID, pOrder->InvestUnitID);
		strcpy_s(order.AccountID, pOrder->AccountID);
		strcpy_s(order.CurrencyID, pOrder->CurrencyID);
		strcpy_s(order.IPAddress, pOrder->IPAddress);
		strcpy_s(order.MacAddress, pOrder->MacAddress);
	}
	get_CtpMsgDistributor()->OnResponse(ctp_msg::RtnOrder, (LPARAM)&order);
}

void TradeSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{//�ɽ�֪ͨ�������������гɽ���ͨ���˽ӿڷ��ء�˽����
	static Trade trade; //ע��Ҫ��static����ֹ��ʱ����
	if (pTrade != NULL)
	{
		strcpy_s(trade.BrokerID, pTrade->BrokerID);
		strcpy_s(trade.InvestorID, pTrade->InvestorID);
		strcpy_s(trade.InstrumentID, pTrade->InstrumentID);
		strcpy_s(trade.OrderRef, pTrade->OrderRef);
		strcpy_s(trade.UserID, pTrade->UserID);
		strcpy_s(trade.ExchangeID, pTrade->ExchangeID);
		strcpy_s(trade.TradeID, pTrade->TradeID);
		trade.Direction = pTrade->Direction;
		strcpy_s(trade.OrderSysID, pTrade->OrderSysID);
		strcpy_s(trade.ParticipantID, pTrade->ParticipantID);
		strcpy_s(trade.ClientID, pTrade->ClientID);
		trade.TradingRole = pTrade->TradingRole;
		strcpy_s(trade.ExchangeInstID, pTrade->ExchangeInstID);
		trade.OffsetFlag = pTrade->OffsetFlag;
		trade.HedgeFlag = pTrade->HedgeFlag;
		trade.Price = pTrade->Price;
		trade.Volume = pTrade->Volume;
		strcpy_s(trade.TradeDate, pTrade->TradeDate);
		strcpy_s(trade.TradeTime, pTrade->TradeTime);
		trade.TradeType = pTrade->TradeType;
		trade.PriceSource = pTrade->PriceSource;
		strcpy_s(trade.TradeID, pTrade->TradeID);
		strcpy_s(trade.OrderLocalID, pTrade->OrderLocalID);
		strcpy_s(trade.ClearingPartID, pTrade->ClearingPartID);
		strcpy_s(trade.BusinessUnit, pTrade->BusinessUnit);
		trade.SequenceNo = pTrade->SequenceNo;
		strcpy_s(trade.TradingDay, pTrade->TradingDay);
		trade.SettlementID = pTrade->SettlementID;
		trade.BrokerOrderSeq = pTrade->BrokerOrderSeq;
		trade.TradeSource = pTrade->TradeSource;
		strcpy_s(trade.InvestUnitID, pTrade->InvestUnitID);
		get_CtpMsgDistributor()->OnResponse(ctp_msg::RtnTrade, (LPARAM)&trade);
	}
}

void TradeSpi::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{//��ctp���ί�з��ִ���ʱ������Ͷ���߷���onRspInsertOrder��ͬʱ����OnErrRtnOrder����ؽ���Ա����ΪͶ���߿ɲ����Ĵ˴�
	//CString msg = pRspInfo->ErrorMsg;
	//::MessageBox(NULL, msg, _T("OnErrRtnOrderInsert"), MB_OK);//������
}

void TradeSpi::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
{
	string msg_str = string(pOrderAction->InstrumentID) + "(" + string(pOrderAction->OrderSysID) + ")" + ":" + "   " + string(pRspInfo->ErrorMsg);
	CString msg = msg_str.c_str();
	::MessageBox(NULL, msg, _T("��������"), MB_OK);//������
}


void TradeSpi::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (IsErrorRspInfo(pRspInfo))
	{
		SetLastError(pRspInfo);//��������Ϣ�����ϲ�
		CString error_msg = pRspInfo->ErrorMsg;
		::MessageBox(NULL, error_msg, _T("����"), MB_OK);//������
		return;
	}
	if (bIsLast && pInstrumentMarginRate!=NULL)
	{//�����ǰ���Լ��ѯ, �������ֲ���Ϸ���,Ϊ��һ����bIsLast
		static MarginRate margin_rate = { 0 };
		strcpy_s(margin_rate.BrokerID, pInstrumentMarginRate->BrokerID);
		strcpy_s(margin_rate.InstrumentID, pInstrumentMarginRate->InstrumentID);
		strcpy_s(margin_rate.InvestorID, pInstrumentMarginRate->InvestorID);
		margin_rate.InvestorRange = pInstrumentMarginRate->InvestorRange;
		margin_rate.HedgeFlag = pInstrumentMarginRate->HedgeFlag;
		margin_rate.LongMarginRatioByMoney = pInstrumentMarginRate->LongMarginRatioByMoney;
		margin_rate.LongMarginRatioByVolume = pInstrumentMarginRate->LongMarginRatioByVolume;
		margin_rate.ShortMarginRatioByMoney = pInstrumentMarginRate->ShortMarginRatioByMoney;
		margin_rate.ShortMarginRatioByVolume = pInstrumentMarginRate->ShortMarginRatioByVolume;
		margin_rate.IsRelative = pInstrumentMarginRate->IsRelative;
		strcpy_s(margin_rate.ExchangeID, pInstrumentMarginRate->ExchangeID);
		strcpy_s(margin_rate.InvestUnitID, pInstrumentMarginRate->InvestUnitID);
		get_CtpMsgDistributor()->OnResponse(ctp_msg::RspQryMarginRate, (LPARAM)&margin_rate);
		return;
	}
}

void TradeSpi::OnRspQryExchangeRate(CThostFtdcExchangeRateField *pExchangeRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (IsErrorRspInfo(pRspInfo))
	{
		SetLastError(pRspInfo);//��������Ϣ�����ϲ�
		CString error_msg = pRspInfo->ErrorMsg;
		::MessageBox(NULL, error_msg, _T("����"), MB_OK);//������
		return;
	}
	if (bIsLast && pExchangeRate!=NULL)
	{
		static _ExchangeRate exchange_rate = { 0 };
		if (pExchangeRate->BrokerID == NULL)
		{//��pExchangeRate->BrokerIDΪNULL,˵�����ص���Ϣ�ṹ��������,�˴����쳣����,������������
			strcpy_s(exchange_rate.BrokerID, "---");
			strcpy_s(exchange_rate.FromCurrencyID, "---");
			exchange_rate.FromCurrencyUnit = 0;
			strcpy_s(exchange_rate.ToCurrencyID, "---");
			exchange_rate.ExchangeRate = 0.00;
			get_CtpMsgDistributor()->OnResponse(ctp_msg::RspQryExchangeRate, (LPARAM)&exchange_rate);
			return;
		}
		strcpy_s(exchange_rate.BrokerID, pExchangeRate->BrokerID);
		strcpy_s(exchange_rate.FromCurrencyID, pExchangeRate->FromCurrencyID);
		exchange_rate.FromCurrencyUnit = pExchangeRate->FromCurrencyUnit;
		strcpy_s(exchange_rate.ToCurrencyID, pExchangeRate->ToCurrencyID);
		exchange_rate.ExchangeRate = pExchangeRate->ExchangeRate;
		get_CtpMsgDistributor()->OnResponse(ctp_msg::RspQryExchangeRate, (LPARAM)&exchange_rate);
		return;
	}
}

void TradeSpi::OnRspQryInstrumentOrderCommRate(CThostFtdcInstrumentOrderCommRateField *pInstrumentOrderCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (IsErrorRspInfo(pRspInfo))
	{
		SetLastError(pRspInfo);//��������Ϣ�����ϲ�
		CString error_msg = pRspInfo->ErrorMsg;
		::MessageBox(NULL, error_msg, _T("����"), MB_OK);//������
		return;
	}
	if (bIsLast && pInstrumentOrderCommRate!=NULL)
	{
		static OrderCommRate order_commrate = { 0 };
		if (pInstrumentOrderCommRate->InstrumentID == NULL)
		{
			strcpy_s(order_commrate.BrokerID, "");
			strcpy_s(order_commrate.InstrumentID, "");
			order_commrate.OrderCommByVolume = 0;
			order_commrate.OrderActionCommByVolume = 0;
			get_CtpMsgDistributor()->OnResponse(ctp_msg::RspQryInstrumentOrderCommRate, (LPARAM)&order_commrate);//��Ϣ�ź���Я����ӦΪҪ�ṩ����Ϣ
			return;
		}
		strcpy_s(order_commrate.BrokerID, pInstrumentOrderCommRate->BrokerID);
		strcpy_s(order_commrate.InstrumentID, pInstrumentOrderCommRate->InstrumentID);
		order_commrate.OrderCommByVolume = pInstrumentOrderCommRate->OrderCommByVolume;
		order_commrate.OrderActionCommByVolume = pInstrumentOrderCommRate->OrderActionCommByVolume;
		get_CtpMsgDistributor()->OnResponse(ctp_msg::RspQryInstrumentOrderCommRate, (LPARAM)&order_commrate);//��Ϣ�ź���Я����ӦΪҪ�ṩ����Ϣ
		return;
	}
}

void TradeSpi::OnRspQryProduct(CThostFtdcProductField *pProduct, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (IsErrorRspInfo(pRspInfo))
	{
		SetLastError(pRspInfo);//��������Ϣ�����ϲ�
		CString error_msg = pRspInfo->ErrorMsg;
		::MessageBox(NULL, error_msg, _T("����"), MB_OK);//������
		return;
	}
	if (pProduct!=NULL)
	{
		static vector<Product_InvaildInstrument> all_product_invaildInstrument;
		Product_InvaildInstrument invaildInstrument = { 0 };
		strcpy_s(invaildInstrument.ProductID, pProduct->ProductID);
		strcpy_s(invaildInstrument.ProductName, pProduct->ProductName);
		strcpy_s(invaildInstrument.ExchangeID, pProduct->ExchangeID);
		invaildInstrument.ProductClass = pProduct->ProductClass;
		invaildInstrument.VolumeMultiple = pProduct->VolumeMultiple;
		invaildInstrument.PriceTick = pProduct->PriceTick;
		invaildInstrument.MaxMarketOrderVolume = pProduct->MaxMarketOrderVolume;
		invaildInstrument.MinMarketOrderVolume = pProduct->MinMarketOrderVolume;
		invaildInstrument.MaxLimitOrderVolume = pProduct->MaxLimitOrderVolume;
		invaildInstrument.MinLimitOrderVolume = pProduct->MinLimitOrderVolume;
		invaildInstrument.PositionType = pProduct->PositionType;
		invaildInstrument.PositionDateType = pProduct->PositionDateType;
		invaildInstrument.CloseDealType = pProduct->CloseDealType;
		strcpy_s(invaildInstrument.TradeCurrencyID, pProduct->TradeCurrencyID);
		invaildInstrument.MortgageFundUseRange = pProduct->MortgageFundUseRange;
		strcpy_s(invaildInstrument.ExchangeProductID, pProduct->ExchangeProductID);
		invaildInstrument.UnderlyingMultiple = pProduct->UnderlyingMultiple;
		all_product_invaildInstrument.push_back(invaildInstrument);
		if (bIsLast)
		{
			get_CtpMsgDistributor()->OnResponse(ctp_msg::RspQryProduct, (LPARAM)&all_product_invaildInstrument);
			return;
		}
	}
}

void TradeSpi::OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (IsErrorRspInfo(pRspInfo))
	{
		SetLastError(pRspInfo);//��������Ϣ�����ϲ�
		CString error_msg = pRspInfo->ErrorMsg;
		::MessageBox(NULL, error_msg, _T("����"), MB_OK);//������
		return;
	}
	if (bIsLast && pDepthMarketData!=NULL)
	{
		static DepthMarketData data = { 0 };
		strcpy_s(data.InstrumentID, pDepthMarketData->InstrumentID);
		data.OpenInterest = pDepthMarketData->OpenInterest;
		get_CtpMsgDistributor()->OnResponse(ctp_msg::RspQryDepthMarketData, (LPARAM)&data);
		return;
	}
}

void TradeSpi::OnRspBatchOrderAction(CThostFtdcInputBatchOrderActionField *pInputBatchOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{//���ֶ���д����֮���CTP������ͨ���˽ӿڷ���
	/*
	string msg_str = string(pRspInfo->ErrorMsg);
	CString msg = msg_str.c_str();
	::MessageBox(NULL, msg, _T("ȫ������"), MB_OK);//������
	*/
}

void TradeSpi::OnErrRtnBatchOrderAction(CThostFtdcBatchOrderActionField *pBatchOrderAction, CThostFtdcRspInfoField *pRspInfo)
{//����������
	string msg_str = string(pBatchOrderAction->ActionDate) + "  " + string(pBatchOrderAction->ActionTime) + ":" + "   " + string(pBatchOrderAction->StatusMsg);
	CString msg = msg_str.c_str();
	::MessageBox(NULL, msg, _T("ȫ������"), MB_OK);//������
}

void TradeSpi::OnRspQryNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (IsErrorRspInfo(pRspInfo))
	{
		SetLastError(pRspInfo);//��������Ϣ�����ϲ�
		CString error_msg = pRspInfo->ErrorMsg;
		::MessageBox(NULL, error_msg, _T("����"), MB_OK);//������
		return;
	}
	if (pNotice!=NULL)
	{
		static vector<Notice> notice_vector;
		Notice notice;
		strcpy_s(notice.BrokerID, pNotice->BrokerID);
		strcpy_s(notice.Content, pNotice->Content);
		strcpy_s(notice.SequenceLabel, pNotice->SequenceLabel);
		notice_vector.push_back(notice);
		if (bIsLast)
		{
			get_CtpMsgDistributor()->OnResponse(ctp_msg::RspQryNotice, (LPARAM)&notice_vector);//��Ϣ�ź���Я����ӦΪҪ�ṩ����Ϣ
		}
	}
}


void TradeSpi::OnRspQryTradingNotice(CThostFtdcTradingNoticeField *pTradingNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (IsErrorRspInfo(pRspInfo))
	{
		SetLastError(pRspInfo);//��������Ϣ�����ϲ�
		CString error_msg = pRspInfo->ErrorMsg;
		::MessageBox(NULL, error_msg, _T("����"), MB_OK);//������
		return;
	}
	//ע���п���pTradingNoticeΪ�����Ҳ��ɶ�����Ϣ,��ͨ��pTradingNotice->BrokerID���ж�
	if (pTradingNotice!=NULL)
	{
		static vector<TradingNotice> tradingnotice_vector;
		TradingNotice tradingnotice;
		strcpy_s(tradingnotice.BrokerID, pTradingNotice->BrokerID);
		tradingnotice.InvestorRange = pTradingNotice->InvestorRange;
		strcpy_s(tradingnotice.InvestorID, pTradingNotice->InvestorID);
		tradingnotice.SequenceSeries = pTradingNotice->SequenceSeries;
		strcpy_s(tradingnotice.UserID, pTradingNotice->UserID);
		strcpy_s(tradingnotice.SendTime, pTradingNotice->SendTime);
		tradingnotice.SequenceNo = pTradingNotice->SequenceNo;
		strcpy_s(tradingnotice.FieldContent, pTradingNotice->FieldContent);
		strcpy_s(tradingnotice.InvestUnitID, pTradingNotice->InvestUnitID);
		tradingnotice_vector.push_back(tradingnotice);
		if (bIsLast)
		{
			get_CtpMsgDistributor()->OnResponse(ctp_msg::RspQryTradingNotice, (LPARAM)&tradingnotice_vector);
		}
	}
}

void TradeSpi::OnRspParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (IsErrorRspInfo(pRspInfo))
	{
		string msg_str = string(pRspInfo->ErrorMsg);
		CString msg = msg_str.c_str();
		::MessageBox(NULL, msg, _T("Ԥ�����"), MB_OK);//������
		return;
	}
	static ParkedOrder order;
	if (bIsLast && pParkedOrder!=NULL)
	{
		strcpy_s(order.BrokerID, pParkedOrder->BrokerID);
		strcpy_s(order.InvestorID, pParkedOrder->InvestorID);
		strcpy_s(order.InstrumentID, pParkedOrder->InstrumentID);
		strcpy_s(order.OrderRef, pParkedOrder->OrderRef);
		strcpy_s(order.UserID, pParkedOrder->UserID);
		order.OrderPriceType = pParkedOrder->OrderPriceType;
		order.Direction = pParkedOrder->Direction;
		strcpy_s(order.CombOffsetFlag, pParkedOrder->CombOffsetFlag);
		strcpy_s(order.CombHedgeFlag, pParkedOrder->CombHedgeFlag);
		order.LimitPrice = pParkedOrder->LimitPrice;
		order.VolumeTotalOriginal = pParkedOrder->VolumeTotalOriginal;
		order.TimeCondition = pParkedOrder->TimeCondition;
		strcpy_s(order.GTDDate, pParkedOrder->GTDDate);
		order.VolumeCondition = pParkedOrder->VolumeCondition;
		order.MinVolume = pParkedOrder->MinVolume;
		order.ContingentCondition = pParkedOrder->ContingentCondition;
		order.StopPrice = pParkedOrder->StopPrice;
		order.ForceCloseReason = pParkedOrder->ForceCloseReason;
		order.IsAutoSuspend = pParkedOrder->IsAutoSuspend;
		strcpy_s(order.BusinessUnit, pParkedOrder->BusinessUnit);
		order.RequestID = pParkedOrder->RequestID;
		strcpy_s(order.ExchangeID, pParkedOrder->ExchangeID);
		strcpy_s(order.ClientID, pParkedOrder->ClientID);
		order.UserForceClose = pParkedOrder->UserForceClose;
		strcpy_s(order.InvestUnitID, pParkedOrder->InvestUnitID);
		strcpy_s(order.AccountID, pParkedOrder->AccountID);
		strcpy_s(order.CurrencyID, pParkedOrder->CurrencyID);
		strcpy_s(order.IPAddress, pParkedOrder->IPAddress);
		strcpy_s(order.MacAddress, pParkedOrder->MacAddress);
		strcpy_s(order.ParkedOrderID, pParkedOrder->ParkedOrderID);
		order.UserType = pParkedOrder->UserType;
		order.Status = pParkedOrder->Status;
		order.ErrorID = pParkedOrder->ErrorID;
		strcpy_s(order.ErrorMsg, pParkedOrder->ErrorMsg);
		get_CtpMsgDistributor()->OnResponse(ctp_msg::RspParkedOrderInsert, (LPARAM)&order);
	}
}

void TradeSpi::OnRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (IsErrorRspInfo(pRspInfo))
	{
		string msg_str = string(pRspInfo->ErrorMsg);
		CString msg = msg_str.c_str();
		::MessageBox(NULL, msg, _T("Ԥ�񳷵�����"), MB_OK);//������
		return;
	}
	if (pRemoveParkedOrder!=NULL)
	{
		static Remove_ParkedOrder  order;
		strcpy_s(order.BrokerID, pRemoveParkedOrder->BrokerID);
		strcpy_s(order.InvestorID, pRemoveParkedOrder->InvestorID);
		strcpy_s(order.InvestUnitID, pRemoveParkedOrder->InvestUnitID);
		strcpy_s(order.ParkedOrderID, pRemoveParkedOrder->ParkedOrderID);
		if (bIsLast)
		{
			get_CtpMsgDistributor()->OnResponse(ctp_msg::RspRemoveParkedOrder, (LPARAM)&order);
		}
	}
}

void TradeSpi::OnRspParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (IsErrorRspInfo(pRspInfo))
	{
		string msg_str = string(pRspInfo->ErrorMsg);
		CString msg = msg_str.c_str();
		::MessageBox(NULL, msg, _T("Ԥ�񳷵�����"), MB_OK);//������
		return;
	}
	static ParkedOrderAction order;
	if (pParkedOrderAction != NULL)
	{
		if (bIsLast)
		{
			strcpy_s(order.BrokerID, pParkedOrderAction->BrokerID);
			strcpy_s(order.InvestorID, pParkedOrderAction->InvestorID);
			order.OrderActionRef = pParkedOrderAction->OrderActionRef;
			strcpy_s(order.OrderRef, pParkedOrderAction->OrderRef);
			order.RequestID = pParkedOrderAction->RequestID;
			order.FrontID = pParkedOrderAction->FrontID;
			order.SessionID = pParkedOrderAction->SessionID;
			strcpy_s(order.ExchangeID, pParkedOrderAction->ExchangeID);
			strcpy_s(order.OrderSysID, pParkedOrderAction->OrderSysID);
			order.ActionFlag = pParkedOrderAction->ActionFlag;
			order.LimitPrice = pParkedOrderAction->LimitPrice;
			order.VolumeChange = pParkedOrderAction->VolumeChange;
			strcpy_s(order.UserID, pParkedOrderAction->UserID);
			strcpy_s(order.InstrumentID, pParkedOrderAction->InstrumentID);
			strcpy_s(order.ParkedOrderActionID, pParkedOrderAction->ParkedOrderActionID);
			order.UserType = pParkedOrderAction->UserType;
			order.Status = pParkedOrderAction->Status;
			order.ErrorID = pParkedOrderAction->ErrorID;
			strcpy_s(order.ErrorMsg, pParkedOrderAction->ErrorMsg);
			strcpy_s(order.InvestUnitID, pParkedOrderAction->InvestUnitID);
			strcpy_s(order.IPAddress, pParkedOrderAction->IPAddress);
			strcpy_s(order.MacAddress, pParkedOrderAction->MacAddress);
			get_CtpMsgDistributor()->OnResponse(ctp_msg::RspParkedOrderAction, (LPARAM)&order);
			return;
		}
	}
}

void TradeSpi::OnRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (IsErrorRspInfo(pRspInfo))
	{
		string msg_str = string(pRspInfo->ErrorMsg);
		CString msg = msg_str.c_str();
		::MessageBox(NULL, msg, _T("����"), MB_OK);//������
		return;
	}
	if (pRemoveParkedOrderAction!=NULL)
	{
		static Remove_ParkedOrderAction  order;
		strcpy_s(order.BrokerID, pRemoveParkedOrderAction->BrokerID);
		strcpy_s(order.InvestorID, pRemoveParkedOrderAction->InvestorID);
		strcpy_s(order.InvestUnitID, pRemoveParkedOrderAction->InvestUnitID);
		strcpy_s(order.ParkedOrderActionID, pRemoveParkedOrderAction->ParkedOrderActionID);
		if (bIsLast)
		{
			get_CtpMsgDistributor()->OnResponse(ctp_msg::RspRemoveParkedOrderAction, (LPARAM)&order);
		}
	}
}


void TradeSpi::Init(const ServerInfo& info)//��ǰ������ǰ��׼������
{
	if (m_bInited)
	{
		return;
	}
	m_BrokerInfo = info;//����������Ϣ�����������ܱ�����Աm_BrokerInfo��
	if (NULL == m_pTradeApi)
	{
		m_pTradeApi = CThostFtdcTraderApi::CreateFtdcTraderApi(info.name.c_str()); //���ݲ�ͬ��serverinfo�����в������ļ��������Ҫ��
	}
	m_pTradeApi->RegisterSpi(this);
	//��������ģʽ��Ϊ��Ҫ
	m_pTradeApi->SubscribePrivateTopic(THOST_TERT_QUICK);//ע��˽����
	m_pTradeApi->SubscribePublicTopic(THOST_TERT_QUICK);//ע�ṫ����
	char trade_front[128];
	sprintf_s(trade_front, "%s%s", m_BrokerInfo.protocol.c_str(), 
	m_BrokerInfo.trade_server_front[0].c_str());//����þ��������ж�����׷��������ã��˴���ѡ���һ�����׷���������
	m_pTradeApi->RegisterFront(trade_front);//ע��ǰ�˽��׷�������ַ
	m_bInited = true;
}

bool TradeSpi::Login(const string& username, const string& password)
{
	if (!m_bInited || username.empty() || password.empty() )
	{
		return false;
	}
	this->username = username;
	this->password = password;
	m_pTradeApi->Init();//����ǰ������
	return true;
}

bool TradeSpi::IsErrorRspInfo(CThostFtdcRspInfoField* pRspInfo)
{//���ص��Ƿ񷵻ش����Դ���ȷ���������Ĳ����Ƿ�ɹ�,Ϊͨ���ͺ���
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID !=0));//�ص���Ӧ���صĴ�����Ϣ�ṹ��pRspInfo��Ϊ���Ҵ��ڴ���,���д����
	return bResult;
}

void TradeSpi::SetLastError(CThostFtdcRspInfoField *pRspInfo)
{
	if (NULL == pRspInfo)//��pRspInfo������Ϣ��Ϊ��,˵��û�д���
	{
		return;
	}
	last_error.ErrorID = pRspInfo->ErrorID;
	last_error.ErrorMsg = pRspInfo->ErrorMsg;
	get_CtpMsgDistributor()->OnResponse(ctp_msg::RspError, (LPARAM)&last_error);//�����󴫵��ϲ�ȥ
}









