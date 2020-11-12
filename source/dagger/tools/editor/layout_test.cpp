
#include "layout_test.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/graphics/window.h"

#include "tools/finder.h"

using namespace dagger;
using namespace dagger::editor;

void LayoutTest::GameplaySystemsSetup(Engine& engine_)
{
	engine_.AddSystem<Finder>();
}

void LayoutTest::WorldSetup(Engine& engine_)
{
    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    camera->zoom = 1;
    camera->position = { 0, 0, 0 };
    camera->Update();
}
