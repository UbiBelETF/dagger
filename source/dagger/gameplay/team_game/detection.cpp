#include "detection.h"
#include "core/engine.h"
#include "core/game/transforms.h"

using namespace dagger;

void DetectionSystem::Run()
{
	auto view = Engine::Registry().view<Detection, Transform>();

	auto it = view.begin();
	while (it != view.end()) //reset previous dettection info
	{
		auto& det = view.get<Detection>(*it);
		det.detected = false;

		it++;
	}

	it = view.begin();
	while (it != view.end())
	{
		auto& detection = view.get<Detection>(*it);
		auto& transform = view.get<Transform>(*it);

		auto it2 = it;
		it2++;
		while (it2 != view.end()) {
			auto& det = view.get<Detection>(*it2);
			auto& tr = view.get<Transform>(*it2);

			if (detection.IsDetected(transform.position, det, tr.position)) 
			{
				detection.detected = true;
				detection.who = *it2;

				det.detected = true;
				det.who = *it;
			}
			it2++;
		}
		it++;
	}
}

bool Detection::IsDetected(const Vector3& pos_, const Detection& other_, const Vector3& posOther_)
{
	Float32 distanceFromEdgeToCenters = 0;//sum of distances from the center to the edge of circle(for both that will be radius of circle)
	Float32 distanceFromCenters = 0;//this will be the "real" distance, counting the gap between the edges of circles if existing

	Vector2 p1(pos_.x + pivot.x * size.x, pos_.y + pivot.y * size.y);
	Vector2 p2(posOther_.x + other_.pivot.x * other_.size.x, posOther_.y + other_.pivot.y * other_.size.y);

	distanceFromEdgeToCenters = size.x / 2.0f + other_.size.x / 2.0f;
	distanceFromCenters = sqrt(pow(posOther_.x - pos_.x, 2) + pow(posOther_.y - pos_.y, 2));

	if (distanceFromCenters > distanceFromEdgeToCenters) return false; // collision won't happen regardless of the object positioning because distance between of circles 
	else
	{
		if (p1.x < p2.x + other_.size.x &&
			p1.x + size.x > p2.x &&
			p1.y < p2.y + other_.size.y &&
			p1.y + size.y > p2.y)
		{
			return true;
		}

		return false;
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
