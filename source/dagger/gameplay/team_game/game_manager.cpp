#include "game_manager.h"
#include "mage.h"

#include "core/engine.h"

using namespace dagger;
using namespace ancient_defenders;

void ancient_defenders::GameManagerSystem::SpinUp()
{
    Engine::Dispatcher().sink<NextFrame>().connect<&GameManagerSystem::OnEndOfFrame>(this);
}

void ancient_defenders::GameManagerSystem::WindDown()
{
    Engine::Dispatcher().sink<NextFrame>().disconnect<&GameManagerSystem::OnEndOfFrame>(this);
}

void ancient_defenders::GameManagerSystem::Run()
{
    // Spawn enemies

    
}

void ancient_defenders::GameManagerSystem::OnEndOfFrame()
{
    auto player = Engine::GetDefaultResource<PlayerInfo>();
    auto& reg = Engine::Registry();


    auto view = Engine::Registry().view<MageStats>();

    Sequence<Entity> toRemove{};
    auto it = view.begin();
    while (it != view.end()) {
        auto & en = view.get<MageStats>(*it);
        if (en.postition == (WalkingPath::numberOfPoints-1)) {
            player->health += reg.get<Health>(*it).currentHealth;

            toRemove.push_back(entt::to_entity(reg, reg.get<Health>(*it).hpBar));
            toRemove.push_back(*it);
        }
        it++;
    }

    // Remove health from enemies that walked all the way

    Engine::Registry().destroy(toRemove.begin(), toRemove.end());

    auto percentage = player->health / player->maxHealth;

    reg.get<Sprite>(player->sprite).scale.x = percentage * 10.0f;


    // If the game ended display right screen

}
