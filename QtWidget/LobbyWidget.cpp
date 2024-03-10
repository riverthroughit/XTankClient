#include "LobbyWidget.h"
#include "Socket/MessageQueue.h"
#include "WidgetManager.h"
#include "Socket/SocketClient.h"
#include "qstring.h"


LobbyWidget::LobbyWidget(QWidget *parent)
	: ReceiveWidget(parent)
{
	ui.setupUi(this);
}

LobbyWidget::~LobbyWidget()
{}

void LobbyWidget::ReceiveMsg()
{
	MessageData msg = MessageQueue::Instance().GetAndPopTopMsg();

	if (msg.type == XTankMsg::LOBBY_NTF) {

		const XTankMsg::LobbyNtf& msgData = static_cast<XTankMsg::LobbyNtf&>(*msg.msgPtr);
		RoomVector rooms;

		for (auto& room : msgData.rooms()) {
			rooms.push_back({ room.id(),room.playernum(),room.ingame() });
		}

		RefreshRoomTable(rooms);

	}
	else if (msg.type == XTankMsg::ROOM_JOIN_ACK ) {

		const XTankMsg::RoomJoinAck& msgData = static_cast<XTankMsg::RoomJoinAck&>(*msg.msgPtr);
		assert(msgData.success());
		WidgetManager::Instance().CloseWidget(WIDGET_NAME::LOBBY);
		WidgetManager::Instance().ShowWidget(WIDGET_NAME::ROOM);
	}
	else if (msg.type == XTankMsg::ROOM_CREATE_ACK) {

		const XTankMsg::RoomCreateAck& msgData = static_cast<XTankMsg::RoomCreateAck&>(*msg.msgPtr);
		assert(msgData.success());
		WidgetManager::Instance().CloseWidget(WIDGET_NAME::LOBBY);
		WidgetManager::Instance().ShowWidget(WIDGET_NAME::ROOM);
	}
	else if (msg.type == XTankMsg::LOGOUT_ACK) {
		WidgetManager::Instance().CloseWidget(WIDGET_NAME::LOBBY);
		WidgetManager::Instance().ShowWidget(WIDGET_NAME::MAIN_MENU);
	}
}

void LobbyWidget::RefreshRoomTable(const RoomVector& rooms)
{
	ui.roomTable->clearContents();

	for (auto& [id, num, isInGame] : rooms) {

		int rowIndex = ui.roomTable->rowCount();
		ui.roomTable->insertRow(rowIndex);

		QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(id));
		QTableWidgetItem* numItem = new QTableWidgetItem(QString::number(num));
		QTableWidgetItem* inGameItem = new QTableWidgetItem();

		if (isInGame) {
			inGameItem->setText("游戏中");
		}
		else {
			inGameItem->setText("准备中");
		}

		ui.roomTable->setItem(rowIndex, 0, idItem);
		ui.roomTable->setItem(rowIndex, 1, numItem);
		ui.roomTable->setItem(rowIndex, 2, inGameItem);
	}

	if (selectedRow >= ui.roomTable->rowCount()) {
		selectedRow = std::max(ui.roomTable->rowCount() - 1, 0);
	}
	ui.roomTable->selectRow(selectedRow);
}

void LobbyWidget::JoinRoomBtnClicked()
{
	if (ui.roomTable->rowCount() == 0) {

	}
	else {
		QTableWidgetItem* idItem = ui.roomTable->item(selectedRow, 0);
		SocketClient::Instance().SendJoinRoomReq(idItem->text().toInt());
	}
	
}

void LobbyWidget::CreateRoomBtnClicked()
{
	SocketClient::Instance().SendCreateRoomReq();
}

void LobbyWidget::BackBtnClicked()
{
	SocketClient::Instance().SendLogoutReq();

}

void LobbyWidget::RoomTableClicked(int row, int col)
{
	selectedRow = row;
}
