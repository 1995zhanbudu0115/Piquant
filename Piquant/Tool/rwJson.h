#ifndef RWJSON_H
#define RWJSON_H

#include "stdafx.h"
#include "jsoncpp/json.h"
#include <iostream>
#include <fstream>

class TOOL_API rwJson
{
public:
	rwJson();
	virtual ~rwJson();
	friend TOOL_API rwJson& get_rwJson();

	//����Ϊ����ʹ�ã��������ҵ�������������
	bool storeAccountData(double avaiable, double profitloss, double closeprofit, double positionprofit, double commission);
	bool getAccountData(double& avaiable, double& profitloss, double& closeprofit, double& positionprofit, double& commission);
	bool storePosition(std::string strategyName, int position_buy, int position_sell);
	bool getPosition(std::string strategyName, int& position_buy, int& position_sell);
};

#endif