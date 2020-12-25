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
    auto view = Engine::Registry().view< Text, CounterType>();
    for (auto entity : view)
    {
        auto& ty = view.get<CounterType>(entity);
            if (ty.type) {
                auto& t = view.get<Text>(entity);
                for (auto entity2 : viewCharacter)
                {
                    auto& c = viewCharacter.get<BrawlerCharacter>(entity2);
                    if (c.player)
                        t.Set("pixel-font", fmt::format("x {}", c.healthHearts), { -300, 222,0 });
                }
            }
            else {
                auto& t = view.get<Text>(entity);
                for (auto entity2 : viewCharacter)
                {
                    auto& c = viewCharacter.get<BrawlerCharacter>(entity2);
                    if (c.player)
                        t.Set("pixel-font", fmt::format("x {}", c.deaths), { -300, 262,0 });
                }
            }
    }
    auto viewCharacterc = Engine::Registry().view<BrawlerCharacter>();
    auto views = Engine::Registry().view< Sprite, BarOrCredits>();
    for (auto entity : views)
    {
        auto& ty = views.get<BarOrCredits>(entity);
        if (ty.bar) {
            auto& t = views.get<Sprite>(entity);
            for (auto entity2 : viewCharacter)
            {
                auto& c = viewCharacterc.get<BrawlerCharacter>(entity2);
                if (!c.player) {
                    if (c.inRoom) {
                        AssignSprite(t, fmt::format("team_game:Healthbar:bar{}", c.healthHearts));
                    }
                    else AssignSprite(t, "team_game:Healthbar:barnone");
                }
            }
        }
        else {
            auto& t = views.get<Sprite>(entity);
            for (auto entity2 : viewCharacter)
            {
                auto& c = viewCharacterc.get<BrawlerCharacter>(entity2);
                if (!c.player) {
                    if (c.healthHearts==0) {
                        AssignSprite(t, "team_game:End");
                    }
                }
            }
        }
    }
}