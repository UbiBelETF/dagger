#include "detection.h"
#include "core/engine.h"
#include "core/game/transforms.h"
#include <gameplay\team_game\character_controller.h>

using namespace dagger;

void DetectionSystem::Run()
{
	auto enemies = Engine::Registry().view<Detection, Transform>();
	auto hero = Engine::Registry().view<Detection, Transform, CharacterController>();

	auto it1 = hero.begin();
	auto it2 = enemies.begin();

	auto& det1 = hero.get<Detection>(*it1); //reset previous dettection info
	det1.detected = false;
	while (it2 != enemies.end()) //reset previous dettection info
	{
		auto& det = enemies.get<Detection>(*it2);
		det.detected = false;

		it2++;
	}

	it2 = enemies.begin();
	while (it1 != hero.end())
	{
		auto& detection = hero.get<Detection>(*it1);
		auto& transform = hero.get<Transform>(*it1);

		while (it2 != enemies.end()) {
			auto& det = enemies.get<Detection>(*it2);
			auto& tr = enemies.get<Transform>(*it2);

			if (detection.IsDetected(transform.position, det, tr.position))
			{
				detection.detected = true;
				detection.who = *it2;
				//detection.where = tr.position;
				if (Engine::Registry().has<CharacterController>(detection.who)) { detection.detected = false; }

				det.detected = true;
				det.who = *it1;
				//det.where = transform.position;
			}
			it2++;
		}
		it1++;
	}
}

bool Detection::IsDetected( Vector3& pos_,  Detection& other_, Vector3& posOther_)
{
	Float32 distanceFromEdgeToCenters = 0;//sum of distances from the center to the edge of circle(for both that will be radius of circle)
	Float32 distanceFromCenters = 0;//this will be the "real" distance, counting the gap between the edges of circles if existing

	Vector2 p1(pos_.x + pivot.x * size.x, pos_.y + pivot.y * size.y);
	Vector2 p2(posOther_.x + other_.pivot.x * other_.size.x, posOther_.y + other_.pivot.y * other_.size.y);

	distanceFromEdgeToCenters = size.x / 2.0f + other_.size.x / 2.0f;
	distanceFromCenters = glm::distance(pos_, posOther_);

	if (distanceFromCenters > distanceFromEdgeToCenters) return false; // collision won't happen regardless of the object positioning because distance between of circles 
	else
	{
		/*if (p1.x < p2.x + other_.size.x &&
			p1.x + size.x > p2.x &&
			p1.y < p2.y + other_.size.y &&
			p1.y + size.y > p2.y)
		{*/
			this->where = posOther_;
			other_.where = pos_;
			return true;
		//}

//		return false;
	}
}

Vector2 Detection::GetDetectionSides(const Vector3& pos_, const Detection& other_, const Vector3& posOther_)
{
	Vector2 res(0, 0);

	Vector2 p(pos_.x + pivot.x * size.x, pos_.y + pivot.y * size.y);
	Vector2 p2(posOther_.x + other_.pivot.x * other_.size.x, posOther_.y + other_.pivot.y * other_.size.y);

	Float32 xDif = std::min(std::abs(p.x + size.x - p2.x), std::abs(p2.x + other_.size.x - p.x));
	Float32 yDif = std::min(std::abs(p.y + size.y - p2.y), std::abs(p2.y + other_.size.y - p.y));

	if (xDif < yDif)
	{
		res.x = std::abs(p.x + size.x - p2.x) < std::abs(p2.x + other_.size.x - p.x) ? 1 : -1;
	}
	else
	{
		res.y = std::abs(p.y + size.y - p2.y) < std::abs(p2.y + other_.size.y - p.y) ? 1 : -1;
	}

	return res;
}

void Detection::SetSize(const Vector2 size_)
{
	size = size_;
}
