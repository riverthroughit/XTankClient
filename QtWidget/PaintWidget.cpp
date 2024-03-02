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
	//����QPainter����
	QPainter painter(this);
	InitQPaint(painter);

	//��ȡ��Ⱦ����
	PRenderBufferComponent& bufferComp = mXTankWorld->GetSingletonComponent<PRenderBufferComponent>();
	auto curBuffer = bufferComp.GetCurBuffer();
	auto preBuffer = bufferComp.GetPreBuffer();

	//�任����
	for (auto& [_, data] : curBuffer) {
		data.pos = logicToScreen(data.pos);
		data.direc = logicToScreen(data.direc);
	}
	for (auto& [_, data] : preBuffer) {
		data.pos = logicToScreen(data.pos);
		data.direc = logicToScreen(data.direc);
	}

	//Բ�뾶
	float r = logicToScreen(10.0);

	//֡���
	FrameComponent& framComp = mXTankWorld->GetSingletonComponent<FrameComponent>();

	//��ֵ�õ���ģ��λ��
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
