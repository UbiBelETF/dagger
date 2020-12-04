#include "tilemap_system.h"
using namespace team_game;
void TilemapSystem::SpinUp() {
	Engine::Dispatcher().sink<TilemapLoadRequest>().connect<&TilemapSystem::OnLoadAsset>(this);
}
void TilemapSystem::Run()
{
}
void TilemapSystem::WindDown() {
	Engine::Dispatcher().sink<TilemapLoadRequest>().disconnect<&TilemapSystem::OnLoadAsset>(this);
}

void TilemapSystem::OnLoadAsset(TilemapLoadRequest request_) {
	Logger::info("Loading tilemap '{}'", request_.path);
	assert(request_.legend != nullptr);
	Tilemap* tilemap = new Tilemap();
	Char ch;
	UInt32 x = 0, y = 10;
	FileInputStream input{ request_.path };
	while (input >> std::noskipws >> ch) {
		if (ch == '\n') {
			x = 0;
			y--;
		}
		else {
			
			if (request_.legend->contains(ch)) {
				tilemap->tiles.push_back((request_.legend->at(ch))(Engine::Registry(), x, y, ch));
			}
			x++;
		}
	}
	
	Engine::Res<Tilemap>()[Filename(request_.path)] = tilemap;
}