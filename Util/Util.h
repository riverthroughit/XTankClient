#pragma once

#include "Math/Vec2.h"
#include <utility>
#include "Config.h"

//�߼�����ת��Ļ����
template<typename T>
T logicToScreen(const T& src) {
	int minLen = std::min(SCREEN_HEIGHT, SCREEN_WIDTH);
	float scale = (float)minLen / SCENE_SIDE_LENTH;
	return src * scale;
}


//��Ⱦ֡��ֵ
template<typename T>
T linearInterp(T src, T dst, float percent) {
	return dst * percent + src * (1 - percent);
}