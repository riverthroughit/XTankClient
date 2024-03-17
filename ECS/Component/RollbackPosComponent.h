#pragma once
#include "Math/Vec2.h"

//记录回滚前的位置 用在回滚后做插值

struct RollbackPosComponent {

	Vec2Fixed pos;

};