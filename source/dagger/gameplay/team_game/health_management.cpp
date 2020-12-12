#include "health_management.h"

#include "core/core.h"
#include "core/engine.h"

#include "gameplay/team_game/character.h"
#include "gameplay/team_game/team_game_collisions.h"

using namespace dagger;
using namespace team_game;

void HealthManagementSystem::Run()
{
    auto& view = Engine::Registry().view< team_game::PlayerCharacter, team_game::Character, team_game::Collider>();
    for (auto& entity : view)
    {
        auto& character = Engine::Registry().get<team_game::Character>(entity);
        auto& collider = Engine::Registry().get<team_game::Collider>(entity);

        for (auto& other : collider.listOfEntities)
        {
            if (Engine::Registry().has<EnemyCharacter>(other))
            {
                auto& enemyCharacter = Engine::Registry().get<team_game::Character>(other);
                character.health -= enemyCharacter.damage * Engine::DeltaTime();
                printf("Lost health. Current HEALTH: %.2f \n", character.health);
            }
        }
    }
    
}
