#include "PaintWidget.h"
#include "qpainter.h"
#include "qevent.h"
#include "ECS/Component/KeyboardComponent.h"
#include "ECS/Component/PRenderBufferComponent.h"
#include "ECS/World.h"
#include <qDebug>
#include "Util/Util.h"
#include "ECS/Component/FrameComponent.h"

PaintWidget::PaintWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

PaintWidget::~PaintWidget()
{}


void PaintWidget::paintEvent(QPaintEvent* event)
{
	//创建QPainter对象
	QPainter painter(this);
	InitQPaint(painter);

	//获取渲染队列
	PRenderBufferComponent& bufferComp = mXTankWorld->GetSingletonComponent<PRenderBufferComponent>();
	auto curBuffer = bufferComp.GetCurBuffer();
	auto preBuffer = bufferComp.GetPreBuffer();

	//变换坐标
	for (auto& [_, data] : curBuffer) {
		data.pos = logicToScreen(data.pos);
		data.direc = logicToScreen(data.direc);
	}
	for (auto& [_, data] : preBuffer) {
		data.pos = logicToScreen(data.pos);
		data.direc = logicToScreen(data.direc);
	}

	//圆半径
	float r = logicToScreen(10.0);

	//帧组件
	FrameComponent& framComp = mXTankWorld->GetSingletonComponent<FrameComponent>();

	//插值得到各模型位置
	for (auto& [id, preData] : preBuffer) {

		Vec2f prePos = preData.pos;
		Vec2f preDirec = preData.direc;

		auto curPairIte = curBuffer.find(id);

		if (curPairIte == curBuffer.end()) {
			painter.drawArc(prePos.x - r, prePos.y + r, r * 2, r * 2, 0, 16 * 360);
		}
		else {
			Vec2f curPos = curPairIte->second.pos;
			Vec2f curDirec = curPairIte->second.direc;

			Vec2f pos = linearInterp(prePos, curPos, framComp.percent);
			Vec2f direc = linearInterp(preDirec, curDirec, framComp.percent);

			painter.drawArc(pos.x - r, pos.y + r, r * 2, r * 2, 0, 16 * 360);

		}
	}


}

void PaintWidget::keyPressEvent(QKeyEvent* event)
{
	if (event->isAutoRepeat()) {
		return;
	}

	int key = event->key();
	KeyboardComponent& keyboardComp = mXTankWorld->GetSingletonComponent<KeyboardComponent>();
	keyboardComp.keysDown[key] = true;
}

void PaintWidget::keyReleaseEvent(QKeyEvent* event)
{
	if (event->isAutoRepeat()) {
		return;
	}

	int key = event->key();
	KeyboardComponent& keyboardComp = mXTankWorld->GetSingletonComponent<KeyboardComponent>();
	keyboardComp.keysDown[key] = false;
}

void PaintWidget::InitQPaint(QPainter& painter)
{
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::TextAntialiasing);
	//设置画笔
	QPen pen;
	pen.setWidth(3); //线宽
	pen.setColor(Qt::red); //划线颜色
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
