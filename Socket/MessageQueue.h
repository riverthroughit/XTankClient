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


class MessageQueue {
	SINGLETON(MessageQueue);

	using Message = google::protobuf::Message;

	//存放服务器来的消息
	std::deque<MessageData> msgDeque;

	//队列锁
	std::shared_mutex dequeMutex;

public:

	void PushMsg(MessageData msgData){
		std::unique_lock<std::shared_mutex> lock;
		msgDeque.push_back(msgData);
	}

	void PopMsg(){
		std::unique_lock<std::shared_mutex> lock;

		if (!msgDeque.empty()) {
			msgDeque.pop_front();
		}
	}

	MessageData GetTopMsg() {
		std::shared_lock<std::shared_mutex> slock;
		MessageData msg;

		if (!msgDeque.empty()) {
			msg = msgDeque.front();
		}

		return msg;
	}

	MessageData GetAndPopTopMsg(){
		std::unique_lock<std::shared_mutex> lock;
		MessageData msg;

		if (!msgDeque.empty()) {
			msg = msgDeque.front();
			msgDeque.pop_front();
		}

		return msg;
	}

	void ClearMsg() {
		std::unique_lock<std::shared_mutex> lock;
		msgDeque.clear();
	}
};