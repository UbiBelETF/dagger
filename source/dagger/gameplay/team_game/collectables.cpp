#include "collectables.h"

#include <core/engine.h>
#include <core/graphics/sprite.h>

#include "gameplay/team_game/character_controller.h"
#include "gameplay/team_game/team_game_collisions.h"
#include "gameplay/team_game/gravity.h"

using namespace team_game;

void CollectablePickupSystem::SpinUp()
{
    Engine::Dispatcher().sink<NextFrame>().connect<&CollectablePickupSystem::OnEndOfFrame>(this);
}

void CollectablePickupSystem::WindDown()
{
    Engine::Dispatcher().sink<NextFrame>().disconnect<&CollectablePickupSystem::OnEndOfFrame>(this);
}

void CollectablePickupSystem::Run()
{
    auto collectableView = Engine::Registry().view<Collectable>();
    for (auto& entity : collectableView)
    {
        auto& collectable = Engine::Registry().get<Collectable>(entity);
        if (!collectable.removed)
        {
            auto& collider = Engine::Registry().get<Collider>(entity);
            if (collider.listOfEntities.size() > 0)
            {
                for (auto& entity : collider.listOfEntities)
                {
                    if (Engine::Registry().has<PlayerCharacter>(entity))
                    {
                        auto& character = Engine::Registry().get<PlayerCharacter>(entity);

                        if (collectable.type == CollectableType::HEALTH)
                        {
                            character.health += collectable.improvement;
                        }
                        else if (collectable.type == CollectableType::SPEED)
                        {
                            character.speed *= collectable.improvement;
                        }
                        else
                        {
                            auto& gravity = Engine::Registry().get<Gravity>(entity);
                            gravity.verticalInitialSpeed *= collectable.improvement;
                        }

                        collectable.pickedUp = true;
                        collectable.userId = character.id;
                    }
                }
            }
        }
        else
        {
            if (collectable.type == CollectableType::HEALTH)
            {
                collectable.finished = true;
            }
            else
            {
                collectable.timeLeft -= Engine::DeltaTime();
                if (collectable.timeLeft <= 0)
                {
                    auto& playerView = Engine::Registry().view<PlayerCharacter>();
                    for (auto& entity : playerView)
                    {
                        auto& character = Engine::Registry().get<PlayerCharacter>(entity);
                        if (character.id == collectable.userId)
                        {
                            if (collectable.type == CollectableType::SPEED)
                            {
                                character.speed /= collectable.improvement;
                            }
                            else
                            {
                                auto& gravity = Engine::Registry().get<Gravity>(entity);
                                gravity.verticalInitialSpeed /= collectable.improvement;
                            }
                        }
                    }

                    collectable.finished = true;
                }
            }
        }
    }
}

void CollectablePickupSystem::OnEndOfFrame()
{
    auto& view = Engine::Registry().view<Collectable>();

    for (auto& entity : view)
    {
        auto& collectable = Engine::Registry().get<Collectable>(entity);

        if (collectable.pickedUp && !collectable.removed)
        {
            collectable.removed = true;
            Engine::Registry().remove<Sprite, Collider>(entity);
        }

        if (collectable.finished)
        {
            Engine::Registry().destroy(entity);
        }
    }
}
