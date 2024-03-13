#pragma once

#include "ReceiveWidget.h"
#include "ui_RoomWidget.h"
#include <vector>
#include <utility>
#include <string>

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

	//id �Ƿ�׼��
	using PlayerVec = std::vector<std::pair<std::string, bool>>;
	//ˢ������б� 
	void RefreshPlayerTable(const PlayerVec& players);

private slots:

	void BackBtnClicked();

	void ReadyBtnClicked();

};
