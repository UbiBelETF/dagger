#include "game_manager.h"

#include <core/engine.h>
#include <core/graphics/sprite.h>
#include <core/game/transforms.h>
#include<core/graphics/text.h>

#include <gameplay/team_game/team_game_main.h>
#include "gameplay/team_game/team_game_collisions.h"
#include "gameplay/team_game/character_controller.h"
#include "gameplay/team_game/collectables.h"
#include "gameplay/team_game/treasure.h"
#include "gameplay/team_game/traps_collision.h"


using namespace dagger;
using namespace team_game;

void GameManagerSystem::SpinUp()
{
    Engine::Dispatcher().sink<NextFrame>().connect<&GameManagerSystem::OnEndOfFrame>(this);
    Engine::Dispatcher().sink<KeyboardEvent>().connect<&GameManagerSystem::OnKeyboardEvent>(this);
}

void GameManagerSystem::WindDown()
{
    Engine::Dispatcher().sink<NextFrame>().disconnect<&GameManagerSystem::OnEndOfFrame>(this);
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&GameManagerSystem::OnKeyboardEvent>(this);
}

void GameManagerSystem::Run()
{
    if (!isGameOver)
    {
        auto& treasureView = Engine::Registry().view<TreasureChest>();
        for (auto& entity : treasureView)
        {
            auto& chest = Engine::Registry().get<TreasureChest>(entity);
            if (chest.hadCollisionWithPlayer)
            {
                isGameOver = true;
                winnerId = chest.playerId;

                Engine::Registry().destroy(entity);
                return;
            }
        }

        auto& trapsView = Engine::Registry().view<Trap>();
        for (auto& entity : trapsView)
        {
            auto& trap = Engine::Registry().get<Trap>(entity);
            if (trap.hadCollisionWithPlayer)
            {
                isGameOver = true;
                winnerId = (1 - trap.collisionId);
                return;
            }
        }
    }
}

void GameManagerSystem::OnKeyboardEvent(KeyboardEvent kEvent_)
{
    if (isGameOver)
    {
        if (kEvent_.key == nextLevelKey && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
        {
            restarted = true;    
        }
    }
}


void GameManagerSystem::LoadNextLevel()
{
    LoadBackDrop();
    LoadPlatforms();
    LoadTraps();
    LoadCollectables();
    gameStarted = false;
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
    LoadTextures(filePath, true, true);
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
        else if (collectableType == "HEALTH")
        {
            collectable.type = CollectableType::HEALTH;
        }
        else
        {
            collectable.type = CollectableType::JUMP;
        }
    }
}

void GameManagerSystem::LoadTextures(String filePath_, Bool addCollision_, Bool isTrap_)
{
    
    FilePath path{ filePath_ };
    std::ifstream fin(Files::absolute(path).string().c_str());

    String baseDir, textureName;
    Float32 zPos, horizontalBlocks, verticalBlocks;
    Vector2 spriteSize, scale, pos;
    Bool addCollision = addCollision_;

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
            }
        }

        if (textureName == "BricksN")
        {
            addCollision = false;
        }

        if (addCollision)
        {
            auto bigBlock = reg.create();
            auto& collider = reg.get_or_emplace<Collider>(bigBlock);
            auto& transform = reg.get_or_emplace<Transform>(bigBlock);

            if (isTrap_)
            {
                collider.entityType = CollisionID::TRAP;
                reg.get_or_emplace<Trap>(bigBlock);
            }
            else
            {
                collider.entityType = CollisionID::TERRAIN;
            }

            collider.state = MovementState::IMMOBILE;
            collider.size.x = spriteSize.x * horizontalBlocks;
            collider.size.y = spriteSize.y * verticalBlocks;

            transform.position = { pos.x + collider.size.x / 2, pos.y + collider.size.y / 2, zPos };
        }

        addCollision = addCollision_;
    }
}

void GameManagerSystem::OnEndOfFrame()
{
    if (isGameOver)
    {
        if (!messageDisplayed)
        {
            auto ui1 = Engine::Registry().create();
            auto& text1 = Engine::Registry().emplace<Text>(ui1);
            text1.spacing = 0.6f;
            text1.Set("pixel-font", "GAME OVER");

            auto ui2 = Engine::Registry().create();
            auto& text2 = Engine::Registry().emplace<Text>(ui2);
            text2.spacing = 0.6f;
            text2.Set("pixel-font", fmt::format("Player {} wins!", winnerId + 1), { 0.0, -30.0, 0.0 });

            auto ui3 = Engine::Registry().create();
            auto& text3 = Engine::Registry().emplace<Text>(ui3);
            text3.spacing = 0.6f;
            text3.Set("pixel-font", "Press Y to restart the game :)", { 0.0, -90.0, 0.0 });

            messageDisplayed = true;
        }
        
        auto& playerView = Engine::Registry().view<PlayerCharacter>();
        for (auto& entity : playerView)
        {
            Engine::Registry().remove(entity);
            Engine::Registry().destroy(entity);
        }
    }

    if (gameStarted)
    {
        Engine::Registry().clear();

        LoadNextLevel();
        team_game::SetupWorld(Engine::Instance());
    }

    if (restarted)
    {
        restarted = false;
        isGameOver = false;
        messageDisplayed = false;

        Engine::Registry().clear();
        LoadNextLevel();

        team_game::SetupWorld(Engine::Instance());
    }
}
