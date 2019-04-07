#include "stdafx.h"
#include "MdSpi.h"
#include "CtpService.h"
#include "CtpMsgDistributor.h"
#include <iostream>
#include <atlstr.h>//ʹ��cstring��Ҫ��ͷ�ļ�
#include "CtpMsg.h"
using namespace std;

#pragma warning(disable : 4996)

CMdSpi::CMdSpi()
	:m_iReqestID(0),
	m_bInited(false),
	m_pMarketApi(NULL)
{
}

CMdSpi::~CMdSpi()
{
}

bool CMdSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// ���ErrorID != 0, ˵���յ��˴������Ӧ
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	return bResult;
}

//******************************************************�ص��麯������*****************************************************************//
void CMdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo,int nRequestID, bool bIsLast)
{
	//
}

void CMdSpi::OnFrontDisconnected(int nReason)
{//һ���Ͽ����ӣ�������Ϣ���͸�Ӧ�ò㴦��
	get_CtpMsgDistributor()->OnResponse(ctp_msg::RspFrontDisconnected_Md, (LPARAM)NULL);
}
		
void CMdSpi::OnHeartBeatWarning(int nTimeLapse)
{
	//cerr << "--->>> nTimerLapse = " << nTimeLapse << endl;
}

void CMdSpi::OnFrontConnected()
{//ǰ�����ӳɹ���������¼
	ReqUserLogin();
}

void CMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		//����Ӧ�ò��¼�ɹ�
		static UserInfo user_info_md; //ע�������user_info������static��̬�ģ���Ϊ���Ǿֲ��������ں���ִ�����ᱻ���٣������ǵ�get_service_spi()->OnResponse�ǽ����ַ����Ӧ�ò�ģ����������ˣ��������ʾʱ����һ������
		strcpy_s(user_info_md.UserID, pRspUserLogin->UserID);
		strcpy_s(user_info_md.BrokerID, pRspUserLogin->BrokerID);
		strcpy_s(user_info_md.TradingDay, pRspUserLogin->TradingDay);
		strcpy_s(user_info_md.LoginTime, pRspUserLogin->LoginTime);
		strcpy_s(user_info_md.ExchangeTime, pRspUserLogin->SHFETime);
		user_info_md.FrontID = pRspUserLogin->FrontID;
		user_info_md.SessionID = pRspUserLogin->SessionID;
		strcpy_s(user_info_md.MaxOrderRef, pRspUserLogin->MaxOrderRef);
		strcpy_s(user_info_md.SystemName, pRspUserLogin->SystemName);
		strcpy_s(user_info_md.UserID, username.c_str());
		string systemname = "MarketHosting";
		strcpy_s(user_info_md.SystemName, systemname.c_str());
		if (bIsLast)
		{//����¼�ɹ�����Ϣ��֪Ӧ�ò�
			get_CtpMsgDistributor()->OnResponse(ctp_msg::RspUserLogined_Md, (LPARAM)&user_info_md);
		}
		//�����ϣ�Ӧ�ڵ�¼�ɹ��󣬿�ʼ�������飬��������Ĳ������õ�Ӧ�ò�UI��
	}
}

void CMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//
}

void CMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//
}

void CMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	static MarketData marketData;
	memset(&marketData, 0 ,sizeof(marketData));
	if (pDepthMarketData!=NULL)
	{
		strcpy_s(marketData.TradingDay, pDepthMarketData->TradingDay);
		strcpy_s(marketData.code, pDepthMarketData->InstrumentID);
		strcpy_s(marketData.ExchangeID, pDepthMarketData->ExchangeID);
		marketData.LastPrice = pDepthMarketData->LastPrice;
		marketData.PreSettlementPrice = pDepthMarketData->PreSettlementPrice;
		marketData.PreClosePrice = pDepthMarketData->PreClosePrice;
		marketData.PreOpenInterest = pDepthMarketData->PreOpenInterest;
		marketData.OpenPrice = pDepthMarketData->OpenPrice;
		marketData.HighestPrice = pDepthMarketData->HighestPrice;
		marketData.LowestPrice = pDepthMarketData->LowestPrice;
		marketData.Volume = pDepthMarketData->Volume;
		marketData.Turnover = pDepthMarketData->Turnover;
		marketData.OpenInterest = pDepthMarketData->OpenInterest;
		marketData.ClosePrice = pDepthMarketData->ClosePrice;
		marketData.SettlementPrice = pDepthMarketData->SettlementPrice;
		marketData.UpperLimitPrice = pDepthMarketData->UpperLimitPrice;
		marketData.LowerLimitPrice = pDepthMarketData->LowerLimitPrice;
		strcpy_s(marketData.UpdateTime, pDepthMarketData->UpdateTime);
		marketData.UpdateMillisec = pDepthMarketData->UpdateMillisec;
		marketData.AveragePrice = pDepthMarketData->AveragePrice;
		marketData.BidPrice = pDepthMarketData->BidPrice1;
		marketData.BidVolume = pDepthMarketData->BidVolume1;
		marketData.AskPrice = pDepthMarketData->AskPrice1;
		marketData.AskVolume = pDepthMarketData->AskVolume1;
		get_CtpMsgDistributor()->OnResponse(ctp_msg::RtnMarketData, (LPARAM)&marketData);
	}
}

void CMdSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//::MessageBox(NULL, _T("����ǳ�"), _T("�ǳ�"), MB_OK);//������
}

void CMdSpi::Init( const ServerInfo& info )
{
	if (m_bInited)
	{//������
		return ;
	}
	m_BrokerInfo=info;	//���澭������Ϣ
	if (NULL==m_pMarketApi)
	{//����Api����
		m_pMarketApi=CThostFtdcMdApi::CreateFtdcMdApi(info.name.c_str());//ע��con���ļ�������
	}
	m_pMarketApi->RegisterSpi(this);						// ע��spi�¼���
	char front_addr[128]={0};
	sprintf(front_addr,"%s%s",m_BrokerInfo.protocol.c_str(),m_BrokerInfo.market_server_front[0].c_str());
	m_pMarketApi->RegisterFront(front_addr);			
	m_bInited = true;		//m_bInited����ָǰ�����ӳɹ�������ָǰ������ǰ��׼�������Ѿ����
}

bool CMdSpi::Login( const string& username,const string& password )
{
	if (!m_bInited || username.empty() || password.empty())
	{
		return false;
	}
	this->username=username;		//�����û���
	this->password=password;		//��������
	m_pMarketApi->Init();				//����ǰ������
	return true;
}

void CMdSpi::ReqUserLogin()
{
	if (m_pMarketApi == NULL)
	{
		return;
	}
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_BrokerInfo.id.c_str());
	strcpy(req.UserID, username.c_str());
	strcpy(req.Password, password.c_str());
	int iResult = m_pMarketApi->ReqUserLogin(&req, ++m_iReqestID);
	while (iResult != 0)
	{
		Sleep(1000);
		iResult = m_pMarketApi->ReqUserLogin(&req, ++m_iReqestID);
	}
}

void CMdSpi::SubscribeMarketData(const vector<string>& instruments)
{
	if (instruments.size() <= 0)
	{//�����趩�ĵĺ�ԼΪ��
		return;
	}
	char** ppInstrumentID = new char*[instruments.size()];	//��̬����instruments.size()��char*ָ�룬ppInstrumentIDΪ����ָ�룬��ָ��ָ�������ָ��
	for (int i = 0; i < instruments.size(); i++)
	{
		ppInstrumentID[i] = new char[9];	//ָ��������ʾ����ppInstrumentID[i]Ϊchar*����
		strcpy(ppInstrumentID[i], instruments[i].c_str());
	}
	int iResult = m_pMarketApi->SubscribeMarketData(ppInstrumentID, instruments.size());
	while (iResult != 0)
	{//ֱ�����Ͷ�������ɹ�
		Sleep(2000);
		iResult = m_pMarketApi->SubscribeMarketData(ppInstrumentID, instruments.size());
	}
	//����������ͷ�ppInstrumentID[i]����Ϊchar���飬֮�����ͷ�ppInstrumentID����Ϊָ������
	for (int i = 0; i < instruments.size(); i++)
	{
		delete[]ppInstrumentID[i]; //����ͷŴ�ź�Լ����char���飬Ϊ��Ӧdelete[]��ÿ��ppInstrumentID[i]
	}
	delete[]ppInstrumentID;		//�ͷ�ָ�������ڴ棬Ϊ��Ӧdelete[]
	ppInstrumentID = NULL;
}

void CMdSpi::UnSubscribeMarketData(const vector<string>& instruments)
{
	if (instruments.size() <= 0)
	{//�������˶��ĺ�ԼΪ��
		return;
	}
	char** ppInstrumentID = new char*[instruments.size()];	//��̬����instruments.size()��char*ָ�룬ppInstrumentIDΪ����ָ�룬��ָ��ָ�������ָ��
	for (int i = 0; i < instruments.size(); i++)
	{
		ppInstrumentID[i] = new char[9];	//ָ��������ʾ����ppInstrumentID[i]Ϊchar*����
		strcpy(ppInstrumentID[i], instruments[i].c_str());
	}
	int iResult = m_pMarketApi->UnSubscribeMarketData(ppInstrumentID, instruments.size());
	while (iResult != 0)
	{//ֱ�������˶�����ɹ�
		Sleep(2000);
		iResult = m_pMarketApi->UnSubscribeMarketData(ppInstrumentID, instruments.size());
	}
	//����������ͷ�ppInstrumentID[i]����Ϊchar���飬֮�����ͷ�ppInstrumentID����Ϊָ������
	for (int i = 0; i < instruments.size(); i++)
	{
		delete[]ppInstrumentID[i]; //����ͷŴ�ź�Լ����char���飬Ϊ��Ӧdelete[]��ÿ��ppInstrumentID[i]
	}
	delete[]ppInstrumentID;		//�ͷ�ָ�������ڴ棬Ϊ��Ӧdelete[]
	ppInstrumentID = NULL;
}
