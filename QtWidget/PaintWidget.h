#pragma once

#include <QWidget>
#include "ui_PaintWidget.h"
#include "TypeConfig.h"
#include "Math/Vec2.h"

class World;

class PaintWidget : public QWidget
{
	Q_OBJECT

private:
	Ui::PaintWidgetClass ui;
	World* mXTankWorld{};
	int updateTimerId{};
public:
	PaintWidget(QWidget *parent = nullptr);
	~PaintWidget();

	void SetWorld(World* world);

protected:
	virtual void paintEvent(QPaintEvent* event) override;
	virtual void keyPressEvent(QKeyEvent* event) override;
	virtual void keyReleaseEvent(QKeyEvent* event) override;
	void timerEvent(QTimerEvent* event);
private:
	void InitQPaint(QPainter& painter);

	//»æÖÆÅö×²Ä£ÐÍ
	void DrawEntityCollision(PRENDER_SHAPE::Type shape, Vec2f pos, QPainter& painter);
};
