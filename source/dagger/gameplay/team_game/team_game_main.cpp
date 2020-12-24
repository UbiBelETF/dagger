#include "team_game_main.h"

#include <fstream>

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/animations.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite_render.h"
#include "core/graphics/textures.h"
#include "core/graphics/animations.h"
#include "core/graphics/gui.h"
#include "tools/diagnostics.h"

#include "gameplay/team_game/team_game_collisions.h"
#include "gameplay/team_game/camera_follow.h"
#include "gameplay/team_game/gravity.h"
#include "gameplay/team_game/game_manager.h"
#include "gameplay/team_game/character_controller.h"
#include "gameplay/team_game/team_game_player_input.h"
#include "gameplay/team_game/traps_collision.h"
#include "gameplay/team_game/collectables.h"
#include "gameplay/team_game/treasure.h"

using namespace dagger;

void team_game::TeamGame::GameplaySystemsSetup(Engine& engine_)
{
    engine_.AddSystem<CharacterControllerSystem>();
    engine_.AddSystem<GravitySystem>();
    engine_.AddSystem<CollisionSystem>();
    engine_.AddSystem<CameraFollowSystem>();
    engine_.AddSystem<TrapsCollisionSystem>();
    engine_.AddSystem<CollectablePickupSystem>();
    engine_.AddSystem<TreasureSystem>();
    engine_.AddSystem<GameManagerSystem>();
}

void team_game::TeamGame::WorldSetup(Engine &engine_)
{
    ShaderSystem::Use("standard");

    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    camera->zoom = 0.9;
    camera->position = { 0, 0, 0 };
    camera->Update();

    String filePath = "levels/player_positions.txt";
    FilePath path{ filePath };
    std::ifstream fin(Files::absolute(path).string().c_str());
    Vector3 playerPos;

    while (fin >> playerPos.x >> playerPos.y >> playerPos.z)
    {
        GameManagerSystem::GetPlayerPositionsPerLevel().push_back(playerPos);
    }

    GameManagerSystem::SetCurrentLevel(1);
}

void team_game::SetupWorld(Engine& engine_)
{
    auto& reg1 = engine_.Registry();
    {
        auto entity = reg1.create();
        auto& sprite = reg1.emplace<Sprite>(entity);
        AssignSprite(sprite, "TeamGame:Characters:Player-Bomb_Guy:Idle:1");

        auto& transform = reg1.get_or_emplace<Transform>(entity);
        transform.position = GameManagerSystem::GetPlayerPositionsPerLevel()[GameManagerSystem::GetCurrentLevel() - 1];

        reg1.get_or_emplace<CameraFollowFocus>(entity);

        auto& collider = reg1.get_or_emplace<Collider>(entity);
        collider.size = sprite.size;
        collider.entityType = CollisionID::PLAYER;
        collider.hasGravity = true;

        auto& gravity = reg1.get_or_emplace<Gravity>(entity);

        auto& input = reg1.emplace<InputReceiver>(entity);
        input.contexts.push_back("Arrows");


        auto& animation = reg1.emplace<Animator>(entity);
        ATTACH_TO_FSM(team_game::AnimationFSM, entity);

        auto& character = reg1.emplace<PlayerCharacter>(entity);
        character.id = 0;
        ATTACH_TO_FSM(team_game::CharacterControllerFSM, entity);

        
        // Add a trap
        auto entity2 = reg.create();
        auto& sprite2 = reg.get_or_emplace<Sprite>(entity2);
        AssignSprite(sprite2, "TeamGame:Other:Spikes");

        auto& transform2 = reg.get_or_emplace<Transform>(entity2);

        Vector2 newSize = { 64.0, 64.0 };
        Vector2 scale;
        scale.x = newSize.x / sprite2.size.x;
        scale.y = newSize.y / sprite2.size.y;
        sprite2.scale = scale;

        transform2.position = { 175.0 + newSize.x/2, -108.0 + newSize.y/2, 4.0 };

        auto& collider2 = reg.get_or_emplace<Collider>(entity2);
        collider2.size = sprite2.size;
        collider2.entityType = CollisionID::TRAP;
        collider2.hasGravity = true;
        //Other Spikes 4.0 64.0 64.0 175.0 -108.0 1 1
    }

    auto& reg2 = engine_.Registry();
    {
        auto entity = reg2.create();
        auto& sprite = reg2.emplace<Sprite>(entity);
        AssignSprite(sprite, "TeamGame:Characters:Enemy-Bald_Pirate:Idle:1");

        auto& transform = reg2.get_or_emplace<Transform>(entity);
        transform.position = GameManagerSystem::GetPlayerPositionsPerLevel()[GameManagerSystem::GetCurrentLevel()];

        reg2.get_or_emplace<CameraFollowFocus>(entity);

        auto& collider = reg2.get_or_emplace<Collider>(entity);
        collider.size = sprite.size;
        collider.entityType = CollisionID::PLAYER;
        collider.hasGravity = true;

        auto& gravity = reg2.get_or_emplace<Gravity>(entity);

        auto& input = reg2.emplace<InputReceiver>(entity);
        input.contexts.push_back("ASDW");


        auto& animation = reg2.emplace<Animator>(entity);
        ATTACH_TO_FSM(team_game::AnimationFSM, entity);

        auto& character = reg2.emplace<PlayerCharacter>(entity);
        character.id = 1;
        ATTACH_TO_FSM(team_game::CharacterControllerFSM, entity);
    }

    {
        auto entity = reg1.create();
        auto& sprite = reg1.get_or_emplace<Sprite>(entity);
        sprite.scale = { 2.0, 2.0 };
        AssignSprite(sprite, "TeamGame:Other:Chest");

        auto& transform = reg1.get_or_emplace<Transform>(entity);
        transform.position = { 468, 270, 1 };

        auto& collider = reg1.get_or_emplace<Collider>(entity);
        collider.size = sprite.size;
        collider.entityType = CollisionID::COLLECTABLE;
        collider.hasGravity = false;

        auto& treasure = reg1.get_or_emplace<TreasureChest>(entity);
    }
}