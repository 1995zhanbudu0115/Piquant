#ifndef _IDATASTORE_H_
#define _IDATASTORE_H_

#include "IService.h"
#include "DataTypes.h"
#include "CtpService.h"
#include <memory>
#include "CtpMsgDistributor.h"
using namespace std;

enum StoreDataType
{
	Store_MarketData = 100,
	Store_BaseData=101,
	Store_UserInfo=102,
	Store_Exchange=103
};

class SERVICE_API IDataStore  
{
public:
	IDataStore();
	virtual ~IDataStore();
	virtual bool login(const string& user_name, const string& password, const ServerInfo& bs);
	virtual bool logout();
	virtual bool isLogined();

	virtual bool StoreData(int type, void* param);
	virtual bool StoreMarketData(MarketData* pMarketData);			//�洢�������
	virtual bool StoreBaseData(BaseData* pBaseData);						//�洢��Լ��Ϣ
	virtual bool StoreUserInfo(UserInfo* pUserInfo);							//�洢��¼�����û���Ϣ
	//...�ȵȣ�δ�����
	virtual bool request(int request_id, void* param);
	virtual bool LoadMarketData(vector<MarketData>& out_data, int start_time, int end_time);				//ȡĳ��ʱ������ݣ������vector��
	virtual bool LoadBaseData(BaseData& out_data, std::string instrumentID);																				//ȡĳ��Լ�ĺ�Լ��Ϣ

private:
	class DatabaseImpl;
	std::shared_ptr<DatabaseImpl> pImpl;//pImplΪָ��DatabaseImpl�������ָ��
};

#endif
