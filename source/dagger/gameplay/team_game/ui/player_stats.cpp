#include "player_stats.h"

#include "core/engine.h"
#include "core/graphics/text.h"
#include "core/graphics/window.h"

#include "gameplay/team_game/character.h"

using namespace team_game;

void PlayerStatsUISystem::Run()
{
    auto* camera = Engine::GetDefaultResource<Camera>();

    auto& textView = Engine::Registry().group<PlayerCharacter, Text>();
    for (auto& entity : textView)
    {
        auto& text = Engine::Registry().get<Text>(entity);
        auto& character = Engine::Registry().get<Character>(entity);

        Vector3 healthPosition = camera->position;
        //healthPosition.x -= 300.0f;
        healthPosition.y += 50.0f;
        //healthPosition.z = 0.0f;

        text.Set("pixel-font", fmt::format("Health: {}/100", (UInt8)(character.health)), healthPosition);
    }
}
