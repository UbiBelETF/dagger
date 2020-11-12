
#include "core/engine.h"
//#include "gameplay/ping_pong/ping_pong_main.h"
//#include "gameplay/platformer/platformer_main.h"
//#include "gameplay/racing/racing_main.h"

#include "tools/editor/layout_test.h"

int main(int argc_, char** argv_)
{
	dagger::Engine engine;
	return engine.Run<dagger::editor::LayoutTest>();
}
