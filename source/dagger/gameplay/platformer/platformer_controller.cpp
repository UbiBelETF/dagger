#include "platformer_controller.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"

#define MAX_JUMP_NUMBER 2

using namespace platformer;

void PlatformerControllerSystem::OnInitialize(Registry& registry_, Entity entity_)
{
    InputReceiver& receiver = registry_.get<InputReceiver>(entity_);
    for (auto command : { "run", "roll","jump", "down", "heavy", "light", "use" })
    {
        receiver.values[command] = 0;
    }
}

void PlatformerControllerSystem::SpinUp()
{
    Engine::Registry().on_construct<InputReceiver>().connect<&PlatformerControllerSystem::OnInitialize>(this);
}

void PlatformerControllerSystem::Run()
{
    Engine::Registry().view<InputReceiver, Sprite, Animator, PlatformerCharacter>().each(
        [](const InputReceiver input_, Sprite& sprite_, Animator& animator_,PlatformerCharacter& char_)
        {
            Float32 run = input_.values.at("run");
            Float32 jump = input_.values.at("jump");
            Float32 roll = input_.values.at("roll");

            if (char_.doubleTap) {
                char_.currentDoubleTapDuration += Engine::DeltaTime();
                if (char_.currentDoubleTapDuration >= char_.doubleTapDurationWindow) {
                    char_.doubleTap = false;
                    char_.currentDoubleTapDuration = 0.f;
                }
            }

            //Reading of the current input values

            if (jump != 0 && !char_.newJump) {
                jump = 0;
            }
            else if (jump == 0) {
                char_.newJump = true;
            }

            if (roll != 0 && jump == 0 && !char_.jumping && !char_.rolling) { // Jump gets higher priority than roll
                if (!char_.doubleTap) {          //First tap of a direction to roll to
                    char_.doubleTap = true;
                    char_.startRoll = roll;
                }
                else if (char_.startRoll != roll) { // Second tap of roll direction , but oposite of the first one
                    char_.currentDoubleTapDuration = 0.f;
                    char_.startRoll = roll;
                }
                else { //Second tap of roll direction in the direction of roll
                    char_.rolling = true;
                    char_.startRoll = 0.f;
                    char_.currentDoubleTapDuration = 0.f;
                    char_.doubleTap = false;
                }
            }

            if (jump != 0 && char_.jumpNumber < MAX_JUMP_NUMBER && !char_.rolling) {
                char_.jumping = true;
                char_.newJump = false;
                char_.jumpNumber++;

                char_.currentJumpSpeed = char_.startJumpSpeed;
                if (char_.falling) {
                    char_.falling = false;
                }
            }


            // Playing animation based on the current input values
            if (char_.jumping) {
                AnimatorPlay(animator_, "souls_like_knight_character:JUMP");
                sprite_.position.y += char_.currentJumpSpeed * Engine::DeltaTime();
                if (run != 0) {
                    sprite_.scale.x = run;
                    sprite_.position.x += char_.speed * sprite_.scale.x * Engine::DeltaTime(); // If character is moving while in the air
                }
               

                char_.currentJumpSpeed -= char_.gravitySpeed * Engine::DeltaTime();

                if (char_.currentJumpSpeed <= 0 && !char_.falling) {
                    char_.jumping = false;
                    char_.falling = true;
                }
            }
            else if (char_.falling) {
                AnimatorPlay(animator_, "souls_like_knight_character:FALLING");
        
                sprite_.position.y += char_.currentJumpSpeed * Engine::DeltaTime();
                if (run != 0) {
                    sprite_.scale.x = run;
                    sprite_.position.x += char_.speed * sprite_.scale.x * Engine::DeltaTime(); // If character is moving while in the air
                }

                char_.currentJumpSpeed -= char_.gravitySpeed * Engine::DeltaTime();

                if (sprite_.position.y <= 0) {
                    char_.falling = false;
                    char_.jumpNumber = 0;
                    sprite_.position.y = 0;
                    char_.currentJumpSpeed = 0.f;
                }
            }
            else if (char_.rolling) {
                if (run != 0) sprite_.scale.x = run; // Enables switching directions while in a roll animation

                AnimatorPlay(animator_, "souls_like_knight_character:ROLL");
                sprite_.position.x += char_.rollingSpeed * sprite_.scale.x * Engine::DeltaTime();

                char_.currentRollingTime += Engine::DeltaTime();
                if (char_.currentRollingTime >= char_.rollingTime) {
                    char_.rolling = false;
                    char_.currentRollingTime = 0.f;
                }
                
                
            }
            else {
                if (run == 0)
                {
                    AnimatorPlay(animator_, "souls_like_knight_character:IDLE");
                    
                }
                else
                {
                    AnimatorPlay(animator_, "souls_like_knight_character:RUN");
                    sprite_.scale.x = run;
                    sprite_.position.x += char_.speed * sprite_.scale.x * Engine::DeltaTime();
                }
            }
            
            
        });
}

void PlatformerControllerSystem::WindDown()
{
    Engine::Registry().on_construct<InputReceiver>().disconnect<&PlatformerControllerSystem::OnInitialize>(this);
}
