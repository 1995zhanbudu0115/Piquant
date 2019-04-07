#include "stdafx.h"
#include "rwJson.h"


rwJson::rwJson()
{
}


rwJson::~rwJson()
{
}

rwJson& get_rwJson()
{//��Ԫ����
	static rwJson g_rwJson;
	return g_rwJson;
}

/*
	����Ϊ���˻�����д��json�ļ���ʾ������
	���������MainFrm.cpp�м������´���ʵ�ֶ�дjson��
	TradingAccount* pTradingAccount = reinterpret_cast<TradingAccount*>(lParam);
	get_rwJson().storeAccountData(pTradingAccount->Available, pTradingAccount->Balance, pTradingAccount->CloseProfit, pTradingAccount->PositionProfit, pTradingAccount->Commission);
	�ο����ף�https://www.jianshu.com/p/62fa72a1ae18
	AccountData.json������������ʾ��
	{
	"�����ʽ�" : 846378.650,
	"Ȩ��" : {
			"�ֲ�Ȩ��" : -3640.0,
			"��̬Ȩ��" : 903514.750,
			"ƽ��Ȩ��" : 0.0
				},
	"������" : 0.0,
	"��Ϣ" : [ "�����ʽ�", "Ȩ��", "������" ]
	}
*/
bool rwJson::storeAccountData(double avaiable, double profitloss, double closeprofit, double positionprofit, double commission)
{
	using std::ofstream;

	Json::Value root;	//���ڵ�

	root["�����ʽ�"] = Json::Value(avaiable);	//���ڵ�����
	Json::Value partner;	//�ӽڵ�
	//�ӽڵ�����
	partner["��̬Ȩ��"] = Json::Value(profitloss);
	partner["ƽ��Ȩ��"] = Json::Value(closeprofit);
	partner["�ֲ�Ȩ��"] = Json::Value(positionprofit);
	root["Ȩ��"] = Json::Value(partner);	//�ӽڵ�ҵ�Ȩ����ڵ���

	root["������"] = Json::Value(commission);		//���ڵ�����

	//������ʽ
	root["��Ϣ"].append("�����ʽ�");
	root["��Ϣ"].append("Ȩ��");
	root["��Ϣ"].append("������");

	//������ļ�
	Json::StyledWriter sw;
	ofstream os;
	os.open("AccountData.json");
	os << sw.write(root);
	os.close();

	return 0;
}

bool rwJson::getAccountData(double& avaiable, double& profitloss, double& closeprofit, double& positionprofit, double& commission)
{
	using namespace std;

	Json::Reader reader;
	Json::Value root;
	//���ļ��ж�ȡ
	ifstream is;
	is.open("AccountData.json", ios::binary);

	if (reader.parse(is, root))
	{
		//��ȡ���ڵ���Ϣ
		avaiable = root["�����ʽ�"].asDouble();
		commission = root["������"].asDouble();

		//��ȡ�ӽڵ���Ϣ
		profitloss = root["Ȩ��"]["�ֲ�Ȩ��"].asDouble();
		closeprofit = root["Ȩ��"]["��̬Ȩ��"].asDouble();
		positionprofit = root["Ȩ��"]["ƽ��Ȩ��"].asDouble();

		//��ȡ������Ϣ
		for (int i = 0; i < root["��Ϣ"].size(); i++)
		{
			string msg = root["��Ϣ"][i].asString();
			cout << msg << '\t';
		}
		cout << endl;
	}
	is.close();
	return 0;
}

bool rwJson::storePosition(std::string strategyName, int position_buy, int position_sell)
{
	using namespace std;
	Json::Value root;	//���ڵ�
	root["���"] = Json::Value(position_buy);	
	root["�ղ�"] = Json::Value(position_sell);		
	//������ļ�
	Json::StyledWriter sw;
	ofstream os;
	std::string json_filename;
	if (os)
	{//���ļ��򿪳ɹ������д����
		json_filename = strategyName + ".json";
		os.open(json_filename);
		os << sw.write(root);
		os.close();
	}
	else
	{//�����½��ļ���д��
		fstream file_position;
		file_position.open(json_filename, fstream::out);
		Json::Value root;	//���ڵ�
		root["���"] = Json::Value(position_buy);
		root["�ղ�"] = Json::Value(position_sell);
		//������ļ�
		Json::StyledWriter sw;
		file_position << sw.write(root);
		file_position.close();
	}
	return 0;
}

bool rwJson::getPosition(std::string strategyName, int& position_buy, int& position_sell)
{
	using namespace std;
	Json::Reader reader;
	Json::Value root;
	//���ļ��ж�ȡ
	ifstream is;
	std::string json_filename = strategyName + ".json";
	is.open(json_filename, ios::binary);
	if (is)
	{//�����ļ��ɹ�������л�ȡ����
		if (reader.parse(is, root))
		{
			//��ȡ���ڵ���Ϣ
			position_buy = root["���"].asInt();
			position_sell = root["�ղ�"].asInt();
		}
		is.close();
	}
	else
	{//���������ڸ��ļ������½�
		fstream file_position;
		file_position.open(json_filename, fstream::out);
		Json::Value root;	//���ڵ�
		root["���"] = Json::Value(0);
		root["�ղ�"] = Json::Value(0);
		//������ļ�
		Json::StyledWriter sw;
		file_position << sw.write(root);
		file_position.close();
	}
	return 0;
}
