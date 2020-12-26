#include "level_generator.h"
#include "tilemap_system.h"
#include "tilemap_legends.h"
#include <string>
using namespace team_game;
void LevelGenerator::GenerateLevel(SInt32 depth_, SInt32 complexity_)
{
	if (depth_ < 2)depth_ = 2;
	if (complexity_ < 1)complexity_ = 1;
	std::set<int> avail_floors;
	floor start_floor;
	std::vector<floor>floor_list;
	start_floor.floor_name = start_floor_name;
	start_floor.object_name = std::to_string((rand() % object_combos) + 1);
	start_floor.creature_name = start_creature_name;
	start_floor.xOffset = 0;
	start_floor.yOffset = 0;
	floor_list.push_back(start_floor);
	SInt32 currXOffset = 0;
	SInt32 currYOffset = 0;
	avail_floors.insert(1);
	avail_floors.insert(2);
	avail_floors.insert(4);
	depth_--;
	SInt32 nextComplexity = 0;
	while (depth_>0) {
		SInt32 left_top = rand() % 2;
		String end_floors[4] = { "3","4","1","2" };
		SInt32 xOffsets[4] = { 0,11,0,-11 };
		SInt32 yOffsets[4] = {11,0,-11,0};
		if (nextComplexity > 4)nextComplexity = 4;
		if (avail_floors.count(4) == 0 && left_top == 0)left_top = 1;
		if (avail_floors.count(1) == 0 && left_top == 1)left_top = 0;
		SInt32 next_main_floor_number = 4 - 3 * left_top;
		nextComplexity = 2 + rand() % (complexity_ + 1);
		if (depth_ == 1) {
			floor end_floor;
			end_floor.creature_name = end_creature_name;
			avail_floors.erase(next_main_floor_number);
			end_floor.floor_name = end_floors[next_main_floor_number-1];
			end_floor.object_name=  std::to_string((rand() % object_combos) + 1);
			end_floor.xOffset = currXOffset + xOffsets[next_main_floor_number-1];
			end_floor.yOffset = currYOffset + yOffsets[next_main_floor_number-1];
			floor_list.push_back(end_floor);
			for (auto single : avail_floors) {
				floor single_end;
				single_end.floor_name = end_floors[single - 1];
				single_end.creature_name= std::to_string((rand() % creature_combos) + 1);
				single_end.object_name= std::to_string((rand() % object_combos) + 1);
				single_end.xOffset = currXOffset + xOffsets[single - 1];
				single_end.yOffset = currYOffset + yOffsets[single - 1];
				bool floor_exists = false;
				for (floor done_floors : floor_list) {
					if (done_floors.xOffset == single_end.xOffset && done_floors.yOffset == single_end.yOffset) {
						floor_exists = true;
						break;
					}
				}
				if (!floor_exists)
					floor_list.push_back(single_end);
			}
		}
		else {
			nextComplexity = 2 + rand() % (complexity_ + 1);
			floor next_main_floor;
			avail_floors.erase(next_main_floor_number);
			for (auto single : avail_floors) {
				floor single_end;
				single_end.floor_name = end_floors[single - 1];
				single_end.creature_name = std::to_string((rand() % creature_combos) + 1);
				single_end.object_name = std::to_string((rand() % object_combos) + 1);
				single_end.xOffset = currXOffset + xOffsets[single - 1];
				single_end.yOffset = currYOffset + yOffsets[single - 1];
				bool floor_exists = false;
				for (floor done_floors : floor_list) {
					if (done_floors.xOffset == single_end.xOffset && done_floors.yOffset == single_end.yOffset) {
						floor_exists = true;
						break;
					}
				}
				if(!floor_exists)
				floor_list.push_back(single_end);

			}
			avail_floors.clear();
			next_main_floor.object_name = std::to_string((rand() % object_combos) + 1);
			next_main_floor.creature_name = std::to_string((rand() % creature_combos) + 1);
			next_main_floor.xOffset = currXOffset + xOffsets[next_main_floor_number - 1];
			next_main_floor.yOffset = currYOffset + yOffsets[next_main_floor_number - 1];
			if (left_top == 0) { //main path going left
				if (nextComplexity == 2) {
					next_main_floor.floor_name = "24";
					avail_floors.insert(4);

				}
				else if (nextComplexity == 3) {
					next_main_floor.floor_name = "124";
					avail_floors.insert(1);
					avail_floors.insert(4);
				}
				else {
					next_main_floor.floor_name = "1234";
					avail_floors.insert(1);
					avail_floors.insert(3);
					avail_floors.insert(4);
				}
				floor_list.push_back(next_main_floor);
			}
			else {             //main path going UP;
				if (nextComplexity == 2) {
					next_main_floor.floor_name = "13";
					avail_floors.insert(1);
				}
				else if (nextComplexity == 3) {
					next_main_floor.floor_name = "134";
					avail_floors.insert(1);
					avail_floors.insert(4);
				}
				else {
					next_main_floor.floor_name = "1234";
					avail_floors.insert(1);
					avail_floors.insert(2);
					avail_floors.insert(4);
				}
				floor_list.push_back(next_main_floor);

			}
			currXOffset = currXOffset + xOffsets[next_main_floor_number - 1];
			currYOffset = currYOffset + yOffsets[next_main_floor_number - 1];
			
		}
		depth_--;
	}
	for (auto floor : floor_list) {
		String floor_name="levels/floor_"+floor.floor_name+".map";
		String object_name="levels/objects_"+floor.object_name+".map";
		String creatures_name = "levels/creatures_" + floor.creature_name + ".map";
		SInt32 xOffset = floor.xOffset;
		SInt32 yOffset = floor.yOffset;
		Tilemap_legends tilemap_legends;
		Engine::Dispatcher().trigger<TilemapLoadRequest>(TilemapLoadRequest{ floor_name, &tilemap_legends.legends.at("floors"),xOffset,yOffset });
		Engine::Dispatcher().trigger<TilemapLoadRequest>(TilemapLoadRequest{ object_name,&tilemap_legends.legends.at("objects"),xOffset,yOffset });
		Engine::Dispatcher().trigger<TilemapLoadRequest>(TilemapLoadRequest{ creatures_name, &tilemap_legends.legends.at("creatures"),xOffset,yOffset });
		
	}
}
