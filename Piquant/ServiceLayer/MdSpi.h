#ifndef _MD_SPI_H_
#define _MD_SPI_H_
#include "../../sdk/ThostFtdcMdApi.h"
#include "ServerInfo.h"

//������Ӧ
class CMdSpi : public CThostFtdcMdSpi
{
public:
	///����Ӧ��
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo,int nRequestID, bool bIsLast);

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	virtual void OnFrontDisconnected(int nReason);
		
	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
	virtual void OnHeartBeatWarning(int nTimeLapse);

	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();
	
	///��¼������Ӧ
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������Ӧ��
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///ȡ����������Ӧ��
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�������֪ͨ
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

	//�ǳ��ص�
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

public:
	CMdSpi();
	~CMdSpi();
	void Init(const ServerInfo& info);
	bool Login(const string& username,const string& password);
	void SubscribeMarketData(const vector<string>& instruments);
	void UnSubscribeMarketData(const vector<string>& instruments);

private:
	void ReqUserLogin();

public:
	CThostFtdcMdApi* GetMdApi(){ return m_pMarketApi; }	//ͨ��m_pMarketApi��ʵ��CTP���������ĵ���
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);

protected:
	CThostFtdcMdApi* m_pMarketApi;
	bool m_bInited;
	ServerInfo m_BrokerInfo;
	string username;
	string password;
	int m_iReqestID;
};
#endif