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

	//���ѡ�е���
	int selectedRow{};

	void ReceiveMsg() override;

	using RoomVector = std::vector<std::tuple<int, int, bool>>;
	//ˢ�·����б� tuple{roomid,num,isInGame}
	void RefreshRoomTable(const RoomVector& rooms);

private slots:

	void JoinRoomBtnClicked();

	void CreateRoomBtnClicked();

	void BackBtnClicked();
	
	void RoomTableClicked(int row, int col);
};
