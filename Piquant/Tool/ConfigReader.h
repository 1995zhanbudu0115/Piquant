#ifndef _CONFIG_READER_H
#define _CONFIG_READER_H

#include "stdafx.h"
#include "../ServiceLayer/ServerInfo.h"
#include <map>
#include <vector>
#include <string>
using namespace std;

struct ColumnInfo
{//��ͷ�ṹ��
	int column_id;
	int column_width;
	unsigned int column_color;
	string column_name;
};

class TiXmlDocument;	//ǰ������,��Ӧ��XML�������ĵ�
class TiXmlElement;		//ǰ������,��Ӧ��XML��Ԫ��

class TOOL_API ConfigReader
{
protected:
	ConfigReader();
	virtual ~ConfigReader();
	//���ߺ���,��LoadXXX������ʹ��
	vector<TiXmlElement*> GetSubElements(TiXmlElement* parent);			//��xml�ĵ������ĳ���ڵ㿪ʼ����ȡ���ӽڵ㼯��
	string GetAttribute(TiXmlElement* elem, const string& attr_name);		//��ȡ�ض������ض�����ֵ
	string GetInnerText(TiXmlElement* elem);												//��ȡ�ض������ı�����
	bool LoadBrokers();				//��xml�ĵ��ж�ȡ������������Ϣ,�羭����ID��ǰ�÷�������ַ,����Щ��Ϣ����m_brokers��
	bool LoadAllListHeaders();		//��xml�ĵ��ж�ȡ��ͷ��Ϣ,��ListCtrl�û���Ϣ/ListCtrl���۱��,����Щ����m_all_list_header��
	bool LoadMyInstrumets();		//��xml�ĵ��ж�ȡ���趩�ĺ�Լ, �ù���Ĭ������
	
protected:
	TiXmlDocument* document_brokers;														//ָ��xml�ĵ�(�洢��������Ϣ��xml)��ָ��
	TiXmlElement*     root_brokers;																	//xml�����ָ��(�洢��������Ϣ��xml)
	TiXmlDocument* document_config;															//ָ��xml�ĵ�(�洢������Ϣ��xml)��ָ��
	TiXmlElement*     root_config;																	//xml�����ָ��(�洢������Ϣ��xml)
	map<string, ServerInfo> m_brokers;										//�洢��������Ϣ��map
	map<string, vector<ColumnInfo>> m_all_list_header;				//�洢��ͷ��map
	vector<string> m_instruments;												//�洢���ĺ�Լ��vector

public:
	friend TOOL_API ConfigReader& get_config_reader();//��Ԫ����

public://�ⲿ�ӿ�
	bool load(const string& brokers_file = "brokers.xml", const string& config_file="config.xml",  bool bForceLoad = false);		
	bool unload();
	map<string, ServerInfo>&  GetBrokers(){ return m_brokers; }					//��ȡmap<string, ServerInfo> m_brokers
	vector<ColumnInfo>& GetHeaderColumn(const string& header_name); //��ȡmap<string, vector<ColumnInfo>> m_all_list_header����Ӧheader_name����ͷ��Ϣ����
	vector<string>& GetMyInstruments(){ return m_instruments; }               //��ȡvector<string> m_instruments
};

#endif