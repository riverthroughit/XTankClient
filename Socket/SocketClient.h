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

public:

	~SocketClient() {
		delete[] msgRcvData;
		delete[] msgSendData;
	}

	//���ӵ�������
	bool ConnectToServer();

	//�߳� ������Ϣ ������Ϣ ѹ����Ϣ����
	void SocketClientThread();

	//������Ϣ
	bool SendToServer(XTankMsg::MSG msgType, const google::protobuf::Message& msg);

	//��ȡ����ip
	std::string GetLocalIP();

	//��ȡ�������id
	XTankMsg::PlayerId GetPlayerId();

	//��¼����
	void SendLoginReq();

	//�ǳ�����
	void SendLogoutReq();

	//���뷿������
	void SendJoinRoomReq(int roomId);

	//������������
	void SendCreateRoomReq();

	//׼����Ϸ����
	void SendGameReadyReq();

	//׼����Ϸ����
	void SendGameReadyCancelReq();

	//�˳���������
	void SendExitRoomReq();

	//�����û�����֪ͨ
	void SendGameInputNtf(int frameId, int cmdId);

	//�˳���Ϸ����
	void SendExitGameReq();
};