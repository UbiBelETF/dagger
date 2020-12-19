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
#include "gameplay/team_game/character.h"
#include "gameplay/team_game/character_controller.h"
#include "gameplay/team_game/team_game_player_input.h"
#include "gameplay/team_game/health_management.h"
#include "gameplay/team_game/ui/player_stats.h"
#include "core/graphics/text.h"

using namespace dagger;

void team_game::TeamGame::GameplaySystemsSetup(Engine &engine_)
{
    engine_.AddSystem<CharacterControllerSystem>();
    engine_.AddSystem<GameManagerSystem>();
    //engine_.AddSystem<TeamGamePlayerInputSystem>();    
    engine_.AddSystem<GravitySystem>();
    engine_.AddSystem<CollisionSystem>();
    engine_.AddSystem<CameraFollowSystem>();
    engine_.AddSystem<HealthManagementSystem>();
    engine_.AddSystem<PlayerStatsUISystem>();
}

void team_game::TeamGame::WorldSetup(Engine &engine_)
{
    ShaderSystem::Use("standard");

    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    camera->zoom = 1;
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
}

void team_game::SetupWorld(Engine& engine_)
{
    auto& reg = engine_.Registry();
    {
        // Add player
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "TeamGame:Characters:Player-Bomb_Guy:Idle:1");

        auto& transform = reg.get_or_emplace<Transform>(entity);
        transform.position = GameManagerSystem::GetPlayerPositionsPerLevel()[GameManagerSystem::GetCurrentLevel() - 1];

        reg.get_or_emplace<CameraFollowFocus>(entity);

        auto& collider = reg.get_or_emplace<Collider>(entity);
        collider.size = sprite.size;
        collider.entityType = CollisionID::PLAYER;
        collider.hasGravity = true;

        auto& gravity = reg.get_or_emplace<Gravity>(entity);
      
        auto& input = reg.emplace<InputReceiver>(entity);
        input.contexts.push_back("Controls");

        reg.emplace<PlayerCharacter>(entity);
        auto& animation = reg.emplace<Animator>(entity);
//        AnimatorPlay(animation, "TeamGame:Player-Bomb_Guy:RUNNING");
        ATTACH_TO_FSM(team_game::AnimationFSM, entity);

        auto& character = reg.emplace<Character>(entity);
        reg.emplace<Text>(entity);


        ATTACH_TO_FSM(team_game::CharacterControllerFSM, entity);
        
        
        // Add enemy
        auto entity2 = reg.create();
        auto& sprite2 = reg.emplace<Sprite>(entity2);
        AssignSprite(sprite2, "TeamGame:Characters:Enemy-Bald_Pirate:1-Idle:1");

        auto& transform2 = reg.get_or_emplace<Transform>(entity2);
        transform2.position = GameManagerSystem::GetPlayerPositionsPerLevel()[GameManagerSystem::GetCurrentLevel()-1];
        transform2.position.x += 400;
        
        auto& collider2 = reg.get_or_emplace<Collider>(entity2);
        collider2.size = sprite2.size;
        collider2.entityType = CollisionID::ENEMY;
        collider2.hasGravity = true;
        
        auto& gravity2 = reg.get_or_emplace<Gravity>(entity2);

        auto& character2 = reg.emplace<Character>(entity2);
        reg.emplace<EnemyCharacter>(entity2);
    }
}
