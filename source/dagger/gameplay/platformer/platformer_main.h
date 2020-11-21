#pragma once

#include "core/game.h"
#include "core/core.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/game/transforms.h"
#include "core/input/inputs.h"

#include "gameplay/common/simple_collisions.h"
#include "gameplay/platformer/platformer_controller.h"
#include "gameplay/platformer/platformer_combat.h"

#define HEALTH_BAR_START_SIZE 50.f

using namespace dagger;

namespace platformer
{
    struct Character
    {
        Entity entity;
        Sprite& sprite;
        Transform& transform;
        Animator& animator;
        InputReceiver& input;
        PlatformerCharacter& character;
        SimpleCollision& col;
        CharacterHealth& chealth;

        static Character Get(Entity entity);

        static Character Create(
            String input_ = "",
            ColorRGB color_ = { 1, 1, 1 },
            Vector2 position_ = { 0, 0 });
    };
    
    class Platformer : public Game
    {
        String GetIniFile() override
        {
            return "platformer.ini";
        }

        void GameplaySystemsSetup(Engine& engine_) override;
        void WorldSetup(Engine& engine_) override;
    };
}