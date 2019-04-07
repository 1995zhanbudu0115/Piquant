
#include "stdafx.h"
#include "SinaDataQuote.h"

SinaDataQuote::SinaDataQuote()
{
	memset(m_prequote, 0, 300);
	memset(m_buffer, 0, 300);
}

SinaDataQuote::~SinaDataQuote()
{
}

/*
	�����ڻ�����ӿ�
	�������� http://stock2.finance.sina.com.cn/futures/api/json.php/IndexService.getInnerFuturesDailyKLine?symbol=[Code]
	�������� http://stock2.finance.sina.com.cn/futures/api/json.php/IndexService.getInnerFuturesMiniKLine[X]m?symbol=[Code]
*/
void SinaDataQuote::getquote_DailyKLine(string instrumentID)
{
	CInternetSession session; //"�����Ự"
	CHttpFile* sessionfile = NULL;	
	const string sinaurl("http://stock2.finance.sina.com.cn/futures/api/json.php/IndexService.getInnerFuturesDailyKLine?symbol=" + instrumentID);
	try
	{
		CString url(sinaurl.c_str());
		sessionfile = (CHttpFile*)session.OpenURL(url);
	}
	catch (CInternetException& ex)
	{
		::MessageBox(NULL, _T("error��open request faile"), _T("�������ݻ�ȡ����"), MB_OK);
		ex.Delete();
	}

	m_outputfile = instrumentID+".csv";
	ofstream output(m_outputfile.c_str(), ios_base::app); 
	if (!output.is_open())
	{
		cout << "can't open the outputfile,please check the filepath is ok.";
		return;
	}
	size_t getlen;
	if (sessionfile == NULL)
	{
		cout << "Error,can't read the stdiofile" << endl;
	}
	else
	{
		char* header = "ʱ��,���̼�,��߼�,��ͼ�,���̼�,�ɽ���";
		output << header;
		getlen = sessionfile->Read(m_buffer, 999999);		//�������ݶ�ʧ���⣬����ϵ���ɱ�֤����ȫ����ȡ
		sessionfile->Flush();
		if (strncmp(m_prequote, m_buffer, 1000) != 0)
		{
			for (size_t i = 0; i < getlen; ++i)
			{
				if (m_buffer[i] == '[' || m_buffer[i] == ']')
				{
					if (m_buffer[i] == '[')
					{
						output << endl;
					}
					continue;//ֱ������
				}
				output << m_buffer[i];
				++m_recordcount;
			}
			strcpy_s(m_prequote, m_buffer);
		}
	}
}

void SinaDataQuote::getquote_MiniKLine(int mimute, string instrumentID)
{
	CInternetSession session; //"�����Ự"
	CHttpFile* sessionfile = NULL;
	char preurl[1000];
	sprintf_s(preurl, "http://stock2.finance.sina.com.cn/futures/api/json.php/IndexService.getInnerFuturesMiniKLine%dm?symbol=", mimute);
	const string sinaurl(preurl+instrumentID);
	try
	{
		CString url(sinaurl.c_str());
		sessionfile = (CHttpFile*)session.OpenURL(url);
	}
	catch (CInternetException& ex)
	{
		::MessageBox(NULL, _T("error��open request faile"), _T("�������ݻ�ȡ����"), MB_OK);
		ex.Delete();
	}

	m_outputfile = instrumentID+"_" + to_string(mimute) + "min.csv";
	ofstream output(m_outputfile.c_str(), ios_base::app);
	if (!output.is_open())
	{
		cout << "can't open the outputfile,please check the filepath is ok.";
		return;
	}
	size_t getlen;
	if (sessionfile == NULL)
	{
		cout << "Error,can't read the stdiofile" << endl;
	}
	else
	{
		char* header = "ʱ��,���̼�,��߼�,��ͼ�,���̼�,�ɽ���";
		output << header;
		getlen = sessionfile->Read(m_buffer, 999999);		//�������ݶ�ʧ���⣬����ϵ���ɱ�֤����ȫ����ȡ
		sessionfile->Flush();
		if (strncmp(m_prequote, m_buffer, 1000) != 0)
		{
			for (size_t i = 0; i < getlen; ++i)
			{
				if (m_buffer[i] == '[' || m_buffer[i] == ']')
				{
					if (m_buffer[i] == '[')
					{
						output << endl;
					}
					continue;//ֱ������
				}
				output << m_buffer[i];
				++m_recordcount;
			}
			strcpy_s(m_prequote, m_buffer);
		}
	}
}

SinaDataQuote& get_SinaDataQuote()
{
	static SinaDataQuote g_SinaDataQuote;
	return g_SinaDataQuote;
}

/*
	CHttpFile.read�����������ݶ�ʧ���⣬��δ��������ϵ��������Կɱ�֤ȫ�����ݻ�ȡ
	�ⲿͨ�����´����ȡcsv��ʽ�������ļ���
	get_SinaDataQuote().getquote_MiniKLine(5,"CU1905"); //CU1905_5min.csv
	get_SinaDataQuote().getquote_DailyKLine("CU1905"); //CU1905.csv
*/