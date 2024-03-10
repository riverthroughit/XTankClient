#pragma once

#include "ReceiveWidget.h"
#include "ui_LobbyWidget.h"
#include <vector>
#include <tuple>

class LobbyWidget : public ReceiveWidget
{
	Q_OBJECT

public:
	LobbyWidget(QWidget *parent = nullptr);
	~LobbyWidget();

private:
	Ui::LobbyWidgetClass ui;

	//玩家选中的行
	int selectedRow{};

	void ReceiveMsg() override;

	using RoomVector = std::vector<std::tuple<int, int, bool>>;
	//刷新房间列表 tuple{roomid,num,isInGame}
	void RefreshRoomTable(const RoomVector& rooms);

private slots:

	void JoinRoomBtnClicked();

	void CreateRoomBtnClicked();

	void BackBtnClicked();
	
	void RoomTableClicked(int row, int col);
};
