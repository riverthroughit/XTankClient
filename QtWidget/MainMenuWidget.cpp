#include "MainMenuWidget.h"
#include <Windows.h>
#include <string>
#include "Socket/MessageQueue.h"
#include "WidgetManager.h"

MainMenuWidget::MainMenuWidget(QWidget *parent)
	: ReceiveWidget(parent)
{
	ui.setupUi(this);
}

MainMenuWidget::~MainMenuWidget()
{}

void MainMenuWidget::ReceiveMsg()
{
	MessageData msg = MsgRecvQueue::Instance().GetAndPopTopMsg();

	if (msg.type == XTankMsg::LOGIN_ACK) {
		const XTankMsg::LoginAck& msgData = static_cast<XTankMsg::LoginAck&>(*msg.msgPtr);
		assert(msgData.success());

		//关闭
		WidgetManager::Instance().CloseWidget(WIDGET_NAME::MAIN_MENU);

		//转到大厅
		WidgetManager::Instance().ShowWidget(WIDGET_NAME::LOBBY);
	}
}

void MainMenuWidget::JoinBtnClicked() {

	MsgSendQueue::Instance().SendLoginReq();

}

void MainMenuWidget::ExitBtnClicked()
{

	
}