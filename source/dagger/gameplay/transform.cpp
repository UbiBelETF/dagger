#include "transform.h"
#include "core/engine.h"
#include "core/graphics/sprite.h"

#include <algorithm>
#include <execution>

using namespace dagger;

void TransformSystem::Run()
{
    auto view = Engine::Registry().view<Sprite, Transform>();
    for(auto entity: view) 
    {
        auto &sprite = view.get<Sprite>(entity);
        auto &transform = view.get<Transform>(entity);

        sprite.position = transform.position;
    }
}