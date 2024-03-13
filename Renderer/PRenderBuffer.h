#pragma once
#include "ECS/Types.h"
#include "Math/Vec2.h"
#include "TypeConfig.h"
#include <shared_mutex>
#include <array>
#include <unordered_map>
#include "Util/Macro.h"


struct PRenderData {
	Entity entityId;
	PRENDER_SHAPE::Type shape;
	Vec2f pos;
	Vec2f direc;

};

//key:entity ID
using BufferMap = std::unordered_map<Entity, PRenderData>;

class PRenderBuffer {

	SINGLETON(PRenderBuffer)

private:

	//��д�� ��������ʱ�軥��
	std::shared_mutex wrMutex;

	//������
	std::array<BufferMap, 3> renderBuffer;
	//��ǰ֡����
	unsigned int frameId{};

	//����֡ ����֡ ����д��֡ ��Ӧ������
	int preIndex{ 1 };
	int curIndex{ 2 };
	int writeIndex{ 0 };

public:

	void SetFrameId(int id) {
		frameId = id;
	}

	std::pair<unsigned int, std::pair<BufferMap, BufferMap>> GetFrameIdAndReadBuffers();


	void WriteToBuffer(const PRenderData& data);

	void SwapPRenderBuffer();

};