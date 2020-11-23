
#include "core/engine.h"
#include "gameplay/ping_pong/ping_pong_main.h"
#include "gameplay/platformer/platformer_main.h"
#include "gameplay/racing/racing_main.h"
#include "gameplay/tank_warfare/tank_warfare_main.h"

int main(int argc_, char** argv_)
{
	dagger::Engine engine;
	return engine.Run<tank_warfare::TankWarfare>();
//	return engine.Run<ping_pong::PingPongGame>();
//	return engine.Run<racing_game::RacingGame>();
//	return engine.Run<platformer::Platformer>();
}
