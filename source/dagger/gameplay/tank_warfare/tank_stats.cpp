#include "tank_stats.h"

#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/game/transforms.h"

#include "gameplay/common/simple_collisions.h"
#include "gameplay/tank_warfare/camera_center.h"

using namespace tank_warfare;

void TankStatsSystem::Run()
{
    auto view = Engine::Registry().view<TankCharacter, Transform, Animator, Sprite>();
    auto viewHS = Engine::Registry().view<TankStats, Transform, Sprite>();

    for (auto entity : view)
    {
        auto& tank = view.get<TankCharacter>(entity);
        auto& transform = view.get<Transform>(entity);
        auto& anim = view.get<Animator>(entity);
        auto& sprite = view.get<Sprite>(entity);

        if (tank.health <= 0)
        {
            tank.zeroHealth = true;
            if (tank.lives <= 0) {
                tank.lastOrientation = ETankOrientation::TankDestroyed;
                AnimatorPlay(anim, "explosion:big");
                if (anim.currentFrame == AnimatorNumberOfFrames(anim))
                {
                    tank.toBeDestroyed = true;
                }
            }
            else
            {
                tank.health = tank.maxHealth;
            }
            tank.lives--;
        }

        for (auto entityHS : viewHS)
        {
            auto& ts = viewHS.get<TankStats>(entityHS);
            auto& t = viewHS.get<Transform>(entityHS);
            auto& s = viewHS.get<Sprite>(entityHS);

            if (tank.id == ts.id && ts.type == EStatType::Health)
            {
                t.position = transform.position;
                t.position.y += (sprite.size.y / 2 + 5);
                if (tank.toBeDestroyed) ts.toBeDestroyed = true;

                if (s.size.x > (tank.health / tank.maxHealth * ts.barWidth) || tank.zeroHealth)
                {
                    s.size.x -= 0.1;
                    s.color.a = 1;
                    ts.barTime = ts.maxBarTime;
                    if (s.size.x <= 0.0)
                    {
                        tank.zeroHealth = false;
                    }
                }
                else if (s.size.x < (tank.health / tank.maxHealth * ts.barWidth))
                {
                    s.size.x = tank.health / tank.maxHealth * ts.barWidth;
                    s.color.a = 1;
                    ts.barTime = ts.maxBarTime;
                }
                else
                {
                    ts.barTime -= Engine::DeltaTime();
                    if (ts.barTime < -1) ts.barTime = 0;
                    if (ts.barTime <= 0)
                    {
                        s.color.a = 0;
                    }
                }
            }

            if (tank.id == ts.id && ts.type == EStatType::Shield)
            {
                t.position = transform.position;
                t.position.y += (sprite.size.y / 2 + 8);
                if (tank.toBeDestroyed) ts.toBeDestroyed = true;

                if (s.size.x > (tank.shield / tank.maxShield * ts.barWidth))
                {
                    s.color.a = 1;
                    ts.barTime = ts.maxBarTime;
                    s.size.x -= 0.1;
                }
                else if (s.size.x < (tank.shield / tank.maxShield * ts.barWidth))
                {
                    s.size.x = tank.shield / tank.maxShield * ts.barWidth;
                    s.color.a = 1;
                    ts.barTime = ts.maxBarTime;
                }
                else
                {
                    ts.barTime -= Engine::DeltaTime();
                    if (ts.barTime < -1) ts.barTime = 0;
                    if (ts.barTime <= 0)
                    {
                        s.color.a = 0;
                    }
                }
            }
        }

        if (tank.powerDuration > 0)
        {
            tank.powerDuration -= Engine::DeltaTime();
        }
        else
        {
            tank.fireRate = tank.maxFireRate / 2;
        }
        
        m_Print += Engine::DeltaTime();
        if (m_Print >= 2.0)
        {
            Logger::info("Tank: {}, Lives: {}, Health: {}, Shield: {}, Coins: {}", tank.id, tank.lives, tank.health, tank.shield, tank.coins);
            m_Print = 0.0;
        }

    }
}

void tank_warfare::CreateTankCharacter(Vector3 pos_, String input_)
{
    auto& reg = Engine::Registry();
    auto entity = reg.create();
    auto& sprite = reg.emplace<Sprite>(entity);
    auto& anim = reg.emplace<Animator>(entity);
    auto& transform = reg.emplace<Transform>(entity);
    auto& collision = reg.emplace<SimpleCollision>(entity);
    auto& input = reg.emplace<InputReceiver>(entity);
    auto& tank = reg.emplace<TankCharacter>(entity);
    auto& cam = reg.emplace<CameraCenter>(entity);
    sprite.scale = { -1, 1 };
    transform.position = pos_;
    collision.size = sprite.size;
    input.contexts.push_back(input_);
    AssignSprite(sprite, "jovanovici:tank:tank3_side");

    auto entityH = reg.create();
    auto& spriteH = reg.emplace<Sprite>(entityH);
    auto& transformH = reg.emplace<Transform>(entityH);
    auto& statsH = reg.emplace<TankStats>(entityH);
    AssignSprite(spriteH, "EmptyWhitePixel");
    spriteH.size = { 0, 2 };
    spriteH.color = { 1, 0, 0, 1 };
    transformH.position = pos_;
    transformH.position.y += (sprite.size.y / 2 + 5);
    statsH.id = tank.id;
    statsH.barWidth = sprite.size.x;
    statsH.type = EStatType::Health;

    auto entityS = reg.create();
    auto& spriteS = reg.emplace<Sprite>(entityS);
    auto& transformS = reg.emplace<Transform>(entityS);
    auto& statsS = reg.emplace<TankStats>(entityS);
    AssignSprite(spriteS, "EmptyWhitePixel");
    spriteS.size = { 0, 2 };
    spriteS.color = { 0, 0, 1, 1 };
    transformS.position = pos_;
    transformS.position.y += (sprite.size.y / 2 + 8);
    statsS.id = tank.id;
    statsS.barWidth = sprite.size.x;
    statsS.type = EStatType::Shield;
}

void TankStatsSystem::SpinUp()
{
    Engine::Dispatcher().sink<NextFrame>().connect<&TankStatsSystem::OnEndOfFrame>(this);
}

void TankStatsSystem::WindDown()
{
    Engine::Dispatcher().sink<NextFrame>().disconnect<&TankStatsSystem::OnEndOfFrame>(this);
}

void TankStatsSystem::OnEndOfFrame()
{
    auto view = Engine::Registry().view<TankCharacter>();
    auto viewHS = Engine::Registry().view<TankStats>();

    for (auto entity : view)
    {
        auto& tank = view.get<TankCharacter>(entity);
        if (tank.toBeDestroyed)
        {
            Engine::Registry().destroy(entity);
        }
    }

    for (auto entity : viewHS)
    {
        auto& ts = viewHS.get<TankStats>(entity);
        if (ts.toBeDestroyed)
        {
            Engine::Registry().destroy(entity);
        }
    }
}