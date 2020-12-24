#include "death_counter.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/graphics/sprite.h"
#include "core/graphics/text.h"
#include "gameplay/team_game/physics.h"
#include "gameplay/team_game/brawler_controller.h"
#include <algorithm>
#include <execution>

using namespace dagger;
using namespace team_game;

void DeathsCounterSystem::Run()
{
    auto viewCharacter = Engine::Registry().view<BrawlerCharacter>();
    auto view = Engine::Registry().view< Text>();
    for (auto entity : view)
    {
        auto& t = view.get<Text>(entity);
        for (auto entity2 : viewCharacter)
        {
            auto& c = viewCharacter.get<BrawlerCharacter>(entity2);
            if (c.player)
            t.Set("pixel-font", fmt::format("x {}", c.deaths), { -300, 262,0 });
        }
    }
}