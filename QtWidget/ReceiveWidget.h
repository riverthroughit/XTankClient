#pragma once

#include <QWidget>
#include "ui_ReceiveWidget.h"

class ReceiveWidget : public QWidget
{
	Q_OBJECT

private:
	Ui::ReceiveWidgetClass ui;

	int receiveMsgTimerId{};

public:
	ReceiveWidget(QWidget *parent = nullptr);
	~ReceiveWidget();

protected:

	void timerEvent(QTimerEvent* event) override;

	virtual void ReceiveMsg() {};
};
