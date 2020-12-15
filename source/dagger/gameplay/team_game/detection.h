#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

struct Detection {
	Vector2 size;
	Vector2 pivot{ -0.5,-0.5 };

	bool detected = false;
	entt::entity who;

	bool IsDetected(const Vector3& pos_, const Detection& other_, const Vector3& posOther_);

	// return (0,1) if collision happen by y, (1,0) if collision happen by x
	Vector2 GetDetectionSides(const  Vector3& pos_, const Detection& other_, const Vector3& posOther_);

	void SetSize(const Vector2 size_);

};

class DetectionSystem : public System
{
public:
	inline String SystemName() { return "Detection System"; }

	void Run() override;

};