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

    Engine::Registry().view<Health>().each(
        [](Health& health_) 
    {
        auto& reg = Engine::Registry();

        auto & sprite = reg.get_or_emplace<Sprite>(health_.hpBar);

        auto val = closestNeighbour(100.0f * health_.currentHealth / health_.maxHealth);

        if (EPSILON_ZERO(val)) return; // Since there is no sprite for 0 hp, skip adding it
        
        AssignSprite(sprite, "spritesheets:hp-bar:hp_"+ std::to_string((UInt32) val));
        auto en = entt::to_entity(reg, health_);
        auto sp = reg.get<Sprite>(en);

        auto ratio = sprite.size.x / sprite.size.y;
        sprite.size = { sp.size.x, sp.size.x / ratio };
        sprite.position = { sp.position.x, sp.position.y - sp.size.y / 2.0f - sprite.size.y / 2.0f, sp.position.z };
    });
}


void ancient_defenders::HealthManagementSystem::OnEndOfFrame()
{
    auto view = Engine::Registry().view<Health>();

    auto it = view.begin();
    while (it != view.end()) {
        auto & en = view.get<Health>(*it);
        if (en.currentHealth <= 0.0f) {
            Engine::Registry().destroy(en.hpBar);
            Engine::Registry().destroy(*it);
        }
        it++;
    }
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
