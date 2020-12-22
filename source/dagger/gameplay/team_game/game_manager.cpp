#include "game_manager.h"

#include <core/engine.h>
#include <core/graphics/sprite.h>
#include <core/game/transforms.h>

#include <gameplay/team_game/team_game_main.h>
#include "gameplay/team_game/team_game_collisions.h"
#include "gameplay/team_game/collectables.h"


using namespace dagger;
using namespace team_game;

void GameManagerSystem::SpinUp()
{
    Engine::Dispatcher().sink<NextFrame>().connect<&GameManagerSystem::OnEndOfFrame>(this);
}

void GameManagerSystem::WindDown()
{
    Engine::Dispatcher().sink<NextFrame>().disconnect<&GameManagerSystem::OnEndOfFrame>(this);
}

void GameManagerSystem::Run()
{
}

void GameManagerSystem::LoadNextLevel()
{
    currentLevel++;
    LoadBackDrop();
    LoadPlatforms();
    LoadTraps();
    LoadCollectables();
    completedObjective = false;
}

void GameManagerSystem::LoadBackDrop()
{
    String filePath = fmt::format("levels/backdrop/backdrop_{}.txt", currentLevel);
    LoadTextures(filePath, false);
}

void GameManagerSystem::LoadPlatforms()
{
    String filePath = fmt::format("levels/platforms/platforms_{}.txt", currentLevel);
    LoadTextures(filePath, true);
}

void GameManagerSystem::LoadTraps()
{
    String filePath = fmt::format("levels/traps/traps_{}.txt", currentLevel);
    LoadTextures(filePath, true);
}

void GameManagerSystem::LoadCollectables()
{
    String filePath = fmt::format("levels/collectables/collectables_{}.txt", currentLevel);
    FilePath path{ filePath };
    std::ifstream fin(Files::absolute(path).string().c_str());

    String baseDir, textureName, collectableType;
    Float32 zPos, improvement, powerUpTime;
    Vector2 spriteSize, scale, pos;

    auto& reg = Engine::Registry();

    while (fin >> baseDir >> textureName >> zPos >>
        spriteSize.x >> spriteSize.y >>
        pos.x >> pos.y >> 
        collectableType >> improvement >> powerUpTime)
    {
        auto block = reg.create();
        auto& spriteBlock = reg.get_or_emplace<Sprite>(block);

        String rootDir = "TeamGame:";
        AssignSprite(spriteBlock, rootDir + baseDir + ":" + textureName);
        auto& transform = reg.get_or_emplace<Transform>(block);
        transform.position = {spriteSize.x / 2 + pos.x,
                              spriteSize.y / 2 + pos.y,
                              zPos};

        scale.x = spriteSize.x / spriteBlock.size.x;
        scale.y = spriteSize.y / spriteBlock.size.y;

        spriteBlock.scale = scale;
            
        auto& collider = reg.get_or_emplace<Collider>(block);
        collider.entityType = CollisionID::COLLECTABLE;
        collider.state = MovementState::IMMOBILE;
        collider.size = spriteBlock.size * spriteBlock.scale;

        auto& collectable = reg.get_or_emplace<Collectable>(block);
        collectable.improvement = improvement;
        collectable.powerUpTime = powerUpTime;
        collectable.timeLeft = collectable.powerUpTime;

        if (collectableType == "SPEED")
        {
            collectable.type = CollectableType::SPEED;
        }
        else
        {
            collectable.type = CollectableType::HEALTH;
        }
    }
}

void GameManagerSystem::LoadTextures(String filePath_, Bool addCollision_)
{
    
    FilePath path{ filePath_ };
    std::ifstream fin(Files::absolute(path).string().c_str());

    String baseDir, textureName;
    Float32 zPos, horizontalBlocks, verticalBlocks;
    Vector2 spriteSize, scale, pos;

    auto& reg = Engine::Registry();

    while (fin >> baseDir >> textureName >> zPos >>
        spriteSize.x >> spriteSize.y >>
        pos.x >> pos.y >>
        horizontalBlocks >> verticalBlocks)
    {
        for (int i = 0; i < horizontalBlocks; i++)
        {
            for (int j = 0; j < verticalBlocks; j++)
            {
                auto block = reg.create();
                auto& spriteBlock = reg.get_or_emplace<Sprite>(block);

                String rootDir = "TeamGame:";
                AssignSprite(spriteBlock, rootDir + baseDir + ":" + textureName);
                auto& transform = reg.get_or_emplace<Transform>(block);
                transform.position = { i * spriteSize.x + spriteSize.x / 2 + pos.x,
                                     j * spriteSize.y + spriteSize.y / 2 + pos.y,
                                     zPos };

                scale.x = spriteSize.x / spriteBlock.size.x;
                scale.y = spriteSize.y / spriteBlock.size.y;

                spriteBlock.scale = scale;

                if (addCollision_)
                {                  
                    auto& collider = reg.get_or_emplace<Collider>(block);
                    collider.entityType = CollisionID::TERRAIN;
                    collider.state = MovementState::IMMOBILE;
                    collider.size = spriteBlock.size * spriteBlock.scale;
                }
            }
        }
    }
}

void GameManagerSystem::OnEndOfFrame()
{
    if (completedObjective)
    {
        Engine::Registry().clear();

        LoadNextLevel();
        team_game::SetupWorld(Engine::Instance());
    }
}
