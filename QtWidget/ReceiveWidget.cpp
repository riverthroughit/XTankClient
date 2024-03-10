#include "ReceiveWidget.h"
#include "qtimer.h"
#include "SocketConfig.h"

ReceiveWidget::ReceiveWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	receiveMsgTimerId = startTimer(OUT_GAME_MSG_TICK, Qt::PreciseTimer);
}

ReceiveWidget::~ReceiveWidget()
{}

void ReceiveWidget::timerEvent(QTimerEvent * event)
{
	if (event->timerId() == receiveMsgTimerId) {
		ReceiveMsg();
	}
}
