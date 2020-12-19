#include "gameplay/team_game/tiles.h"
#include "gameplay/team_game/tile.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/graphics/shaders.h"
#include "core/graphics/textures.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/window.h"
#include <ios>


using namespace dagger;

void TilemapLoadingSystem::OnLoadAsset(TilemapLoadRequest request_)
{

    assert(request_.legend != nullptr);
    Tilemap* tilemap = new Tilemap();

    Char ch;
    UInt32 x = 0, y = 0;

    FileInputStream input{request_.path};

    while (input >> std::noskipws >> ch) 
    {
        if(ch == '\n')
        {
            x = 0; y++;
        }
        else
        {
           assert(request_.legend->contains(ch));
           tilemap->tiles.push_back(request_.legend->at(ch)(Engine::Registry(), x, y));
           x++;
        }
    }
     Engine::Res<Tilemap>()[request_.path] = tilemap;
    
    
   
}

void TilemapLoadingSystem::SpinUp()
{
    Engine::Dispatcher().sink<TilemapLoadRequest>().connect<&TilemapLoadingSystem::OnLoadAsset>(this);

}

void TilemapLoadingSystem::WindDown()
{
    Engine::Dispatcher().sink<TilemapLoadRequest>().disconnect<&TilemapLoadingSystem::OnLoadAsset>(this);
}

