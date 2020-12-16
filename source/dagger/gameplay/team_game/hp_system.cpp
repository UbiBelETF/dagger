#include "hp_system.h"

#include "core/engine.h"
#include "core/graphics/sprite.h"
#include "core/game/transforms.h"


using namespace dagger;
using namespace ancient_defenders;

Sequence<Float32> ancient_defenders::Health::hpSteps = {0, 11, 22, 33, 44, 55, 66, 77, 88, 100 };

void ancient_defenders::HealthManagementSystem::SpinUp()
{
    Engine::Dispatcher().sink<NextFrame>().connect<&HealthManagementSystem::OnEndOfFrame>(this);
}

void ancient_defenders::HealthManagementSystem::WindDown()
{
    Engine::Dispatcher().sink<NextFrame>().disconnect<&HealthManagementSystem::OnEndOfFrame>(this);
}

void ancient_defenders::HealthManagementSystem::Run() {

    auto& reg = Engine::Registry();
    auto entities = reg.view<Health>();

    entities.each([&](Entity entity_, Health& health_)
        {
            auto& healthBarSprite = reg.get_or_emplace<Sprite>(health_.hpBar);
            AssignSprite(healthBarSprite, "spritesheets:hp-bar:hp_100");
            
            auto health = health_.currentHealth / health_.maxHealth;
            health = 0.5f;
            if (EPSILON_ZERO(health)) {
                healthBarSprite.color = { 0,0,0,0 }; // Make the previous sprite invisible; solves previous sprite staying still after character drops to low HP
            }
            else
            {
                const auto& parentSprite = reg.get<Sprite>(entity_);

                healthBarSprite.scale.x = health * 0.5f;
                healthBarSprite.position = parentSprite.position;
                healthBarSprite.position.x -= (1.0f - health) * (parentSprite.size.x / 2.0f);
                healthBarSprite.position.y -= parentSprite.size.y / 2.0f;
                healthBarSprite.position.z = 0.0f;
            }
        });
}


void ancient_defenders::HealthManagementSystem::OnEndOfFrame()
{
    auto view = Engine::Registry().view<Health>();
    
    Sequence<Entity> toRemove{};
    auto it = view.begin();
    while (it != view.end()) {
        auto & en = view.get<Health>(*it);
        if (en.currentHealth <= 0.0f) {
            toRemove.push_back(en.hpBar);
            toRemove.push_back(*it);
        }
        it++;
    }

    Engine::Registry().destroy(toRemove.begin(), toRemove.end());
}

Float32 ancient_defenders::closestNeighbour(Float32 number_)
{
    SInt32 numOfSteps = Health::hpSteps.size()-1;
    
    SInt32 i = 0;
    while (number_ > Health::hpSteps[i]) i++;

    if (i == 0) {
        return Health::hpSteps[0];
    }
    else {
        if (std::abs(Health::hpSteps[i - 1] - number_) < std::abs(Health::hpSteps[i] - number_)) {
            return Health::hpSteps[i - 1];
        }
        else {
            return Health::hpSteps[i];
        }
    }
}
