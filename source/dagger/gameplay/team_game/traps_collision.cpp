#include "traps_collision.h"

#include "core/engine.h"
#include "core/graphics/window.h"
#include "core/graphics/text.h"

#include "gameplay/team_game/team_game_collisions.h"
#include "gameplay/team_game/character_controller.h"

using namespace team_game;

void TrapsCollisionSystem::Run()
{
    auto trapsView = Engine::Registry().view<Trap>();
    for (auto& entity : trapsView)
    {
        auto& trap = Engine::Registry().get<Trap>(entity);
        auto& collider = Engine::Registry().get<Collider>(entity);

        if (!trap.hadCollisionWithPlayer)
        {
            if (collider.listOfEntities.size() > 0)
            {
                for (auto& entity : collider.listOfEntities)
                {
                    if (Engine::Registry().has<PlayerCharacter>(entity))
                    {
                        auto& character = Engine::Registry().get<PlayerCharacter>(entity);

                        trap.hadCollisionWithPlayer = true;
                        trap.collisionId = character.id;

                        return;
                    }
                }
            }
        }
    }
}


