#include "ECS/ECSUtil.h"

#include "ECS/Component/PosComponent.h"
#include "ECS/Component/CollisionComponent.h"

bool isCollision(PosComponent& p1, CollisionComponent& c1, PosComponent& p2, CollisionComponent& c2)
{
	bool res = false;

	if (c1.shape == LOGIC_SHAPE::CIRCLE && c2.shape == LOGIC_SHAPE::CIRCLE) {
		//¡Ω‘≤œ‡Ωª
		Vec2Fixed pp = p1.pos - p2.pos;

		float ppx = static_cast<float>(pp.x);
		float ppy = static_cast<float>(pp.y);

		float p1x = static_cast<float>(p1.pos.x);
		float p1y = static_cast<float>(p1.pos.y);
		float p2x = static_cast<float>(p2.pos.x);
		float p2y = static_cast<float>(p2.pos.y);
		FixedPoint rr = c1.shapeData.r + c2.shapeData.r;
		float rrr = static_cast<float>(rr);
		if (pp * pp < rr * rr) {
			res = true;
		}
	}

	return res;
}
