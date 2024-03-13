#pragma once

#include "Util/Macro.h"
#include <utility>
#include "Protobuf/XTankMsg.pb.h"
#include "SocketConfig.h"
#include <string>


class SocketClient {
	SINGLETON(SocketClient);


private:
	//socket������
	unsigned long long csocket{};

	//���ܵ���Ϣ�ֽ�����
	char* msgRcvData{ new char[BUFFER_SIZE] {0} };

	//���͵���Ϣ�ֽ�����
	char* msgSendData{ new char[BUFFER_SIZE] {0} };




	//���շ�������Ϣ ����{��Ϣ��id,��Ϣ�����ݲ��ֵĳ���}
	std::pair<XTankMsg::MSG,int> ReceiveFromServer();

	//������Ϣ
	std::shared_ptr<google::protobuf::Message> ParseMsg(int msgId, int dataSize);

	//������Ϣ
	bool SendToServer();

public:

	~SocketClient() {
		delete[] msgRcvData;
		delete[] msgSendData;
	}

	//���ӵ�������
	bool ConnectToServer();

	//�߳� ������Ϣ ������Ϣ ѹ����Ϣ����
	void SocketClientThread(const bool& isEnd);

	//��ȡ����ip
	std::string GetLocalIP();

	//��ȡ�������id
	XTankMsg::PlayerId GetPlayerId();

};