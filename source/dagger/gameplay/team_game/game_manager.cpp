#include "game_manager.h"

#include <core/engine.h>
#include <core/graphics/sprite.h>

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
    auto view = Engine::Registry().view<Level>();
    for (auto entity : view)
    {
        auto& level = view.get<Level>(entity);
        UInt8 currentLevel = level.level;

        if (level.completedObjective)
        {
            auto& level = Engine::Registry().get_or_emplace<Level>(entity);
            level.level = currentLevel;
            level.completedObjective = false;

            Engine::Registry().view<ComponentOfLevel>().each([](ComponentOfLevel& levelComponent_) 
            {
                levelComponent_.discard = true;
            });

            LoadNextLevel(level);
        }
    }
}

void GameManagerSystem::LoadNextLevel(Level& level_)
{
    level_.level += 1;
    LoadBackDrop(level_.level);
    LoadPlatforms(level_.level);
}

void GameManagerSystem::LoadBackDrop(UInt8 level_)
{
    String levelName = fmt::format("levels/backdrop_{}.txt", level_);
    FilePath path{ levelName };
    std::ifstream fin(Files::absolute(path).string().c_str());

    String baseDir, textureName;
    Float32 zPos, horizontalBlocks, verticalBlocks;
    Vector2 blockSize, scale, bias;

    auto& reg = Engine::Registry();

    while (fin >> baseDir >> textureName >> zPos >>
        blockSize.x >> blockSize.y >>
        bias.x >> bias.y >>
        horizontalBlocks >> verticalBlocks)
    {
        for (int i = 0; i < horizontalBlocks; i++)
        {
            for (int j = 0; j < verticalBlocks; j++)
            {
                auto block = reg.create();
                auto& spriteBlock = reg.get_or_emplace<Sprite>(block);
                auto& componentOfLevel = reg.get_or_emplace<ComponentOfLevel>(block);

                String rootDir = "TeamGame:";
                AssignSpriteTexture(spriteBlock, rootDir + baseDir + ":" + textureName);
                spriteBlock.position = { i * blockSize.x + blockSize.x / 2 + bias.x,
                                         j * blockSize.y + blockSize.y / 2 + bias.y,
                                         zPos };

                scale.x = blockSize.x / spriteBlock.size.x;
                scale.y = blockSize.y / spriteBlock.size.y;

                spriteBlock.scale = scale;
            }
        }
    }
}

void GameManagerSystem::LoadPlatforms(UInt8 level_)
{

}

void GameManagerSystem::OnEndOfFrame()
{
    auto levelComponentsView = Engine::Registry().view<ComponentOfLevel>();

    for (auto& entity : levelComponentsView)
    {
        auto& levelComponent = levelComponentsView.get<ComponentOfLevel>(entity);
        if (levelComponent.discard)
        {
            Engine::Registry().remove_all(entity);
        }

        if (Engine::Registry().orphan(entity))
        {
            Engine::Registry().destroy(entity);
        }
    }
}
