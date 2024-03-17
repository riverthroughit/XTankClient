#pragma once

#include "ReceiveWidget.h"
#include "ui_RoomWidget.h"
#include <vector>
#include <utility>
#include <string>
#include "qstring.h"

class RoomWidget : public ReceiveWidget
{
	Q_OBJECT

public:
	RoomWidget(QWidget *parent = nullptr);
	~RoomWidget();

private:
	Ui::RoomWidgetClass ui;

	//玩家是否已准备
	bool isReady{};

	//房间是否已开赛
	bool isInGame{};

	//准备按钮或加入按钮上的文字
	QString JoinBtnText;

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
