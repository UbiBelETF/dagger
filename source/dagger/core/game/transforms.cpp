#include "transforms.h"
#include "core/engine.h"
#include "core/graphics/sprite.h"

#include "imgui.h"

using namespace dagger;

void Transform::PropertiesDrawer(Entity entity_)
{
    if (ImGui::CollapsingHeader("Transform"))
    {
        auto& transform = Engine::Registry().get<Transform>(entity_);

        float pos[3] = { transform.position.x, transform.position.y, transform.position.z };
        ImGui::DragFloat3("World Position", pos);
        transform.position = { pos[0], pos[1], pos[2] };
    }
}

void TransformSystem::Run()
{
    // note: groups are much faster than views for such simple tasks as transferring some values
    Engine::Registry().group<Transform, Sprite>().each([](const Transform& transform_, Sprite& sprite_)
        {
            sprite_.position = transform_.position;
        });
}
