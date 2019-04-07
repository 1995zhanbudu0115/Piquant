//Easy-to-use log classes,called minilogger
#ifndef LOGGER_H
#define LOGGER_H
#include "stdafx.h"
#include <string>
#include <fstream>

// ============================================================
//  get system time 
// ============================================================
#include <time.h>
#include <windows.h>

// ============================================================
// time in 24 hours hh:mm:ss format
// ============================================================
static  std::string TimeStamp()
{
	char str[9];

	// get the time, and convert it to struct tm format
	time_t a = time(0);
	struct tm* b = localtime(&a);

	// print the time to the string
	strftime(str, 9, "%H:%M:%S", b);

	return str;
}

// ============================================================
// date YYYY:MM:DD format
// ============================================================
static std::string DateStamp()
{
	char str[11];

	// get the time, and convert it to struct tm format
	time_t a = time(0);
	struct tm* b = localtime(&a);

	// print the time to the string
	strftime(str, 11, "%Y_%m_%d", b);

	return str;
}


// ============================================================
// Here is an example of a simple log decorator, you can define your own decorator
// ============================================================
class TextDecorator
{
public:
	static std::string FileHeader(const std::string& p_title)
	{
		return "==================================================\n" +
			p_title + "\n" +
			"==================================================\n\n";
	}

	static std::string SessionOpen()
	{
		return "\n";
	}

	static std::string SessionClose()
	{
		return "\n";
	}

	static std::string Decorate(const std::string& p_string)
	{
		return p_string + "\n";
	}
};


// ============================================================
// New Logger with a new log file and new log title
// ============================================================
template<class decorator>
class Logger
{
public:
	Logger(const std::string& p_filename,
				const std::string& p_logtitle,
				bool p_timestamp = false,
				bool p_datestamp = false);

	~Logger();
	void Log(const std::string& p_entry);

protected:
	std::fstream m_logfile;
	bool m_timestamp;
	bool m_datestamp;

};


typedef Logger<TextDecorator> TextLog;



template<class decorator>
Logger<decorator>::Logger(const std::string& p_filename,
	const std::string& p_logtitle,
	bool p_timestamp,
	bool p_datestamp)
{
	// now the tricky part... testing to see if a file is open or not.
    // stupid C++. You need to open a file in read mode, and if it doesn't
    // open correctly, you know that it doesn't exist.
	// a file is open or not.
	//�ڼ��������ں�׺����־�ļ��Ƿ����ǰ��Ӧ�����м����ӦĿ¼�Ƿ����
	char* filePath = "log"; //��Ĭ��·���¼���Ƿ����logĿ¼
	FILE* fp = NULL;
	fp = fopen(filePath, "r");//ֻ����ȡ
	if (fp == NULL)
	{
		CreateDirectory(_T("log"), NULL);
	}
	else
	{//�ļ����Ѵ���
		fclose(fp);//�ر��ļ���
		fp = NULL;//��Ҫָ��գ������ָ��ԭ���ļ���ַ
	}
	char filename[100];
	sprintf_s(filename, "%s/%s_%s.txt", filePath, p_filename.c_str(), DateStamp().c_str());
	std::fstream filetester(filename, std::ios::in);
	if (filetester.is_open())
	{
		// the file exists, so just close the test file
		filetester.close();
		// open the real file and set app mode
		m_logfile.open(filename, std::ios::out | std::ios::app);
	}
	else
	{
		// file doesn't exist.
		m_logfile.open(filename, std::ios::out);
		// print out a file header to the file
		m_logfile << decorator::FileHeader(p_logtitle); //FileHeaderΪ�ྲ̬������decorator::FileHeader(p_logtitle)ʹ����ok��
	}

	// print out an opening statement. Make sure it is time-and-date-stamped
	m_timestamp = true;
	m_datestamp = true;
	m_logfile << decorator::SessionOpen();	//���������Ự��ʼ
	Log("Session opened.");
	m_timestamp = p_timestamp;
	m_datestamp = p_datestamp;

}

template< class decorator >
Logger< decorator >::~Logger()
{
	m_timestamp = true;
	m_datestamp = true;
	Log("Session closed.");
	m_logfile << decorator::SessionClose();		//�ر������Ự����

}


template< class decorator >
void Logger< decorator >::Log(const std::string& p_entry)
{
	std::string message;
	if (m_datestamp)
	{
		message += "[" + DateStamp() + "] ";
	}
	if (m_timestamp)
	{
		message += "[" + TimeStamp() + "] ";
	}
	message += p_entry;
	m_logfile << decorator::Decorate(message);
	m_logfile.flush();
}
#endif


/*
C++����ģ����
	ȥ��__declspec(dllexport)������Ҳ��ͨ����Ҳ����˵���õ��������Ϳ�����dll�ⲿʹ��ģ����
	��C++ģ���������ʵ�ֺͶ��嶼��ͷ�ļ��У������������Ӵ���
	��UI���У�����get_global_mgr().GetLog()->Log("this is log1.")������־��¼����־��������UI��ȫ�ֹ���������ά��
*/