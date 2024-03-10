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
	//������Ļ�ֱ���
	SCREEN_HEIGHT = height();
	SCREEN_WIDTH = width();
	//����QPainter����
	QPainter painter(this);
	InitQPaint(painter);

	//��ȡ��Ⱦ����
	PRenderBufferComponent& bufferComp = mXTankWorld->GetSingletonComponent<PRenderBufferComponent>();
	auto [renderFrameId, buffers] = bufferComp.GetFrameIdAndReadBuffers();
	auto& preBuffer = buffers.first, & curBuffer = buffers.second;

	//֡���
	FrameComponent& framComp = mXTankWorld->GetSingletonComponent<FrameComponent>();
	unsigned int frameId = framComp.frameId;
	float percent = framComp.percent;

	//���߼�֡�Ѹ�������һ֡ ����Ⱦ֡ʹ�õĲ�ֵ����ֱ����Ϊ1
	if (frameId > renderFrameId) {
		percent = 1;
	}


	//�任����
	for (auto& [_, data] : curBuffer) {
		data.pos = logicToScreen(data.pos);
		data.direc = logicToScreen(data.direc);
	}
	for (auto& [_, data] : preBuffer) {
		data.pos = logicToScreen(data.pos);
		data.direc = logicToScreen(data.direc);
	}


	//��ֵ�õ���ģ��λ��
	for (auto& [id, preData] : preBuffer) {

		Vec2f prePos = preData.pos;
		Vec2f preDirec = preData.direc;

		auto curPairIte = curBuffer.find(id);

		//�����Ƿ�����֡�����о�����
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
	//���û���
	QPen pen;
	pen.setWidth(3); //�߿�
	pen.setColor(Qt::red); //������ɫ
	pen.setStyle(Qt::SolidLine);//�ߵ���ʽ��ʵ�ߡ����ߵ�
	pen.setCapStyle(Qt::FlatCap);//�߶˵���ʽ
	pen.setJoinStyle(Qt::BevelJoin);//�ߵ����ӵ���ʽ
	painter.setPen(pen);
	//���û�ˢ
	QBrush brush;
	brush.setColor(Qt::yellow); //��ˢ��ɫ
	brush.setStyle(Qt::SolidPattern); //��ˢ�����ʽ
	painter.setBrush(brush);
}

void PaintWidget::DrawEntityCollision(PRENDER_SHAPE::Type shape, Vec2f pos, QPainter& painter)
{

	//Բ�뾶
	int blockR = logicToScreen(CUBE_SIDE_LENTH / 2);
	int tankR = logicToScreen(TANK_RADIUS);
	int bulletR = logicToScreen(BULLET_RADIUS);

	//�������� ��Ȼ���ܶ���
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
