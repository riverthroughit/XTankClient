#include "SocketClient.h"
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include "MessageQueue.h"


using google::protobuf::Message;

std::pair<XTankMsg::MSG, int> SocketClient::ReceiveFromServer()
{

	//读取头部以及id

	int len = 0;
	while (len != MSG_TOTAL_HEAD_SIZE) {
		int curLen = 0;
		curLen = recv(csocket, msgRcvData + len, MSG_TOTAL_HEAD_SIZE - len, 0);
		if (curLen == 0 || curLen == -1) {
			return {XTankMsg::NONE,XTankMsg::NONE };
		}
		len += curLen;
	}

	int dataSize = *reinterpret_cast<int*>(msgRcvData);
	XTankMsg::MSG msgId = *reinterpret_cast<XTankMsg::MSG*>(msgRcvData + MSG_HEAD_SIZE);

	//读取消息内容

	len = 0;
	while (len != dataSize) {
		int curLen = 0;
		curLen = recv(csocket, msgRcvData + MSG_TOTAL_HEAD_SIZE + len, dataSize - len, 0);
		if (curLen == 0 || curLen == -1) {
			return { XTankMsg::NONE,XTankMsg::NONE };
		}
		len += curLen;
	}

	return { msgId,dataSize };
}

std::shared_ptr<Message> SocketClient::ParseMsg(int msgId, int dataSize)
{
	std::shared_ptr<Message> msgPtr;

	switch (msgId)
	{
	case XTankMsg::NONE:
		msgPtr = nullptr;
		break;

	case XTankMsg::LOGIN_ACK:
		msgPtr = std::make_shared<XTankMsg::LoginAck>();
		break;

	case XTankMsg::LOGOUT_ACK:
		msgPtr = std::make_shared<XTankMsg::LogoutAck>();
		break;

	case XTankMsg::LOBBY_NTF:
		msgPtr = std::make_shared<XTankMsg::LobbyNtf>();
		break;

	case XTankMsg::ROOM_CREATE_ACK:
		msgPtr = std::make_shared<XTankMsg::RoomCreateAck>();
		break;

	case XTankMsg::ROOM_JOIN_ACK:
		msgPtr = std::make_shared<XTankMsg::RoomJoinAck>();
		break;

	case XTankMsg::ROOM_EXIT_ACK:
		msgPtr = std::make_shared<XTankMsg::RoomExitAck>();
		break;

	case XTankMsg::ROOM_NTF:
		msgPtr = std::make_shared<XTankMsg::RoomNtf>();
		break;

	case XTankMsg::PLAYER_READY_ACK:
		msgPtr = std::make_shared<XTankMsg::PlayerReadyAck>();
		break;

	case XTankMsg::PLAYER_READY_CANCEL_ACK:
		msgPtr = std::make_shared<XTankMsg::PlayerReadyCancelAck>();
		break;

	case XTankMsg::GAME_READY_REQ:
		msgPtr = std::make_shared<XTankMsg::GameReadyReq>();
		break;

	case XTankMsg::GAME_INIT_NTF:
		msgPtr = std::make_shared<XTankMsg::GameInitNtf>();
		break;

	case XTankMsg::GAME_START_NTF:
		msgPtr = std::make_shared<XTankMsg::GameStartNtf>();
		break;

	case XTankMsg::GAME_FORWARD_NTF:
		msgPtr = std::make_shared<XTankMsg::GameForwardNtf>();
		break;

	case XTankMsg::PLAYER_EXIT_ACK:
		msgPtr = std::make_shared<XTankMsg::PlayerExitAck>();
		break;

	case XTankMsg::PLAYER_CUT_IN_NTF:
		msgPtr = std::make_shared<XTankMsg::PlayerCutInNtf>();
		break;

	default:
		break;
	}

	if (msgPtr == nullptr
		|| !msgPtr->ParseFromArray(msgRcvData + MSG_TOTAL_HEAD_SIZE, dataSize)) {
		return nullptr;
	}
	
	return msgPtr;
}

bool SocketClient::ConnectToServer()
{
	WSAData wdata;//用于存储有关socket的信息
	//初始化Winsock服务 准备socket所需相关资源
	if (WSAStartup(MAKEWORD(2, 2), &wdata) != 0) {//2,2为请求的socket版本号 返回值为0表示初始化socket成功并将相关信息存储在wdata中
		WSACleanup();//释放初始化socket所占用的资源
		assert(0);
		return false;
	}

	csocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//参数分别表示套接字通信的区域 套接字类型 以及对应传输层的协议
	if (csocket == INVALID_SOCKET) {
		closesocket(csocket);//关闭socket
		WSACleanup();
		assert(0);
		return false;
	}

	sockaddr_in skin;//用于绑定socket到指定ip 端口号等
	skin.sin_family = AF_INET;//与套接字中通信域相同
	//skin.sin_addr.S_un.S_addr = inet_addr(ip.c_str());//绑定到的ip 即为服务器ip
	inet_pton(AF_INET, SERVER_IP, &skin.sin_addr.s_addr);//绑定到的ip 即为服务器ip
	skin.sin_port = htons(SOCKET_PORT);//绑定到的端口 即服务器的端口号

	//建立socket与skin中所存地址的连接
	if (connect(csocket, reinterpret_cast<sockaddr*>(&skin), sizeof(skin)) == SOCKET_ERROR) {
		int r = WSAGetLastError();
		closesocket(csocket);//关闭socket
		WSACleanup();
		assert(0 && "连接服务器失败");
		return false;
	}

	u_long nonBlocking = 1;
	//设置为非阻塞
	ioctlsocket(csocket, FIONBIO, &nonBlocking);

	return true;
}

void SocketClient::SocketClientThread(const bool& isEnd)
{
	while (!isEnd) {

		//收
		auto [msgId, dataSize] = ReceiveFromServer();
		if (msgId != XTankMsg::NONE) {
			std::shared_ptr<Message> msg = ParseMsg(msgId, dataSize);
			MsgRecvQueue::Instance().PushMsg({ msgId,msg });
		}

		//发
		SendToServer();
	}
}

bool SocketClient::SendToServer()
{

	auto [msgType,msg] = MsgSendQueue::Instance().GetAndPopTopMsg();

	if (msgType == XTankMsg::NONE) {
		return true;
	}

	memset(msgSendData, 0, BUFFER_SIZE);

	int msgByteSize = msg->ByteSize();

	reinterpret_cast<int&>(*msgSendData) = msgByteSize;

	reinterpret_cast<XTankMsg::MSG&>(*(msgSendData + MSG_HEAD_SIZE)) = msgType;

	if (!msg->SerializeToArray(msgSendData + MSG_TOTAL_HEAD_SIZE, msgByteSize)) {
		assert(0);
		return false;
	}

	if (send(csocket, msgSendData, msgByteSize + MSG_TOTAL_HEAD_SIZE, 0) == -1) {
		assert(0);
		return false;
	}

	return true;
}

std::string SocketClient::GetLocalIP()
{
	char host_name[255];
	std::string local_IP = "";

	if (gethostname(host_name, sizeof(host_name)) != 0) {
		assert(0);
	}
	else {
		struct hostent* host_info = gethostbyname(host_name);
		if (host_info == nullptr) {
			assert(0);
		}
		else {
			struct in_addr* address = (struct in_addr*)host_info->h_addr_list[0];
			local_IP = inet_ntoa(*address);
		}
	}

	return local_IP;
}

XTankMsg::PlayerId SocketClient::GetPlayerId()
{
	static std::string ip = GetLocalIP();
	static unsigned long pid = GetCurrentProcessId();

	XTankMsg::PlayerId playerId;
	playerId.set_ip(ip);
	playerId.set_pid(pid);

	return playerId;
}


