#pragma once

#include <QWidget>
#include "ui_PaintWidget.h"

class World;

class PaintWidget : public QWidget
{
	Q_OBJECT

private:
	Ui::PaintWidgetClass ui;
	World* mXTankWorld{};
public:
	PaintWidget(QWidget *parent = nullptr);
	~PaintWidget();

protected:
	virtual void paintEvent(QPaintEvent* event) override;
	virtual void keyPressEvent(QKeyEvent* event) override;
	virtual void keyReleaseEvent(QKeyEvent* event) override;
private:
	void InitQPaint(QPainter& painter);
};
