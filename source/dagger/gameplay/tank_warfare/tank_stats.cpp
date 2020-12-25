#include "tank_stats.h"

#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/game/transforms.h"

#include "gameplay/common/simple_collisions.h"
#include "gameplay/tank_warfare/camera_center.h"
#include "gameplay/tank_warfare/game_menu.h"

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
                    if (tank.id == 1) GameMenuSystem::EndOfGame("two");
                    else if (tank.id == 2) GameMenuSystem::EndOfGame("one");
                }
            }
            else
            {
                tank.health = tank.maxHealth;
            }
            tank.lives--;
        }

        if (tank.lives > 0)
        {
            if (tank.id == 1 && m_LifeNumPlayerOne < tank.lives) m_LifePlayerOne = true;
            if (tank.id == 2 && m_LifeNumPlayerTwo < tank.lives) m_LifePlayerTwo = true;
        }

        for (auto entityHS : viewHS)
        {
            auto& ts = viewHS.get<TankStats>(entityHS);
            auto& t = viewHS.get<Transform>(entityHS);
            auto& s = viewHS.get<Sprite>(entityHS);

            if (tank.id == ts.id && ts.type == EStatType::Health)
            {
                if (tank.toBeDestroyed) ts.toBeDestroyed = true;

                if (s.size.x > (tank.health / tank.maxHealth * ts.barWidth) || tank.zeroHealth)
                {
                    s.size.x -= 0.8;
                    if (ts.startingPosition.x < 0) t.position.x -= 0.4;
                    if (ts.startingPosition.x > 0) t.position.x += 0.4;
                    if (s.size.x <= 0.0)
                    {
                        tank.zeroHealth = false;
                        t.position = ts.startingPosition;
                    }
                }
                else if (s.size.x < (tank.health / tank.maxHealth * ts.barWidth))
                {
                    s.size.x = tank.health / tank.maxHealth * ts.barWidth;
                }
            }

            if (tank.id == ts.id && ts.type == EStatType::Shield)
            {
                if (tank.toBeDestroyed) ts.toBeDestroyed = true;

                if (s.size.x > (tank.shield / tank.maxShield * ts.barWidth))
                {
                    s.color.a = 1;
                    s.size.x -= 0.8;
                    if (ts.startingPosition.x < 0) t.position.x -= 0.4;
                    if (ts.startingPosition.x > 0) t.position.x += 0.4;
                }
                else if (s.size.x < (tank.shield / tank.maxShield * ts.barWidth))
                {
                    s.size.x = tank.shield / tank.maxShield * ts.barWidth;
                    s.color.a = 1;
                }
                else if (tank.shield == 0)
                {
                    t.position = ts.startingPosition;
                    s.color.a = 0;
                }
            }

            if (tank.id == ts.id && ts.type == EStatType::Life)
            {
                if (tank.lives < ts.life)
                {
                    ts.toBeDestroyed = true;
                    if (tank.id == 1) m_LifeNumPlayerOne--;
                    if (tank.id == 2) m_LifeNumPlayerTwo--;
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

    if (m_LifePlayerOne)
    {
        m_LifeNumPlayerOne++;
        AddHeart({ -250, 250, 0 }, 1, m_LifeNumPlayerOne);
        m_LifePlayerOne = false;
    }

    if (m_LifePlayerTwo)
    {
        m_LifeNumPlayerTwo++;
        AddHeart({ 250, 250, 0 }, 2, m_LifeNumPlayerTwo);
        m_LifePlayerTwo = false;
    }

}

void tank_warfare::CreateTankCharacter(int playerNo_, Vector3 pos_, String input_)
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
    tank.id = playerNo_;
    sprite.scale = { -1, 1 };
    transform.position = pos_;
    collision.size = sprite.size;
    input.contexts.push_back(input_);
    AssignSprite(sprite, "jovanovici:tank:tank3_side");
}

void tank_warfare::CreateUIBars(Vector3 pos_, int id_)
{
    auto& reg = Engine::Registry();

  /*  auto entHBB = reg.create();
    auto& sprHBB = reg.emplace<Sprite>(entHBB);
    auto& traHBB = reg.emplace<Transform>(entHBB);
    AssignSprite(sprHBB, "jovanovici:health_bar2");
    sprHBB.UseAsUI();
    traHBB.position = pos_;
    traHBB.position.z += 2;

    auto entH = reg.create();
    auto& sprH = reg.emplace<Sprite>(entH);
    auto& traH = reg.emplace<Transform>(entH);
    AssignSprite(sprH, "jovanovici:health_bar1");
    sprH.UseAsUI();
    traH.position = pos_;
    traH.position.x -= 100;
    */
    auto entityH = reg.create();
    auto& spriteH = reg.emplace<Sprite>(entityH);
    auto& transformH = reg.emplace<Transform>(entityH);
    auto& statsH = reg.emplace<TankStats>(entityH);
    AssignSprite(spriteH, "EmptyWhitePixel");
    spriteH.size = { 200, 20 };
    spriteH.color = { 1, 0, 0, 1 };
    spriteH.UseAsUI();
    transformH.position = pos_;
    //transformH.position.z += 1;
    statsH.startingPosition = pos_;
    statsH.id = id_;
    statsH.barWidth = 200;
    statsH.type = EStatType::Health;

    auto entityS = reg.create();
    auto& spriteS = reg.emplace<Sprite>(entityS);
    auto& transformS = reg.emplace<Transform>(entityS);
    auto& statsS = reg.emplace<TankStats>(entityS);
    AssignSprite(spriteS, "EmptyWhitePixel");
    spriteS.size = { 0, 20 };
    spriteS.color = { 0, 0, 1, 1 };
    spriteS.UseAsUI();
    transformS.position = pos_;
    statsS.startingPosition = pos_;
    statsS.startingPosition.y -= 25;
    transformS.position.y -= 25;
    statsS.id = id_;
    statsS.barWidth = 200;
    statsS.type = EStatType::Shield;
}

void tank_warfare::AddHeart(Vector3 pos_, int id_, int lives_)
{
    auto& reg = Engine::Registry();

    auto entityL = reg.create();
    auto& spriteL = reg.emplace<Sprite>(entityL);
    auto& statsL = reg.emplace<TankStats>(entityL);
    AssignSprite(spriteL, "jovanovici:powerup:heart");
    spriteL.UseAsUI();
    spriteL.size *= 1.5;
    auto& transformL = reg.emplace<Transform>(entityL);
    transformL.position = pos_;
    transformL.position.y -= 50;
    if (id_ == 1) transformL.position.x -= 89 - ((lives_ - 1) * 31);
    if (id_ == 2) transformL.position.x += 89 - ((lives_ - 1) * 31);
    statsL.life = lives_;
    statsL.id = id_;
    statsL.type = EStatType::Life;
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