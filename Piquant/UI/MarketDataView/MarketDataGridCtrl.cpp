// MarketDataGridCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "../App.h"
#include "MarketDataGridCtrl.h"
#include "../RealTimeDataDisp/RealtimePriceDlg.h"
#include <string>

// CMarketDataGridCtrl

IMPLEMENT_DYNAMIC(CMarketDataGridCtrl, CGridCtrl)

CMarketDataGridCtrl::CMarketDataGridCtrl()
{

}

CMarketDataGridCtrl::~CMarketDataGridCtrl()
{
}


void CMarketDataGridCtrl::OnFixedColumnClick(CCellID& cell)
{
	USES_CONVERSION;
	int row = cell.row;
	CString InstrumentID_eastMoneyneed = this->GetItemText(row, 0);//��ȡ��ԼID��ȷ����0��һ���Ǻ�ԼID���˴�֮�����������Ƹ���ƷID
	CString InstrumentID_sinaneed;
	CString ExchangeID = this->GetItemText(row, 19);//��ȡ������ID
	std::string instrument;
	//�˴�Ҫ���ж����Ƹ��ĺ�ԼIDת��
	if (ExchangeID==_T("SHFE"))
	{//��AG19061
		std::string instrument_str = CW2A(InstrumentID_eastMoneyneed.GetString());
		for (int i = 0; i < instrument_str.length(); ++i)
		{
			if (isalpha(instrument_str[i]))
			{
				instrument += toupper(instrument_str[i]);
			}
			else
			{
				instrument += instrument_str[i];
			}
		}
		InstrumentID_sinaneed = instrument.c_str(); //���˲ƾ��ڻ�API�����ƷID����ĸ�Ѵ�дת����
		instrument += "1";  //�����Ƹ���������Ʒ��ź���Ҫ��1
	}
	else if (ExchangeID == _T("INE"))
	{//��SC2001_INE
		std::string instrument_str = CW2A(InstrumentID_eastMoneyneed.GetString());
		for (int i = 0; i < instrument_str.length(); ++i)
		{
			if (isalpha(instrument_str[i]))
			{
				instrument += toupper(instrument_str[i]);
			}
			else
			{
				instrument += instrument_str[i];
			}
		}
		InstrumentID_sinaneed = instrument.c_str(); //���˲ƾ��ڻ�API�����ƷID����ĸ�Ѵ�дת����
		instrument += "_INE";  //�����Ƹ���������Ʒ��ź���Ҫ��_INE
	}
	else if (ExchangeID == _T("DCE"))
	{//��J20013
		std::string instrument_str = CW2A(InstrumentID_eastMoneyneed.GetString());
		for (int i = 0; i < instrument_str.length(); ++i)
		{
			if (isalpha(instrument_str[i]))
			{
				instrument += toupper(instrument_str[i]);
			}
			else
			{
				instrument += instrument_str[i];
			}
		}
		InstrumentID_sinaneed = instrument.c_str(); //���˲ƾ��ڻ�API�����ƷID����ĸ�Ѵ�дת����
		instrument += "3";  //�����Ƹ���������Ʒ��ź���Ҫ��3
	}
	else if (ExchangeID == _T("CZCE"))
	{//��JR0034
		std::string instrument_str = CW2A(InstrumentID_eastMoneyneed.GetString());
		for (int i = 0; i < instrument_str.length(); ++i)
		{
			if (isalpha(instrument_str[i]))
			{
				instrument += toupper(instrument_str[i]);
			}
			else
			{
				instrument += instrument_str[i];
			}
		}
		InstrumentID_sinaneed = instrument.c_str(); //���˲ƾ��ڻ�API�����ƷID����ĸ�Ѵ�дת����
		instrument += "4";  //�����Ƹ���������Ʒ��ź���Ҫ��4
	}
	else
	{//�н��������Ѵ���ģ�����ܶ�
		std::string instrument_str = CW2A(InstrumentID_eastMoneyneed.GetString());
		std::string product;
		std::string month;
		int cnt = 0;
		for (int i = 0; i < instrument_str.length(); ++i)
		{//��ò�ƷID�������·�
			if (isalpha(instrument_str[i]))
			{
				product += instrument_str[i];
			}
			else
			{//ֻȡ�·�, ����300�ڻ�һ��ΪIF1906��4λ����
				cnt+=1;
				if (cnt>=3)
				{
					month += instrument_str[i];
				}
			}
		}
		if (product=="IF")
		{
			instrument += "0411";//����300��ǰ׺
			instrument += month;
			instrument += "_ZJ";//��׺
		}
		else if (product == "IC")
		{
			instrument += "0611";//IC��ǰ׺
			instrument += month;
			instrument += "_ZJ";//��׺
		}
		else if (product == "IH")
		{
			instrument += "0711";//IC��ǰ׺
			instrument += month;
			instrument += "_ZJ";//��׺
		}
		else if (product == "T")
		{//10���ڹ�ծ�ڻ�
			instrument += "1111";//IC��ǰ׺
			instrument += month;
			instrument += "_ZJ";//��׺
		}
		else if (product == "TF")
		{//5���ڹ�ծ�ڻ�
			instrument += "0511";//IC��ǰ׺
			instrument += month;
			instrument += "_ZJ";//��׺
		}
		else
		{//CTP���Է���2���ڹ�ծ�ڻ����������Ƹ��޴�����
			//��������
		}
		InstrumentID_sinaneed = InstrumentID_eastMoneyneed; //���˲ƾ��ڻ�API�����ƷID����ĸ�����дת������ΪCTP�ײ㴫�ص��н���ID����ĸ���Ǵ�д��
	}
	InstrumentID_eastMoneyneed = instrument.c_str(); //�γɶ����Ƹ���Ʒ���
	CRealtimePriceDlg realtimeprice_dlg(InstrumentID_eastMoneyneed, InstrumentID_sinaneed);
	realtimeprice_dlg.DoModal();
}

BEGIN_MESSAGE_MAP(CMarketDataGridCtrl, CGridCtrl)
END_MESSAGE_MAP()



// CMarketDataGridCtrl message handlers


