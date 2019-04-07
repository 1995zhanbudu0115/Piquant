#include "stdafx.h"
#include "HttpDataEntry.h"


namespace data_http
{
    CHttpDataEntry::CHttpDataEntry()
    {
    }


    CHttpDataEntry::~CHttpDataEntry()
    {
    }

    // ��������
    CHttpDataEntry::CHttpDataEntry(const CHttpDataEntry& rhs)
    {
        if(this != &rhs) {
            this->name = rhs.name; 
			this->datasourceID = rhs.datasourceID;
            this->open = rhs.open; // ���տ��̼�
            this->close = rhs.close; // �������̼�
            this->price = rhs.price; // ��ǰ�۸�
            this->maxPrice = rhs.maxPrice; // ������߼�
            this->minPrice = rhs.minPrice; // ������ͼ�
			this->buyCount01 = rhs.buyCount01; // ��1����
			this->buyPrice01 = rhs.buyPrice01; // ��1����
			this->sellCount01 = rhs.sellCount01; // ��1����
			this->sellPrice01 = rhs.sellPrice01; // ��1����
			this->presettlementprice = rhs.presettlementprice; 
			this->settlementprice = rhs.settlementprice; 
			this->openinstrest = rhs.openinstrest; 
			this->volumevalue = rhs.volumevalue; 
        }
    }
    // ��ֵ
    CHttpDataEntry& CHttpDataEntry::operator=(const CHttpDataEntry& rhs)
    {
        if(this != &rhs) {
			this->name = rhs.name;
			this->datasourceID = rhs.datasourceID;
			this->open = rhs.open; // ���տ��̼�
			this->close = rhs.close; // �������̼�
			this->price = rhs.price; // ��ǰ�۸�
			this->maxPrice = rhs.maxPrice; // ������߼�
			this->minPrice = rhs.minPrice; // ������ͼ�
			this->buyCount01 = rhs.buyCount01; // ��1����
			this->buyPrice01 = rhs.buyPrice01; // ��1����
			this->sellCount01 = rhs.sellCount01; // ��1����
			this->sellPrice01 = rhs.sellPrice01; // ��1����
			this->presettlementprice = rhs.presettlementprice;
			this->settlementprice = rhs.settlementprice;
			this->openinstrest = rhs.openinstrest;
			this->volumevalue = rhs.volumevalue;
        }
        return *this;
    }
}

