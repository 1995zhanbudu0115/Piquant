#include "stdafx.h"
#include "ConfigReader.h"
#include "tinyxml\tinystr.h"
#include "tinyxml\tinyxml.h"
#include<vector>


ConfigReader::ConfigReader()
	:document_brokers(NULL), root_brokers(NULL), document_config(NULL), root_config(NULL)
{
		
}
ConfigReader::~ConfigReader()
{
}

ConfigReader& get_config_reader()
{//��Ԫ����
	static ConfigReader g_ConfigReader;
	return g_ConfigReader;
}

bool ConfigReader::load(const string& brokers_file, const string& config_file, bool bForceLoad)
{//bForceLoad�Ƿ�ǿ���������ö�дxml�Ļ���
	if (bForceLoad)
	{//��ǿ���������ö�дxml�Ļ���,���ڴ�֮ǰҪ����������Դ
		unload();
	}
	if (document_brokers != NULL && document_config != NULL)
	{
		return true;
	}
	{//����brokers.xml
		document_brokers = new TiXmlDocument(brokers_file.c_str());			//����ָ��xml�ĵ���ָ��
		if (NULL == document_brokers){ return false; }									//����ʧ�ܷ���false
		document_brokers->LoadFile();														//����xml�ĵ�
		root_brokers = document_brokers->RootElement();							//��ȡ�����
		if (NULL == root_brokers){ return false; }											//��ȡ�����ʧ�ܷ���false
		LoadBrokers();																					//���ؾ�������Ϣ��m_brokers
	}
	{//����config.xml
		document_config = new TiXmlDocument(config_file.c_str());
		if (NULL == document_config){ return false; }
		document_config->LoadFile();
		root_config = document_config->RootElement();
		if (NULL == root_config){ return false; }
		LoadAllListHeaders();																//���ر�ͷ��Ϣ��m_all_list_header
		LoadMyInstrumets();																//���ض��ĺ�Լ��Ϣ��m_instruments
	}
	return true;
}

bool ConfigReader::unload()
{
	if (NULL != document_brokers)
	{
		delete document_brokers;
		document_brokers = NULL;
	}
	if (NULL != document_config)
	{
		delete document_config;
		document_config = NULL;
	}
	if (root_brokers != NULL)
	{
		delete root_brokers;
		root_brokers = NULL;
	}
	if (root_config != NULL)
	{
		delete root_config;
		root_config = NULL;
	}
	m_brokers.clear();
	m_all_list_header.clear();
	m_instruments.clear();
	return true;
}

bool ConfigReader::LoadBrokers()
{//�˴���Ҫ����xml�ļ��ľ���ṹ��д����
	//��ȡ������󣬿�ʼ��ȡ�����ļ��е�������Ϣ
	string broker_tag_name = "broker";
	TiXmlElement* elem = root_brokers->FirstChildElement(broker_tag_name.c_str());//�Ӹ����������ҵ�"broker"���ӽ��
	while (NULL != elem)//���ҵ���Ӧ���ӽڵ㣬������ѭ��
	{
		//��ȡ������ľ�������Ϣ,�羭����ID/��������
		string broker_name = GetAttribute(elem, "BrokerName");//��ȡBrokerName����ֵ
		ServerInfo si;
		si.name = broker_name;
		si.id = elem->Attribute("BrokerID");
		TiXmlElement* servers_elem = elem->FirstChildElement("Servers");									//����ȡbroker���ڵ��Servers�ӽڵ�
		TiXmlElement* front_server_elem = servers_elem->FirstChildElement("FrontServer");		//������ȡServers�ӽڵ��FrontServer�ӽڵ�
		TiXmlElement* name_elem = front_server_elem->FirstChildElement("Name");					//������ȡFrontServer�ӽڵ��Name�ӽڵ�
		TiXmlElement* protocol_elem = front_server_elem->FirstChildElement("Protocol");			//������ȡFrontServer�ӽڵ��Protocol�ӽڵ�
		si.protocol = protocol_elem->GetText();//��ȡ��Ӧ�����ı�����,��Э��
		TiXmlElement* trading_elem = front_server_elem->FirstChildElement("Trading");				//������ȡFrontServer�ӽڵ��Trading�ӽڵ�
		vector<TiXmlElement*> trading_addrs = GetSubElements(trading_elem);							//���ж������ǰ�ã���˴�trading_elem��㿪ʼ��ȡ���к��ӽڵ㣬��Ҫ��ȡ�������ǰ��
		for (int i = 0; i < trading_addrs.size(); ++i)
		{
			si.trade_server_front.push_back(string(trading_addrs[i]->GetText()));
		}
		TiXmlElement* market_elem = front_server_elem->FirstChildElement("MarketData");		//������ȡFrontServer�ӽڵ��MarketData�ӽڵ�
		vector<TiXmlElement*> market_addrs = GetSubElements(market_elem);							//���ж������ǰ��
		for (int j = 0; j < market_addrs.size(); ++j)
		{
			si.market_server_front.push_back(string(market_addrs[j]->GetText()));
		}
		m_brokers.insert(make_pair(broker_name, si));																	//���þ�������Ϣ��broker_nameΪ��, ServerInfoΪֵѹ��map
		elem = elem->NextSiblingElement(broker_tag_name.c_str());											//��Ѱ�ֵܽ�㣬ֱ���Ҳ���Ϊֹ
	}
	return true;
}

bool ConfigReader::LoadAllListHeaders()
{//���ڻ�ȡ��ͷ����
	if (NULL != root_config)
	{
		TiXmlElement* all_list_column_elem = root_config->FirstChildElement("m_ListCtrlColumns");					//��root���ڵ�����ȡ�ض�m_ListCtrlColumns���ӽ��
		vector<TiXmlElement*> vec_list_column_elem = GetSubElements(all_list_column_elem);				//��m_ListCtrlColumns���ӽ������ȡ���к��ӽ��
		for (int i = 0; i < vec_list_column_elem.size(); ++i)
		{
			TiXmlElement* one_list_column = vec_list_column_elem[i];		//ѭ����ȡ���ӽ��
			string list_name = one_list_column->Attribute("_key");				//��ȡ�ض�����ֵ
			vector<TiXmlElement*> one_list_all_columns = GetSubElements(one_list_column);					//��һ����ȡ�ú��ӽ���µ����к��ӽ��
			vector<ColumnInfo> one_list_col_vec;										//���ڴ�ȡ���Լ�ֵ��
			for (int j = 0; j < one_list_all_columns.size(); ++j)
			{
				ColumnInfo ci;
				ci.column_name = one_list_all_columns[j]->Attribute("Name");			//ע�����ִ�Сд
				ci.column_width = atoi(one_list_all_columns[j]->Attribute("Width"));
				string id = GetAttribute(one_list_all_columns[j], ("Id"));
				ci.column_id = atoi(id.c_str());
				ci.column_color = 0;																			//xml�ĵ���δ�и����ԣ����Ĭ����0
				one_list_col_vec.push_back(ci);
			}
			m_all_list_header.insert(make_pair(list_name, one_list_col_vec));
		}
	}
	return false;
}

bool ConfigReader::LoadMyInstrumets()
{
	string my_instruments_tag_name = "MyInstruments";
	TiXmlElement* elem = root_config->FirstChildElement(my_instruments_tag_name.c_str());	//��root����㿪ʼ��ȡ�ض�MyInstruments���ӽ��
	if (NULL == elem)
	{
		return false;
	}
	vector<TiXmlElement*> instruments_elems = GetSubElements(elem);						//��MyInstruments���ӽ������ȡ���к��ӽ��
	for (int i = 0; i < instruments_elems.size(); ++i)
	{
		m_instruments.push_back(GetInnerText(instruments_elems[i]));								//��ȡ�ض������ı�����
	}
	return true;
}


//���ߺ�����
vector<TiXmlElement*> ConfigReader::GetSubElements(TiXmlElement* parent)
{
	vector<TiXmlElement*> sub_elements;
	if (NULL != parent)
	{
		TiXmlElement* elem = parent->FirstChildElement();//�Ӹ����������ҵ���һ�����ӽ�㣬��ȻҲ����ָ��Ҫ���ҵĺ��ӽ��
		while (NULL != elem)//���ҵ��ӽڵ㣬������ѭ��
		{
			sub_elements.push_back(elem);			//�����ӽ��ѹ��vector��
			elem = elem->NextSiblingElement();//�ҵ���Ӧ�ֵܽ�㣬ֱ���Ҳ���Ϊֹ
		}
	}
	return sub_elements;
}

std::string ConfigReader::GetAttribute(TiXmlElement* elem, const string& attr_name)
{//���ض���㣬�����ض�������ȡֵ
	if (NULL != elem)//���������Ƿ���Ч
	{
		const char* attr_val = elem->Attribute(attr_name.c_str());//����Ӧ������ȡֵ
		if (NULL == attr_val)
		{
			return "";
		}
		return attr_val;
	}
	return "";
}

std::string ConfigReader::GetInnerText(TiXmlElement* elem)
{
	if (NULL != elem)
	{
		const char* val = elem->GetText();
		if (NULL == val)
		{
			return "";
		}
		return val;
	}
	return "";
}


vector<ColumnInfo>& ConfigReader::GetHeaderColumn(const string& header_name)
{
	map<string, vector<ColumnInfo>>::iterator iter = m_all_list_header.find(header_name);
	if (iter != m_all_list_header.end())
	{
		return iter->second;
	}
	return vector<ColumnInfo>();//����ͷ��ؿ�;
}