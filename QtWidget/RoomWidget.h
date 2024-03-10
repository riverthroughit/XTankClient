#pragma once

#include "ReceiveWidget.h"
#include "ui_RoomWidget.h"

class RoomWidget : public ReceiveWidget
{
	Q_OBJECT

public:
	RoomWidget(QWidget *parent = nullptr);
	~RoomWidget();

private:
	Ui::RoomWidgetClass ui;

	//�Ƿ���׼��
	bool isReady{};

	void ReceiveMsg() override;

	void ReadyToGame();

private slots:

	void BackBtnClicked();

	void ReadyBtnClicked();

};
