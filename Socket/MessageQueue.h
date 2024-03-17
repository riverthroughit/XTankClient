#pragma once

#include <deque>
#include "Util/Macro.h"
#include <memory>
#include "Protobuf/XTankMsg.pb.h"
#include <shared_mutex>
#include <mutex>
#include <utility>


struct MessageData {
	XTankMsg::MSG type{};
	std::shared_ptr<google::protobuf::Message> msgPtr{};
};


class MsgRecvQueue {
	SINGLETON(MsgRecvQueue);

private:
	using Message = google::protobuf::Message;

	//存放服务器来的消息
	std::deque<MessageData> msgDeque;

	//队列锁
	std::shared_mutex dequeMutex;

public:

	void PushMsg(const MessageData& msgData){
		std::unique_lock<std::shared_mutex> lock(dequeMutex);
		msgDeque.push_back(msgData);
	}

	void PopMsg() {
		std::unique_lock<std::shared_mutex> lock(dequeMutex);

		if (!msgDeque.empty()) {
			msgDeque.pop_front();
		}
	}

	MessageData GetTopMsg() {
		std::shared_lock<std::shared_mutex> slock(dequeMutex);
		MessageData msg;

		if (!msgDeque.empty()) {
			msg = msgDeque.front();
		}

		return msg;
	}

	MessageData GetAndPopTopMsg() {
		std::unique_lock<std::shared_mutex> lock(dequeMutex);
		MessageData msg;

		if (!msgDeque.empty()) {
			msg = msgDeque.front();
			msgDeque.pop_front();
		}

		return msg;
	}

	void ClearMsg() {
		std::unique_lock<std::shared_mutex> lock(dequeMutex);
		msgDeque.clear();
	}
};

class MsgSendQueue {

	SINGLETON(MsgSendQueue);

private:
	using Message = google::protobuf::Message;

	//存放服务器来的消息
	std::deque<MessageData> msgDeque;

	//队列锁
	std::shared_mutex dequeMutex;

public:

	void PushMsg(const MessageData& msgData) {
		std::unique_lock<std::shared_mutex> lock(dequeMutex);
		msgDeque.push_back(msgData);
	}

	void PopMsg() {
		std::unique_lock<std::shared_mutex> lock(dequeMutex);

		if (!msgDeque.empty()) {
			msgDeque.pop_front();
		}
	}

	MessageData GetTopMsg() {
		std::shared_lock<std::shared_mutex> slock(dequeMutex);
		MessageData msg;

		if (!msgDeque.empty()) {
			msg = msgDeque.front();
		}

		return msg;
	}

	MessageData GetAndPopTopMsg() {
		std::unique_lock<std::shared_mutex> lock(dequeMutex);
		MessageData msg;

		if (!msgDeque.empty()) {
			msg = msgDeque.front();
			msgDeque.pop_front();
		}

		return msg;
	}

	void ClearMsg() {
		std::unique_lock<std::shared_mutex> lock(dequeMutex);
		msgDeque.clear();
	}

	//登录请求
	void SendLoginReq();

	//登出请求
	void SendLogoutReq();

	//加入房间请求
	void SendJoinRoomReq(int roomId);

	//创建房间请求
	void SendCreateRoomReq();

	//玩家准备游戏请求
	void SendPlayerReadyReq();

	//玩家加入游戏请求
	void SendPlayerCutInReq();

	//玩家准备游戏请求
	void SendPlayerReadyCancelReq();

	//退出房间请求
	void SendExitRoomReq();

	//游戏准备通知
	void SendGameReadyAck();

	//本地用户输入通知
	void SendPlayerInputNtf(int frameId, int cmdId);

	//本地用户追帧完成通知
	void SendPlayerChaseUpNtf();

	//退出游戏请求
	void SendExitGameReq();

};