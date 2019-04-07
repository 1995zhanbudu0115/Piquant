#pragma once
#include <string>
namespace data_http
{
    using namespace std;
    class CHttpDataEntry
    {
    public:
        CHttpDataEntry();
        ~CHttpDataEntry();

        // ��������
        CHttpDataEntry(const CHttpDataEntry& rhs);

        // ���ظ�ֵ������
        CHttpDataEntry& operator=(const CHttpDataEntry& rhs);

    public:
        string name; // �ڻ�����
		string datasourceID; //�����ṩ�̱��
        string open; // ���̼�
		string maxPrice; //��߼�
		string minPrice; //��ͼ�
		string close; // �����̼�

		string buyPrice01; // ��1����
		string sellPrice01; // ��1����
		string price; //���¼�

		string settlementprice;//�����
		string presettlementprice;//�����
		string buyCount01; // ��1����
		string sellCount01; // ��1����
		string openinstrest;//�ֲ���
		string volumevalue;//�ɽ���
    };
}



