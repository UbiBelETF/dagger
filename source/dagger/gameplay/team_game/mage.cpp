#include "mage.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/animation.h"

void ancient_defenders::MageBehaviorSystem::SpinUp()
{
    Engine::Dispatcher().sink<NextFrame>().connect<&MageBehaviorSystem::OnEndOfFrame>(this);
}

void ancient_defenders::MageBehaviorSystem::WindDown()
{
    Engine::Dispatcher().sink<NextFrame>().disconnect<&MageBehaviorSystem::OnEndOfFrame>(this);
}

void ancient_defenders::MageBehaviorSystem::Run()
{
    Engine::Registry().view<MageStats, Transform>().each(
        [](MageStats& mage_, Transform& transform_)
    {
        if (mage_.currentAction == EAction::Idling) { // Animations are waiting for spritesheets to be done
            // Play idle animation
            mage_.actionTime += Engine::DeltaTime();

            if (mage_.actionTime > MageStats::s_maxActionTime) {
                mage_.actionTime = 0;
                mage_.currentAction = EAction::Moving;
            }
        }
        else if (mage_.currentAction == EAction::Attacking) {
            // Display attack animation, deal damage to that enemy and go back to idle state
        }
        else if (mage_.currentAction == EAction::Moving) {
            //Play move animation
            transform_.position.x -= mage_.speed * Engine::DeltaTime();

            auto borderX = -300; // Temporary hard code, will be properly implemented later

            mage_.actionTime += Engine::DeltaTime();
            if (mage_.actionTime > MageStats::s_maxActionTime || transform_.position.x <= borderX) {
                mage_.actionTime = 0;
                mage_.currentAction = EAction::Idling;

                if (transform_.position.x < borderX) transform_.position.x = borderX;
            }
        }
    });
  
}

void ancient_defenders::MageBehaviorSystem::OnEndOfFrame()
{
}
