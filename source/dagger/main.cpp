
#include "core/engine.h"
#include "gameplay/ping_pong/ping_pong_main.h"
#include "gameplay/platformer/platformer_main.h"
#include "gameplay/racing/racing_main.h"
#include "gameplay/team_game/team_game_main.h"
#include "gameplay/tiles_example/tiles_example_main.h"

int main(int argc_, char** argv_)
{
	dagger::Engine engine;




//	return engine.Run<tiles_example::TilesExampleMain>();

	return engine.Run<team_game::TeamGame>();

//	return engine.Run<ping_pong::PingPongGame>();

//	return engine.Run<racing_game::RacingGame>();

//  return engine.Run<platformer::Platformer>();

}
