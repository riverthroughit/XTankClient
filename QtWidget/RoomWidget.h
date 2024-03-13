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

	//是否已准备
	bool isReady{};

	void ReceiveMsg() override;

	void ReadyToGame();

	//id 是否准备
	using PlayerVec = std::vector<std::pair<std::string, bool>>;
	//刷新玩家列表 
	void RefreshPlayerTable(const PlayerVec& players);

private slots:

	void BackBtnClicked();

	void ReadyBtnClicked();

};
