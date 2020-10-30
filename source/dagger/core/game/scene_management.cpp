#include "scene_management.h"

#include "core/core.h"
#include "core/engine.h"

#include "gameplay/examples/character_controller.h"
#include "gameplay/ping_pong/ping_pong_main.h"

using namespace dagger;

EScene SceneManagement::s_currentScene = EScene::Example1;

void SceneManagement::SetupSystems(Engine& engine_)
{
    switch (s_currentScene)
    {
    case dagger::EScene::Example1:
        example1::SetupSystems(engine_);
        break;
    case dagger::EScene::PingPongGame:
        pingPong::SetupSystems(engine_);
        break;
    default:
        break;
    }
}

void SceneManagement::SetupWorld(Engine& engine_)
{
    switch (s_currentScene)
    {
    case dagger::EScene::Example1:
        example1::SetupWorld(engine_);
        break;
    case dagger::EScene::PingPongGame:
        pingPong::SetupWorld(engine_);
        break;
    default:
        break;
    }
}


