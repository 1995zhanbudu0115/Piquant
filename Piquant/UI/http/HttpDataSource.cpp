#include "stdafx.h"

#include "HttpDataSource.h"
#include "HttpHelper.h"
#include "HttpDataEntry.h"
#include <Afx.h>
#include <regex>
#include <iostream>
#include <fstream>

namespace data_http
{

    // ��ѯ ʵʱ���� URL
    const std::string CHttpDataSource::m_strQueryURL = "http://hq.sinajs.cn/list="; //+TA1905 //�������˲ƾ�������

    // ��ѯʵʱ���۵�URL
	const std::string CHttpDataSource::m_strQueryURLGif = "http://webquotepic.eastmoney.com/GetPic.aspx?id=";  //���Զ����Ƹ���ǰ׺

    CHttpDataSource::CHttpDataSource(void)
        : m_http(nullptr)
    {
        Init();
    }


    CHttpDataSource::~CHttpDataSource(void)
    {
        Release();
    }

    // ��ʼ�����ݿ�
    bool CHttpDataSource::Init(void)
    {
        try {
            // http���ʹ��߳�ʼ��
            m_http = new CHttpHelper;
            return true;
        }
        catch(const std::bad_alloc& e) {
            TRACE("%s", e.what());
        }
        catch(const std::exception& e) {
            TRACE("%s", e.what());
        }
        catch(...) {
            TRACE("����δ֪�쳣");
        }
        return false;
    }

    // ������Դ
    void CHttpDataSource::Release(void)
    {
        if (m_http!=nullptr)
        {
			delete m_http;
			m_http = nullptr;
        }
    }

    // ��ѯ�ڻ�ʵʱ����
    bool CHttpDataSource::QueryStockRealTimeData(const std::string& Code, CHttpDataEntry& data)
    {
        using namespace std;
        //1.��ȡ��ҳ��Ϣ
        // ƴ�ӳ������� URL: http://hq.sinajs.cn/list=TA1905
        std::string strURL = m_strQueryURL;
		strURL.append(Code);
        // ��ȡ����
        std::string strContent;
        if(!m_http->Get(strURL, strContent)) 
		{
            return false;
        }
        //2.��ֻ�ȡ,����
        /* �������: var hq_str_TA1905="PTA1905,150004,6410.00,6428.00,6288.00,6430.00,6294.00,6296.00,6296.00,6366.00,6460.00,204,19,1050908,2120722,֣,PTA,2019-03-18,1,6638.000,6288.000,6848.000,6288.000,6848.000,6288.000,6848.000,5560.000,142.233";
           ʹ��������ʽ���в��
           v[0] = "var hq_str_TA1905"
           v[1] = "PTA1905"
           v[2] = "150004"
		   v[3] = "6410.00"
		   v[4] = "6428.00"
		   v[5] = "6288.00"
		   v[6] = "6430.00"
		   v[7] = "6294.00"
		   v[8] = "6294.00"
		   v[9] = "6296.00"
		   v[10] = "6296.00"
		   v[11] = "6366.00"
		   v[12] = "6460.00"
		   v[13] = "204"
		   v[14] = "19"
		   v[15] = "1050908"
		   v[16] = "2120722"
		   v[17] = "֣"
           ...
           */
        regex reg("=|,|;|\""); // ��'=' or ',' or '"' or ';' ��Ϊ�ָ�����в��
        sregex_token_iterator it(strContent.begin(), strContent.end(), reg, -1);
        sregex_token_iterator end;
        std::vector<std::string> v;
        while(it != end) 
		{//������ݽ�vector
            //��ȡ��Ϊ�յ���,�洢
            if(*it != "") 
			{
                v.push_back(*it);
            }
            ++it;
        }
		if (v.size()<15) //��Ϊֻ����15�����ݹ���������жϽ�������15
		{//����飬���˲ƾ��ڻ�API���п��ܷ��ؿ����ݵģ�Ϊ�˱�������飬����ᱨvector��ش���
			return false;
		}
        //3.�����صĽṹ�帳ֵ
        data.name = v[1]; // �ڻ�����
		data.datasourceID = v[2]; //�����ṩ�̱��
		data.open = v[3]; // ���̼�
		data.maxPrice = v[4]; //��߼�
		data.minPrice = v[5]; //��ͼ�
		data.close = v[6]; // �������̼�
		data.buyPrice01 = v[7]; // ��1����
		data.sellPrice01 = v[8]; // ��1����
		data.price = v[9]; //���¼�
		data.settlementprice = v[10];//�����
		data.presettlementprice = v[11];//������
		data.buyCount01 = v[12]; // ��1����
		data.sellCount01 = v[13]; // ��1����
		data.openinstrest = v[14]; //�ֲ���
		data.volumevalue = v[15];//�ɽ���
        // TODO: ������ݸ�ʽ�Ƿ�Ϸ�
        return true;
    }
	

	
	 // ���ع�Ʊ����K��ͼ
	 bool CHttpDataSource::QueryStockKLineMin(const std::string& stockCode, const std::string& filename)
	 {
		using namespace std;
		std::string strURL = m_strQueryURLGif; //����ǰ׺
		strURL.append(stockCode);					 //��Ӳ�ƷID���밴�ն����Ƹ���IDҪ��
		strURL.append("&imageType=rf&token=44c9d251add88e27b65ed86506f6e5da"); //�����׺�������Ƹ��˴�������Դ������֮��
		// ��ȡ����
		std::string content;
		 if (!m_http->Get(strURL, content, nullptr)) 
		 {
			 return false;
		 }
		// 2.�����ļ����ݵ�filenameָ��λ��
		try 
		{
			// ����ļ�����,��ɾ���ļ��ؽ�
			ofstream file(filename, ios::out | ios::binary);
			if(file) 
			{//�Զ���������дgif
				file.write(content.c_str(), content.size());
				file.close();
			}
		}
		catch(const std::exception& e) 
		{
			TRACE("%s", e.what());
		}
		catch(...) 
		{
			TRACE("����δ֪�쳣");
		}
		 return true;
	 }
}
