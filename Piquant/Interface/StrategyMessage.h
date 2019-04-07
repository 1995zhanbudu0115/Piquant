#ifndef _STRATEGYMESSAGE_H_
#define _STRATEGYMESSAGE_H_

#include <string>
#define WM_UPDATESTRATEGYSTATUS WM_USER+2000
#define WM_UPDATESTRATEGYMSG WM_USER+2001

struct StrategyStatus
{
	std::string strategyName;
	int position_buy;
	int position_sell;
	std::string status;
	std::string description;
};

struct StrategyMsg
{
	std::string strategyName;
	std::string loggingTime;
	int loggingLevel;
	std::string loggingContent;
};

//������־�������, �����ɸߵ���Ϊ��������Ҫ��һ��
#define UEGENT 1
#define IMPORTANT 2
#define ORDINARY 3

#endif