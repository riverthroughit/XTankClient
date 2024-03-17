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

	//��ŷ�����������Ϣ
	std::deque<MessageData> msgDeque;

	//������
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

	//��ŷ�����������Ϣ
	std::deque<MessageData> msgDeque;

	//������
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

	//��¼����
	void SendLoginReq();

	//�ǳ�����
	void SendLogoutReq();

	//���뷿������
	void SendJoinRoomReq(int roomId);

	//������������
	void SendCreateRoomReq();

	//���׼����Ϸ����
	void SendPlayerReadyReq();

	//��Ҽ�����Ϸ����
	void SendPlayerCutInReq();

	//���׼����Ϸ����
	void SendPlayerReadyCancelReq();

	//�˳���������
	void SendExitRoomReq();

	//��Ϸ׼��֪ͨ
	void SendGameReadyAck();

	//�����û�����֪ͨ
	void SendPlayerInputNtf(int frameId, int cmdId);

	//�����û�׷֡���֪ͨ
	void SendPlayerChaseUpNtf();

	//�˳���Ϸ����
	void SendExitGameReq();

};