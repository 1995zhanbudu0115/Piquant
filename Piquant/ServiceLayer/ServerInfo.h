#ifndef _SERVER_INFO_H_
#define _SERVER_INFO_H_
#include<string>
#include<vector>
using namespace std;
struct ServerInfo
{
	string id;			//������ID
	string name;		//��������
	string protocol; //Э��: tcp; udp��
	vector<string> trade_server_front;		//����ǰ��
	vector<string> market_server_front;	//����ǰ��
	int trade_server_port;		//����ǰ�ö˿�
	int market_server_port;		//����ǰ�ö˿�
}; 
#endif
