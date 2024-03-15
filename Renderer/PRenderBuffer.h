#pragma once
#include "ECS/Types.h"
#include "Math/Vec2.h"
#include "TypeConfig.h"
#include <shared_mutex>
#include <array>
#include <vector>
#include "Util/Macro.h"


struct PRenderData {
	PRENDER_SHAPE::Type shape;
	Vec2f pos;
	Vec2f direc;
};



class PRenderBuffer {

	SINGLETON(PRenderBuffer)

private:

	//��д�� ��������ʱ�軥��
	std::shared_mutex wrMutex;

	//˫����
	std::array<std::vector<PRenderData>, 2> renderBuffer;

	//д��������� ��Ӧ������
	int writeIndex{ 0 };
	int readIndex{ 1 };

public:

	std::vector<PRenderData> GetReadBuffer();

	void WriteToBuffer(const PRenderData& data);

	void SwapPRenderBuffer();

};