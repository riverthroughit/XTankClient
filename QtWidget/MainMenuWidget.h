#pragma once

#include "ReceiveWidget.h"
#include "ui_MainMenuWidget.h"

class MainMenuWidget : public ReceiveWidget
{
	Q_OBJECT

public:
	MainMenuWidget(QWidget *parent = nullptr);
	~MainMenuWidget();

private:
	Ui::MainMenuWidgetClass ui;

	void ReceiveMsg() override;

private slots:
	
	void JoinBtnClicked();

	void ExitBtnClicked();
};
