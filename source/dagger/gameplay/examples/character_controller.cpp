#include "character_controller.h"

#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"

using namespace example1;

void CharacterControllerSystem::OnInitialize(Registry& registry_, Entity entity_)
{
    InputReceiver& receiver = registry_.get<InputReceiver>(entity_);
    for (auto command : { "run", "jump", "down", "heavy", "light", "use" })
    {
        receiver.values[command] = 0;
    }
}

void CharacterControllerSystem::SpinUp()
{
    Engine::Registry().on_construct<InputReceiver>().connect<&CharacterControllerSystem::OnInitialize>(this);
}

void CharacterControllerSystem::Run()
{
    Engine::Registry().group<InputReceiver, Sprite, Animator>().each(
        [](const InputReceiver input_, Sprite& sprite_, Animator& animator_)
        {
            Float32 run = input_.values.at("run");
            if (run == 0)
            {
                AnimatorPlay(animator_, "souls_like_knight_character:IDLE");
            }
            else
            {
                AnimatorPlay(animator_, "souls_like_knight_character:RUN");
                sprite_.scale.x = run;
                sprite_.position.x += sprite_.scale.x * Engine::DeltaTime();
            }
        });
}

void CharacterControllerSystem::WindDown()
{
    Engine::Registry().on_construct<InputReceiver>().disconnect<&CharacterControllerSystem::OnInitialize>(this);
}


void example1::SetupSystems(Engine& engine_)
{
    engine_.AddSystem<CharacterControllerSystem>();
}

void example1::SetupWorld(Engine& engine_)
{
    ShaderSystem::Use("standard");

    auto& reg = engine_.GetRegistry();
    auto entity = reg.create();

    auto& sprite = reg.emplace<Sprite>(entity);

    auto& anim = reg.emplace<Animator>(entity);
    AnimatorPlay(anim, "souls_like_knight_character:IDLE");

    auto& input = reg.emplace<InputReceiver>(entity);
    input.contexts.push_back("Player");
}
