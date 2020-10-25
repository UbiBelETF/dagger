#include "SimpleCollisions.h"

#include "core/engine.h"
#include "gameplay/transform.h"

using namespace dagger;

void SimpleCollisionsSystem::Run()
{
    auto view = Engine::Registry().view<SimpleCollision, Transform>();

    auto it = view.begin();
    while(it != view.end())
    {
        auto &collision = view.get<SimpleCollision>(*it);
        auto &transform = view.get<Transform>(*it);

        for (auto it2 = it++; it2 != view.end(); it2++)
        {
            auto &col = view.get<SimpleCollision>(*it2);
            auto &tr = view.get<Transform>(*it2);

            if (collision.IsCollided(transform.position, col, tr.position))
            {
                //it, it2 -- 
            }
        }
    }

    for (auto entity : view)
    {
        

        for (auto entity : view)
        {

        }
    }
}