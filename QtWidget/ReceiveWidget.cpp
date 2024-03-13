#include "ReceiveWidget.h"
#include "qtimer.h"
#include "SocketConfig.h"


ReceiveWidget::ReceiveWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	
}

ReceiveWidget::~ReceiveWidget()
{}

void ReceiveWidget::timerEvent(QTimerEvent * event)
{
	if (event->timerId() == receiveMsgTimerId) {
		ReceiveMsg();
	}
}

void ReceiveWidget::closeEvent(QCloseEvent* event)
{
	killTimer(receiveMsgTimerId);
}

void ReceiveWidget::showEvent(QShowEvent* event)
{
	receiveMsgTimerId = startTimer(OUT_GAME_MSG_TICK, Qt::PreciseTimer);
}
