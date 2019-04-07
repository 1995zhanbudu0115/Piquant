#include "stdafx.h"
#include "IDataStore.h"
#include "CtpMsg.h"

#ifdef _WIN32
#if defined(_UNICODE) || defined(UNICODE)
#undef _UNICODE
#undef UNICODE
#define  OTL_ODBC_MSSQL_2008 //����Ҫͨ��OTL_ODBC_MSSQL_2008����궨����ʹotl�ⶨλ����ȷ�����ݿ������������
#define  OTL_EXCEPTION_IS_DERIVED_FROM_STD_EXCEPTION
#include "otlv4.h"
#endif
#define _UNICODE
#define UNICODE
#else
##include "otlv4.h"//�ڲ�ͬϵͳ�¶���꣬����windows�£���ֱ�Ӱ�����ͷ�ļ��򵥴���
#endif

/*
//��ʱ�����ú���
void insert(otl_connect& db)
{
otl_stream o(1, //buffer size should be ==1 always on Insert,
"insert into tb2 values(:f1<char[32]>, :f2<char[64]>, :f3<float>)",
db);
o << "IF1905" << "��ָ1905" << 1234.3f;
o << "IF1906" << "��ָ1906" << 1234.0f;
}

//��ʱ�����ú���
void select_data(otl_connect& db)
{
otl_stream o(1024, //buffer size
"select * from tb2",
db);//�����е��������ݵ�������������
char InstrumentID[32], InstrumentName[64];
double LastPrice;
while (!o.eof())//ֻҪ��δ��������������ĩβ
{
o >> InstrumentID >> InstrumentName >> LastPrice;
char buf[256] = { 0 };
sprintf(buf,"��ԼID��%s,��Լ����%s,�۸�%.02f",InstrumentID,InstrumentName,LastPrice);
MessageBoxA(NULL, buf, "��ѯ", MB_OK);
}
}
*/

class IDataStore::DatabaseImpl
{
public:
	DatabaseImpl();
	virtual bool StoreData(int type, void* param);
	virtual bool StoreMarketData(MarketData* pMarketData);	//�洢�������
	virtual bool StoreBaseData(BaseData* pBaseData);				//�洢��Լ��������
	virtual bool StoreUserInfo(UserInfo* pUserInfo);					//�洢��¼�û���Ϣ
	virtual bool StoreExchange(Exchange* exchange);				//�洢��������Ϣ
	//...�ȵȣ�δ�����
	virtual bool request(int request_id, void* param);
	virtual bool LoadMarketData(vector<MarketData>& out_data, int start_time, int end_time);//ȡĳ��ʱ������ݣ������vector��
	virtual bool LoadBaseData(BaseData& out_data, std::string instrumentID);							//ȡĳ��Լ�ĺ�Լ��Ϣ

	virtual bool login(const string& user_name, const string& password, const ServerInfo& bs);
	virtual bool logout();
	virtual bool isLogined(){ return m_bIsLogined; }
protected:
	otl_connect sql_server;
	bool m_bIsLogined;
	ServerInfo m_ServerInfo;
};

IDataStore::DatabaseImpl::DatabaseImpl()
{
	sql_server.otl_initialize();
	m_bIsLogined = false;
}

bool IDataStore::DatabaseImpl::login(const string& user_name, const string& password, const ServerInfo& bs)
{
	try
	{
		char szConnStr[256] = { 0 };
		sprintf(szConnStr,
			"driver={%s};server=fe80::6c10:1a2c:c3bf:9907%12;uid=%s;pwd=%s;DSN=%s",
			bs.protocol.c_str(), user_name.c_str(), password.c_str(), bs.name.c_str());
		sql_server.rlogon(szConnStr);
		//string connect_string = "driver={SQL Server};server=fe80::6c10:1a2c:c3bf:9907%12;uid=mt;pwd=w123123;DSN=MultiTrader";//����ODBC�������ַ���,server=fe80::6c10:1a2c:c3bf:9907%12;uid=mt;pwd=w123123
		//sql_server.rlogon(connect_string.c_str());	//DESKTOP-R1LD508\SQLEXPRESSΪ��������sql server���ݿ��������,����������Ӳ���,Ϊ��ʹ��IP��ַ����
		
		//����Ϊ�����õ�
		/*
		//����ȥ��sql_server��һ��otl_connect����������sql server���ݿ������Ҫ�����ö���
		otl_cursor::direct_exec(
			sql_server,
			("use "+bs.id).c_str(),			//Ҳ����"use MultiTraderDB"
			otl_exception::enabled
			);//�����׳��쳣���˴���ѡ��ODBC����Դ

		//ͨ��OTL��ɾ��
		otl_cursor::direct_exec(
		sql_server,
		"drop table test_tab",
		otl_exception::disabled
		);//�������׳��쳣���˴���ѡ��ODBC����Դ���ɾ�����

		//ͨ��OTL�⽨��
		otl_cursor::direct_exec(
			sql_server,
			"IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id=OBJECT_ID(N'[dbo].[tb2]') AND type in (N'U'))"
			"create table tb2(InstrumentID varchar(32), InstrumentName varchar(64), PriceTick float);"
			);//ѡ��ODBC����Դ��Ľ���������ַ����е�����Ϊsql server������

		//��������,�����õ�
		//insert(sql_server);

		//��ѯ����
		//select_data(sql_server);

		//sql_server.logoff();//�ǳ���
		*/
	}
	catch (otl_exception& p)
	{
		//�׳��쳣��ʾʧ�ܣ�
		MessageBoxA(NULL, (char*)p.msg, __FUNCTION__, MB_OK); //__FUNCTION__��ʾ����ĺ�����,���ö�λ����
	}
	m_ServerInfo = bs;//����¼�ɹ�������Ӧ�����ݿ���Ϣ��������
	return (m_bIsLogined=true);
}

bool IDataStore::DatabaseImpl::logout()
{
	if (isLogined())
	{
		sql_server.logoff();
		return true;
	}
	return false;
}

bool IDataStore::DatabaseImpl::request(int request_id, void* param)
{//request�ڲ�����LoadMarketData��LoadBaseData�ȴ����ݿ�����ȡ����
	return true;
}


bool IDataStore::DatabaseImpl::LoadMarketData(vector<MarketData>& out_data, int start_time, int end_time)
{
	return true;
}

bool IDataStore::DatabaseImpl::LoadBaseData(BaseData& out_data, std::string instrumentID)
{
	return true;
}


bool IDataStore::DatabaseImpl::StoreData(int type, void* param)
{
	switch (type)
	{
		case StoreDataType::Store_Exchange:
		{
			Exchange* exchange = reinterpret_cast<Exchange*>(param);
			StoreExchange(exchange);
			break;
		}
		case StoreDataType::Store_BaseData:
		{
			BaseData* basedata = reinterpret_cast<BaseData*>(param);
			StoreBaseData(basedata);
			break;
		}
		case StoreDataType::Store_MarketData:
		{
			MarketData* marketdata = reinterpret_cast<MarketData*>(param);
			StoreMarketData(marketdata);
			break;
		}
		case StoreDataType::Store_UserInfo:
		{
			UserInfo* userinfo = reinterpret_cast<UserInfo*>(param);
			StoreUserInfo(userinfo);
			break;
		}
		default:
			break;
	}
	return true;
}

bool IDataStore::DatabaseImpl::StoreExchange(Exchange* pExchange)
{
	if (!m_bIsLogined)
	{
		return false;
	}
	if (NULL == pExchange)
	{
		return false;
	}
	try
	{
		//**************************���Ӷ�Ӧ��ODBC����Դ��********************************//
		otl_cursor::direct_exec(
			sql_server,
			("use " + m_ServerInfo.id).c_str(),		//Ҳ����"use MultiTraderDB"
			otl_exception::enabled
			);//�����׳��쳣���˴���ѡ��ODBC����Դ
		//**************************����洢********************************//
		string create_table_sql = "IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[tb_exchanges]') AND type in (N'U')) "
			"create table tb_exchanges(ExchangeID varchar(9),ExchangeName varchar(31),EchangeProperty char(1)";
		otl_cursor::direct_exec
			(
			sql_server,
			create_table_sql.c_str()
			, otl_exception::enabled); // create table
		//**************************�洢����********************************//
		string insert_data_sql = "insert into tb_exchanges values(:f1<char[9]>,:f2<char[31]>,:f3<char[1]>)";
		otl_stream o(1, //buffer size should be == 1 always on INSERT.
			insert_data_sql.c_str(),
			sql_server // connectobject
			);
		o << pExchange->ExchangeID.c_str()
			<< pExchange->ExchangeName.c_str()
			<< pExchange->ExchangeProperty;
	}
	catch (otl_exception& p)
	{
		MessageBoxA(NULL, (char*)p.msg, __FUNCTION__, MB_OK);
	}
}

bool IDataStore::DatabaseImpl::StoreUserInfo(UserInfo* pUserInfo)
{
	if (!m_bIsLogined)
	{
		return false;
	}
	if (NULL == pUserInfo)
	{
		return false;
	}
	try
	{
		//**************************���Ӷ�Ӧ��ODBC����Դ��********************************//
		otl_cursor::direct_exec(
			sql_server,
			("use " + m_ServerInfo.id).c_str(),//Ҳ����"use MultiTraderDB"
			otl_exception::enabled
			);//�����׳��쳣���˴���ѡ��ODBC����Դ

		//**************************����洢********************************//
		string create_table_sql = "IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id=OBJECT_ID(N'[dbo].[tb_user_login]') AND type in (N'U'))"
			"create table tb_user_login("
			"UserID varchar(16), BrokerID varchar(11), TradingDay varchar(9), LoginTime varchar(9),"
			"SystemName char(41), ExchangeTime varchar(9))";
		otl_cursor::direct_exec(
			sql_server,
			create_table_sql.c_str(),
			otl_exception::disabled
			);//ѡ��ODBC����Դ��Ľ������

		//**************************�洢����********************************//
		string insert_data_sql = "insert into tb_user_login values(:f1<char[16]>, :f2<char[11]>, :f3<char[9]>, :f4<char[9]>, :f5<char[41]>, :f6<char[9]>)";
		otl_stream o(1, //buffer size should be ==1 always on Insert,
			insert_data_sql.c_str(),
			sql_server);//sql_serverΪotl_connect����
		o << pUserInfo->UserID
			<< pUserInfo->BrokerID
			<< pUserInfo->TradingDay
			<< pUserInfo->LoginTime
			<< pUserInfo->SystemName
			<< pUserInfo->ExchangeTime;
	}
	catch (otl_exception& e)
	{
		//�׳��쳣��ʾʧ�ܣ�
		MessageBoxA(NULL, (char*)e.msg, __FUNCTION__, MB_OK);
	}
}

bool IDataStore::DatabaseImpl::StoreBaseData(BaseData* pBaseData)
{
	//����sql���
	if (!m_bIsLogined)
	{
		return false;
	}
	if (NULL == pBaseData)
	{
		return false;
	}
	try
	{
		//����ѡ�������ݿ�
		otl_cursor::direct_exec
			(
			sql_server,
			("use " + m_ServerInfo.id).c_str(),
			otl_exception::enabled // disable OTL exceptions
			); // droptable

		//����, ���������������sql�����е���������
		string table_name("tb_instrumets");
		string create_table_sql =
			"IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[" + table_name + "]') AND type in (N'U')) "
			"create table " + table_name + "(InstrumentID varchar(31),"
			"ExchangeID varchar(9),��InstrumentName varchar(21),��ProductID varchar(33),"
			"ProductClass char(1),��MaxMarketOrderVolume int,��MinMarketOrderVolume int,"
			"MaxLimitOrderVolume int,��MinLimitOrderVolume int,��VolumeMultiple int,"
			"PriceTick float,��CreateDate varchar(9),��OpenDate varchar(9),��ExpireDate varchar(9),"
			"StartDelivDate varchar(9),��EndDelivDate varchar(9),��IsTrading int, ��PositionType char(1),"
			"LongMarginRatio float,��ShortMarginRatio float,"
			"ExchangeInstID  varchar(31),  DeliveryYear int,  DeliveryMonth int, InstLifePhase char(1),"
			"PositionDateType char(1),  MaxMarginSideAlgorithm char(1), UnderlyingInstrID varchar(31),"
			"StrikePrice float,  OptionsType char(1), UnderlyingMultiple float,   CombinationType char(1))";
		otl_cursor::direct_exec
			(
			sql_server,
			create_table_sql.c_str()
			, otl_exception::enabled); // create table

		//�洢����,���������������C++�е���������,���ǲ�֧��char����,���Ƚ��鷳����
		string insert_data_sql = "insert into " + table_name +
			" values(:f1<char[31]>,:f2<char[9]>,:f3<char[21]>,:f4<char[33]>,:f5<int>,:f6<int>,:f7<int>,"
			":f8<int>,:f9<int>,:f10<int>,:f11<double>,:f12<char[9]>,:f13<char[9]>,:f14<char[9]>,:f15<char[9]>,:f16<char[9]>,"
			":f17<int>,:f18<int>,:f19<double>,:f20<double>,:f21<char[31]>,:f22<int>,:f23<int>,:f24<int>,:f25<int>,"
			":f26<int>,:f27<char[31]>,:f28<double>,:f29<int>,:f30<double>,:f31<int>)";
		otl_stream o(1, //buffer size should be == 1 always on INSERT.
			insert_data_sql.c_str(),
			sql_server // connectobject
			);
		o << pBaseData->code
			<< pBaseData->ExchangeID
			<< pBaseData->InstrumentName
			<< pBaseData->ProductID
			<< (int)pBaseData->ProductClass
			<< pBaseData->MaxMarketOrderVolume
			<< pBaseData->MinMarketOrderVolume
			<< pBaseData->MaxLimitOrderVolume
			<< pBaseData->MinLimitOrderVolume
			<< pBaseData->VolumeMultiple
			<< pBaseData->PriceTick
			<< pBaseData->CreateDate
			<< pBaseData->OpenDate
			<< pBaseData->ExpireDate
			<< pBaseData->StartDelivDate
			<< pBaseData->EndDelivDate
			<< pBaseData->IsTrading
			<< (int)pBaseData->PositionType
			<< pBaseData->LongMarginRatio
			<< pBaseData->ShortMarginRatio
			<< pBaseData->ExchangeInstID
			<< pBaseData->DeliveryYear
			<< pBaseData->DeliveryMonth
			<< (int)pBaseData->InstLifePhase
			<< (int)pBaseData->PositionDateType
			<< (int)pBaseData->MaxMarginSideAlgorithm
			<< pBaseData->UnderlyingInstrID
			<< pBaseData->StrikePrice
			<< (int)pBaseData->OptionsType
			<< pBaseData->UnderlyingMultiple
			<< (int)pBaseData->CombinationType;
	}
	catch (otl_exception& p)
	{
		MessageBoxA(NULL, (char*)p.msg, __FUNCTION__, MB_OK);
	}
	return true;
}

bool IDataStore::DatabaseImpl::StoreMarketData(MarketData* pMarketData)
{
	//����sql���
	if (!m_bIsLogined)
	{//������ݿ��Ƿ��Ѿ���¼��
		return false;
	}
	if (NULL == pMarketData)
	{//��鴫���������Ƿ�Ϊ��
		return false;
	}
	try
	{
		//ѡ��ʹ�õ����ݿ�
		otl_cursor::direct_exec
			(
			sql_server,
			("use " + m_ServerInfo.id).c_str(),
			otl_exception::enabled // disable OTL exceptions
			);

		//����;
		string table_name(pMarketData->code); //���ݲ�ͬ�ĺ�ԼID�����ཨ��
		string create_table_sql =
			"IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[" + table_name + "]') AND type in (N'U')) "
			"create table " + table_name + "(InstrumentID varchar(31),"
			"ExchangeID varchar(9),��TradingDay varchar(9),��LastPrice float,"
			"OpenPrice float, HighestPrice float, LowestPrice float, ClosePrice float,"
			"Volume int, Turnover float, PreSettlementPrice float,  SettlementPrice float,  PreClosePrice float,"
			"PreOpenInterest float,  OpenInterest float,  UpdateTime varchar(9), UpdateMillisec int,  BidPrice1 float,"
			"BidVolume1 int,  AskPrice1 float, AskVolume1 int, AveragePrice float,"
			"UpperLimitPrice float, LowerLimitPrice float)";
		otl_cursor::direct_exec
			(
			sql_server,
			create_table_sql.c_str()
			, otl_exception::enabled); // create table

		//�洢����;
		string insert_data_sql = "insert into " + table_name +
			" values(:f1<char[31]>,:f2<char[9]>,:f3<char[9]>,:f4<double>,:f5<double>,:f6<double>,:f7<double>,"
			":f8<double>,:f9<int>,:f10<double>,:f11<double>,:f12<double>,:f13<double>,:f14<double>,:f15<double>,:f16<char[9]>,"
			":f17<int>,:f18<double>,:f19<int>,:f20<double>,:f21<int>,:f22<double>, :f23<double>, :f24<double>)";
		otl_stream o(1, //buffer size should be == 1 always on INSERT.
			insert_data_sql.c_str(),
			sql_server // connectobject
			);
		o << pMarketData->code
			<< pMarketData->ExchangeID
			<< pMarketData->TradingDay
			<< pMarketData->LastPrice
			<< pMarketData->OpenPrice
			<< pMarketData->HighestPrice
			<< pMarketData->LowestPrice
			<< pMarketData->ClosePrice
			<< pMarketData->Volume
			<< pMarketData->Turnover
			<< pMarketData->PreSettlementPrice
			<< pMarketData->SettlementPrice
			<< pMarketData->PreClosePrice
			<< pMarketData->PreOpenInterest
			<< pMarketData->OpenInterest
			<< pMarketData->UpdateTime
			<< pMarketData->UpdateMillisec
			<< pMarketData->BidPrice
			<< pMarketData->BidVolume
			<< pMarketData->AskPrice
			<< pMarketData->AskVolume
			<< pMarketData->AveragePrice
			<< pMarketData->UpperLimitPrice
			<< pMarketData->LowerLimitPrice;

	}
	catch (otl_exception& p)
	{
		MessageBoxA(NULL, (char*)p.msg, __FUNCTION__, MB_OK);
	}
	return true;
}


//���ⲿʹ�õĽӿ�
IDataStore::IDataStore()
{
	pImpl = make_shared<DatabaseImpl>();//������DatabaseImpl�����,�������ַ����pImpl
}

IDataStore::~IDataStore()
{

}

bool IDataStore::login(const string& user_name, const string& password, const ServerInfo& bs)
{
	return pImpl->login(user_name, password, bs);
}

bool IDataStore::logout()
{
	return pImpl->logout();
}

bool IDataStore::request(int request_id, void* param)
{
	return pImpl->request(request_id, param);
}

bool IDataStore::StoreMarketData(MarketData* pMarketData)
{
	return pImpl->StoreMarketData(pMarketData);
}

bool IDataStore::StoreBaseData(BaseData* pBaseData)
{
	return pImpl->StoreBaseData(pBaseData);
}


bool IDataStore::LoadMarketData(vector<MarketData>& out_data, int start_time, int end_time)
{
	return pImpl->LoadMarketData(out_data, start_time, end_time);
}

bool IDataStore::LoadBaseData(BaseData& out_data, std::string instrumentID)
{
	return pImpl->LoadBaseData(out_data, instrumentID);
}

bool IDataStore::isLogined()
{
	return pImpl->isLogined();
}

bool IDataStore::StoreUserInfo(UserInfo* pUserInfo)
{
	return pImpl->StoreUserInfo(pUserInfo);
}

bool IDataStore::StoreData(int type, void* param)
{//�ⲿ������õĽӿں���
	return pImpl->StoreData(type, param);
}



