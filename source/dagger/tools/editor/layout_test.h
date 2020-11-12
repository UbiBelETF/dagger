#pragma once

#include "core/core.h"
#include "core/game.h"

namespace dagger
{
    namespace editor
    {
        class LayoutTest : public Game
        {
            inline String GetIniFile() { return "editor.ini"; };

            void GameplaySystemsSetup(Engine& engine_) override;
            void WorldSetup(Engine& engine_) override;
        };
    }
}