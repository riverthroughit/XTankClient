#pragma once

#include "Util/Macro.h"
#include <utility>
#include "Protobuf/XTankMsg.pb.h"
#include "SocketConfig.h"
#include <string>


class SocketClient {
	SINGLETON(SocketClient);


private:
	//socket描述符
	unsigned long long csocket{};

	//接受的消息字节数组
	char* msgRcvData{ new char[BUFFER_SIZE] {0} };

	//发送的消息字节数组
	char* msgSendData{ new char[BUFFER_SIZE] {0} };




	//接收服务器消息 返回{消息体id,消息体内容部分的长度}
	std::pair<XTankMsg::MSG,int> ReceiveFromServer();

	//解析消息
	std::shared_ptr<google::protobuf::Message> ParseMsg(int msgId, int dataSize);

	//发送消息
	bool SendToServer();

public:

	~SocketClient() {
		delete[] msgRcvData;
		delete[] msgSendData;
	}

	//连接到服务器
	bool ConnectToServer();

	//线程 接收消息 解析消息 压入消息队列
	void SocketClientThread(const bool& isEnd);

	//获取本机ip
	std::string GetLocalIP();

	//获取本地玩家id
	XTankMsg::PlayerId GetPlayerId();

};