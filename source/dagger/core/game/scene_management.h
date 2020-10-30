#pragma once

#include "core/core.h"
#include "core/engine.h"

namespace dagger
{
    // all available scenes in game
    enum class EScene
    {
        Example1,
        PingPongGame,

    };

    class SceneManagement
    {
    public:
        static EScene GetCurrentScene() { return s_currentScene; }
        static void SetCurrentScene(EScene currentScene_) { s_currentScene = currentScene_; }

        static void SetupSystems(Engine& engine_);
        static void SetupWorld(Engine& engine_);

    private:
        static EScene s_currentScene;
    };
}
