#include "PaintWidget.h"
#include "qpainter.h"
#include "qevent.h"
#include "Input/KeyboardData.h"
#include "Renderer/PRenderBuffer.h"
#include "ECS/World.h"
#include <qDebug>
#include "Util/Util.h"
#include "ECS/Component/FrameComponent.h"
#include "qtimer.h"

PaintWidget::PaintWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	updateTimerId = startTimer(RENDER_TICK, Qt::PreciseTimer);

}

PaintWidget::~PaintWidget()
{}


void PaintWidget::paintEvent(QPaintEvent* event)
{

	//更新屏幕分辨率
	SCREEN_HEIGHT = height();
	SCREEN_WIDTH = width();
	//创建QPainter对象
	QPainter painter(this);
	InitQPaint(painter);

	//获取需渲染的物体并渲染
	auto& pRenderBuffer = PRenderBuffer::Instance();
	auto buffer = pRenderBuffer.GetReadBuffer();

	for (auto& [shape, pos,direc] : buffer) {

		DrawEntityCollision(shape, pos, direc, painter);
	}


}

void PaintWidget::keyPressEvent(QKeyEvent* event)
{
	if (event->isAutoRepeat()) {
		return;
	}

	int key = event->key();
	KeyboardData::Instance().SetKeyDown(key, true);
}

void PaintWidget::keyReleaseEvent(QKeyEvent* event)
{
	if (event->isAutoRepeat()) {
		return;
	}

	int key = event->key();
	KeyboardData::Instance().SetKeyDown(key, false);
}

void PaintWidget::timerEvent(QTimerEvent* event)
{
	if (event->timerId() == updateTimerId) {
		update();
		//repaint();
	}
}

void PaintWidget::InitQPaint(QPainter& painter)
{
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::TextAntialiasing);
	//设置画笔
	QPen pen;
	pen.setWidth(3); //线宽
	pen.setColor(Qt::black); //划线颜色
	pen.setStyle(Qt::SolidLine);//线的样式，实线、虚线等
	pen.setCapStyle(Qt::FlatCap);//线端点样式
	pen.setJoinStyle(Qt::BevelJoin);//线的连接点样式
	painter.setPen(pen);
	//设置画刷
	QBrush brush;
	brush.setColor(Qt::yellow); //画刷颜色
	brush.setStyle(Qt::SolidPattern); //画刷填充样式
	painter.setBrush(brush);
}

void PaintWidget::DrawEntityCollision(PRENDER_SHAPE::Type shape, Vec2f pos, Vec2f direc, QPainter& painter)
{

	//圆半径
	const static int blockR = logicToScreen(BLOCK_RADIUS) + 0.5;
	const static int tankR = logicToScreen(TANK_RADIUS);
	const static int bulletR = logicToScreen(BULLET_RADIUS);

	//坦克炮台半径
	const static int tankBatteryR = logicToScreen(1);

	//变换坐标
	pos = logicToScreen(pos);

	//四舍五入
	int x = std::round(pos.x), y = std::round(pos.y);

	switch (shape)
	{
	case PRENDER_SHAPE::BLOCK:
		painter.drawArc(x - blockR, y - blockR, blockR * 2, blockR * 2, 0, 16 * 360);
		break;

	case PRENDER_SHAPE::TANK:
		painter.drawArc(x - tankR, y - tankR, tankR * 2, tankR * 2, 0, 16 * 360);

		//炮台
		x = pos.x + direc.x * tankR;
		y = pos.y + direc.y * tankR;
		painter.drawArc(x - tankBatteryR, y - tankBatteryR, tankBatteryR * 2, tankBatteryR * 2, 0, 16 * 360);

		break;

	case PRENDER_SHAPE::BULLET:
		painter.drawArc(x - bulletR, y - bulletR, bulletR * 2, bulletR * 2, 0, 16 * 360);
		break;

	default:
		break;
	}
}
