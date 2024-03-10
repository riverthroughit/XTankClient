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
	MessageData msg = MessageQueue::Instance().GetAndPopTopMsg();

	if (msg.type == XTankMsg::GAME_READY_ACK) {
		ui.readyBtn->setEnabled(true);
		ui.readyBtn->setText("取消准备");
	}
	else if (msg.type == XTankMsg::GAME_READY_CANCEL_ACK) {
		ui.readyBtn->setEnabled(true);
		ui.readyBtn->setText("准备");
	}
	else if (msg.type == XTankMsg::ROOM_EXIT_ACK) {
		WidgetManager::Instance().CloseWidget(WIDGET_NAME::ROOM);
		WidgetManager::Instance().ShowWidget(WIDGET_NAME::LOBBY);
	}
	else if (msg.type == XTankMsg::GAME_READY_NTF) {

		ReadyToGame();

	}
}

void RoomWidget::ReadyToGame()
{
	ThreadManager::Instance().InvokeGameThread();

	XTankWorld* xTankWorld = ThreadManager::Instance().GetXTankWorld();
	PaintWidget* paintWidget = static_cast<PaintWidget*>(WidgetManager::Instance().GetWidget(WIDGET_NAME::PAINT));

	paintWidget->SetWorld(xTankWorld);

	WidgetManager::Instance().CloseWidget(WIDGET_NAME::ROOM);
	WidgetManager::Instance().ShowWidget(WIDGET_NAME::PAINT);
}

void RoomWidget::BackBtnClicked()
{
	SocketClient::Instance().SendExitRoomReq();
}


void RoomWidget::ReadyBtnClicked()
{
	if (!isReady) {
		ui.readyBtn->setEnabled(false);
		SocketClient::Instance().SendGameReadyReq();
	}
	else {
		ui.readyBtn->setEnabled(false);
		SocketClient::Instance().SendGameReadyCancelReq();
	}
}
