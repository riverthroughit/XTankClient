#include "PaintWidget.h"
#include "qpainter.h"
#include "qevent.h"
#include "ECS/Component/KeyboardComponent.h"
#include "ECS/Component/PRenderBufferComponent.h"
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

void PaintWidget::SetWorld(World * world)
{
	mXTankWorld = world;
}


void PaintWidget::paintEvent(QPaintEvent* event)
{
	//更新屏幕分辨率
	SCREEN_HEIGHT = height();
	SCREEN_WIDTH = width();
	//创建QPainter对象
	QPainter painter(this);
	InitQPaint(painter);

	//获取渲染队列
	PRenderBufferComponent& bufferComp = mXTankWorld->GetSingletonComponent<PRenderBufferComponent>();
	auto [renderFrameId, buffers] = bufferComp.GetFrameIdAndReadBuffers();
	auto& preBuffer = buffers.first, & curBuffer = buffers.second;

	//帧组件
	FrameComponent& framComp = mXTankWorld->GetSingletonComponent<FrameComponent>();
	unsigned int frameId = framComp.frameId;
	float percent = framComp.percent;

	//若逻辑帧已更新至下一帧 则渲染帧使用的插值比例直接设为1
	if (frameId > renderFrameId) {
		percent = 1;
	}


	//变换坐标
	for (auto& [_, data] : curBuffer) {
		data.pos = logicToScreen(data.pos);
		data.direc = logicToScreen(data.direc);
	}
	for (auto& [_, data] : preBuffer) {
		data.pos = logicToScreen(data.pos);
		data.direc = logicToScreen(data.direc);
	}


	//插值得到各模型位置
	for (auto& [id, preData] : preBuffer) {

		Vec2f prePos = preData.pos;
		Vec2f preDirec = preData.direc;

		auto curPairIte = curBuffer.find(id);

		//物体是否在两帧缓冲中均出现
		if (curPairIte == curBuffer.end()) {
			DrawEntityCollision(preData.shape, prePos, painter);			
			continue;
		}
		else {
			Vec2f curPos = curPairIte->second.pos;
			Vec2f curDirec = curPairIte->second.direc;

			
			Vec2f pos = linearInterp(prePos, curPos, percent);
			Vec2f direc = linearInterp(preDirec, curDirec, percent);

			DrawEntityCollision(preData.shape, pos, painter);
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
	keyboardComp.keysDown.insert(key);
}

void PaintWidget::keyReleaseEvent(QKeyEvent* event)
{
	if (event->isAutoRepeat()) {
		return;
	}

	int key = event->key();
	KeyboardComponent& keyboardComp = mXTankWorld->GetSingletonComponent<KeyboardComponent>();
	keyboardComp.keysDown.erase(key);
}

void PaintWidget::timerEvent(QTimerEvent* event)
{
	if (event->timerId() == updateTimerId) {
		update();
	}
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

void PaintWidget::DrawEntityCollision(PRENDER_SHAPE::Type shape, Vec2f pos, QPainter& painter)
{

	//圆半径
	int blockR = logicToScreen(CUBE_SIDE_LENTH / 2);
	int tankR = logicToScreen(TANK_RADIUS);
	int bulletR = logicToScreen(BULLET_RADIUS);

	//四舍五入 不然可能抖动
	int x = pos.x + 0.5, y = pos.y + 0.5;

	switch (shape)
	{
	case PRENDER_SHAPE::BLOCK:
		painter.drawArc(x - blockR, y - blockR, blockR * 2, blockR * 2, 0, 16 * 360);
		break;

	case PRENDER_SHAPE::TANK:
		painter.drawArc(x - tankR, y - tankR, tankR * 2, tankR * 2, 0, 16 * 360);
		break;

	case PRENDER_SHAPE::BULLET:
		painter.drawArc(x - bulletR, y - bulletR, bulletR * 2, bulletR * 2, 0, 16 * 360);
		break;

	default:
		break;
	}
}
