#pragma once
#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"
#include <functional>
#include <set>
namespace team_game {
    class LevelGenerator
    {
        struct floor {
            String floor_name;
            String creature_name;
            String object_name;
            SInt32 xOffset, yOffset;
        };

    private:
        SInt32 creature_combos = 3;
        SInt32   object_combos = 2;
        String  start_floor_name = "124";
        String  end_creature_name = "boss";
        String  start_creature_name = "start";


    public:
        void GenerateLevel(SInt32 depth, SInt32 complexity);

    };
}
