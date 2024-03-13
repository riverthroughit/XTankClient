#include "RoomWidget.h"
#include "Socket/MessageQueue.h"
#include "WidgetManager.h"
#include "Socket/SocketClient.h"
#include "qstring.h"
#include "Thread/ThreadManager.h"
#include "PaintWidget.h"
#include "ECS/XTankWorld.h"

RoomWidget::RoomWidget(QWidget *parent)
	: ReceiveWidget(parent)
{
	ui.setupUi(this);
}

RoomWidget::~RoomWidget()
{}

void RoomWidget::ReceiveMsg()
{
	MessageData msg = MsgRecvQueue::Instance().GetAndPopTopMsg();

	if (msg.type == XTankMsg::ROOM_NTF) {
		const XTankMsg::RoomNtf& msgData = static_cast<XTankMsg::RoomNtf&>(*msg.msgPtr);
		PlayerVec players;

		for (auto& room : msgData.playerstates()) {
			std::string ip = room.playerid().ip();
			auto pid = room.playerid().pid();
			ip += std::to_string(pid);
			players.push_back({ ip,room.ready() });
		}

		RefreshPlayerTable(players);
	}
	else if (msg.type == XTankMsg::PLAYER_READY_ACK) {
		isReady = true;
		ui.readyBtn->setEnabled(true);
		ui.readyBtn->setText(u8"取消准备");
	}
	else if (msg.type == XTankMsg::PLAYER_READY_CANCEL_ACK) {
		isReady = false;
		ui.readyBtn->setEnabled(true);
		ui.readyBtn->setText(u8"准备");
	}
	else if (msg.type == XTankMsg::ROOM_EXIT_ACK) {
		WidgetManager::Instance().CloseWidget(WIDGET_NAME::ROOM);
		WidgetManager::Instance().ShowWidget(WIDGET_NAME::LOBBY);
	}
	else if (msg.type == XTankMsg::GAME_READY_REQ) {

		ReadyToGame();

	}
}

void RoomWidget::ReadyToGame()
{
	MsgSendQueue::Instance().SendGameReadyAck();
	WidgetManager::Instance().CloseWidget(WIDGET_NAME::ROOM);
	ThreadManager::Instance().InvokeGameThread();
	WidgetManager::Instance().ShowWidget(WIDGET_NAME::PAINT);
}

void RoomWidget::RefreshPlayerTable(const PlayerVec& players)
{
	//ui.playerTable->clearContents();
	ui.playerTable->setRowCount(0);
	
	for (auto& [id,isReady] : players) {

		int rowIndex = ui.playerTable->rowCount();
		ui.playerTable->insertRow(rowIndex);

		QTableWidgetItem* idItem = new QTableWidgetItem(QString::fromStdString(id));
		QTableWidgetItem* isReadyItem = new QTableWidgetItem();

		if (isReady) {
			isReadyItem->setText(u8"已准备");
		}
		else {
			isReadyItem->setText(u8"空闲中");
		}

		ui.playerTable->setItem(rowIndex, 0, idItem);
		ui.playerTable->setItem(rowIndex, 1, isReadyItem);
	}

}


void RoomWidget::BackBtnClicked()
{
	MsgSendQueue::Instance().SendExitRoomReq();
}


void RoomWidget::ReadyBtnClicked()
{
	if (!isReady) {
		ui.readyBtn->setEnabled(false);
		MsgSendQueue::Instance().SendPlayerReadyReq();
	}
	else {
		ui.readyBtn->setEnabled(false);
		MsgSendQueue::Instance().SendPlayerReadyCancelReq();
	}
}
