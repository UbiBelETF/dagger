#pragma once

#include "core/system.h"
#include "gameplay/ping_pong/pingpong_ball.h"

using namespace dagger;
using namespace ping_pong;

struct PingPongWinner {
	Float32 lightOnTime{ 2.0f };
	Float32 lightLeft{ 2.0f };
	bool playerWon{ false };
};

class PingPongWinnerSystem : public System {
public:
	inline String SystemName() { return "PingPong Winner System"; }

	void Run() override;

};
