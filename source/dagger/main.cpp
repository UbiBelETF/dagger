
#include "core/engine.h"
#include "gameplay/team_game/team_game_main.h"

int main(int argc_, char** argv_)
{
	dagger::Engine engine;
	return engine.Run<team_game::TeamGame>();
}
