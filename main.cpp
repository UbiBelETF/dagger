
#include "dagger.h"
#include "render.h"

#include "escape.h"
#include "keylog.h"
#include "frame_count.h"

#include <spdlog/spdlog.h>

using namespace dagger::engine;

int main(int argc_, char** argv_)
{
	spdlog::set_level(spdlog::level::trace);

	Engine engine;

	engine.AddSystem<dagger::RenderSystem>();
	engine.AddSystem<EscapeSystem>();
	engine.AddSystem<FrameCountSystem>();
	engine.AddSystem<KeyLogSystem>();

	EngineInit(engine);

	while (engine.Up())
	{
		EngineLoop(engine);
	}

	EngineStop(engine);
}
