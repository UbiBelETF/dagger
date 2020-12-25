#include "game_manager.h"
#include "mage.h"
#include "golem.h"
#include "team_game_main.h"
#include "controls.h"

#include "core/engine.h"
#include "core/graphics/text.h"

#include <random>

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
    auto player = Engine::GetDefaultResource<PlayerInfo>();
    if (player == nullptr) return;

    if ((player->timeLeft -= Engine::DeltaTime()) < 0) {
        m_victory = true;
        return;
    }
    
    Engine::Registry().get<Text>(player->countdownTimer).Set("pixel-font", fmt::format("Raid: {}s left!", (UInt32)player->timeLeft), { 10, 260, 98 });
    Engine::Registry().get<Sprite>(player->countdownTimer).scale = { 0.5,0.5 };

    auto percentage = player->health / player->maxHealth;
    if (percentage < 0.0f) percentage = 0.0f;

    Engine::Registry().get<Sprite>(player->hpSprite).scale.x = percentage * 10.0f;

    if (player->health < 0.0f) {
        m_defeat = true;
        return;
    }
    if ((player->spawnTimer -= Engine::DeltaTime()) > 0) return;


    std::random_device dev;
    std::mt19937 rng(dev());

    std::uniform_int_distribution<std::mt19937::result_type> roll10(0, 9);

    auto spawnCode = roll10(rng);

    if (spawnCode < 4) { // 40% chance to spawn a little golem
        for (auto i = spawnCode + 1; i > 0; i--) { // Create a random number of golems (based on the spawnCode)
            Golem::Create("LITTLE");
        }
        player->spawnTimer = (spawnCode + 1)*1.0f;
    }
    else if (spawnCode < 6) { // 20% chance to spawn a middle golem
        for (auto i = (spawnCode - 4) + 1; i > 0; i--) { // Create a random number of golems (based on the spawnCode)
            Golem::Create("MIDDLE");
        }
        player->spawnTimer = (spawnCode - 3)*2.0f;
    }
    else if (spawnCode < 7) { // 10% chance to spawn a big golem
        Golem::Create("BIG");
        player->spawnTimer = 3.0f;
    }
    // 30% chance to skip spawn
     
}

void ancient_defenders::GameManagerSystem::OnEndOfFrame()
{
    auto player = Engine::GetDefaultResource<PlayerInfo>();
    if (player == nullptr) return;

    auto& reg = Engine::Registry();

    Sequence<Entity> toRemove{};

    auto viewM = reg.view<MageStats>();

    auto itM = viewM.begin();
    while (itM != viewM.end()) {
        auto & en = viewM.get<MageStats>(*itM);
        if (en.postition == (WalkingPath::numberOfPoints-1)) {
            player->health += reg.get<Health>(*itM).currentHealth;

            toRemove.push_back(entt::to_entity(reg, reg.get<Health>(*itM).hpBar));
            toRemove.push_back(*itM);
        }
        itM++;
    }
    
    auto viewE = reg.view<Enemy>();

    auto itE = viewE.begin();
    while (itE != viewE.end()) {
        auto & en = viewE.get<Enemy>(*itE);
        if (en.postition == 1) {
            player->health -= reg.get<Health>(*itE).currentHealth;

            toRemove.push_back(entt::to_entity(reg, reg.get<Health>(*itE).hpBar));
            toRemove.push_back(*itE);
        }
        itE++;
    }

    reg.destroy(toRemove.begin(), toRemove.end());


    if (m_defeat || m_victory) {
        Engine::ToggleSystemsPause(true);

        Engine::GetDefaultResource<PlayerControlsSystem>()->Unpause(); // So that player can restart the game

        auto viewA = reg.view<Animator>();
        
        auto itA = viewA.begin();
        while (itA != viewA.end()) {
            AnimatorStop(reg.get<Animator>(*itA)); // Freeze everything
            itA++;
        }

        
        ancient_defenders::SetupEndScreen(Engine::Instance(), m_victory?true:false);

        m_victory = m_defeat = false;
    }

}
