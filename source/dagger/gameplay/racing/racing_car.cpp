#include "racing_car.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"

#include "gameplay/racing/racing_game_logic.h"

using namespace dagger;
using namespace racing_game;

void RacingCarSystem::Run()
{
    RacingGameFieldSettings fieldSettings;
    if (auto* ptr = Engine::GetDefaultResource<RacingGameFieldSettings>())
    {
        fieldSettings = *ptr;
    }

    auto view = Engine::Registry().view<Sprite, Transform, RacingCar>();
    for (auto entity : view)
    {
        auto &t = view.get<Transform>(entity);
        auto &car = view.get<RacingCar>(entity);

        auto &img = view.get<Sprite>(entity);

        t.position.y -= car.speed * Engine::DeltaTime();

        Float32 boarderY = fieldSettings.GetYBoarder();
        if (t.position.y < -boarderY - img.size.y / 2.0f)
        {
            t.position.y = boarderY + img.size.y / 2.0f;
            car.speed = fieldSettings.fieldTileSize * (rand() % 5 + 3);
        }
    }
}
