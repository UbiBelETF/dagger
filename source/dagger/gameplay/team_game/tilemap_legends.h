#pragma once
#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"
#include <functional>
#include "tilemap.h"
static Map<Char, String>wall_type;
class Tilemap_legends
{
private:
    
public:
    Tilemap_legends();
    Map<String, TilemapLegend>legends;
};
struct TeamGamePot {
    int startHp = 20;
};
struct TeamGameCrate {
    int startHp = 100;
};
Entity CreateFloor(Registry& reg_, SInt32 x_, SInt32 y_,char type);
Entity CreateWall(Registry& reg_, SInt32 x_, SInt32 y_, char type);
Entity CreateSlime(Registry& reg_, SInt32 x_, SInt32 y_,char type);
Entity CreateHero(Registry& reg_, SInt32 x_, SInt32 y_,char type);
Entity CreateDoorHorizontal(Registry& reg_, SInt32 x_, SInt32 y_,char type);
Entity CreateDoorVertical(Registry& reg_, SInt32 x_, SInt32 y_,char type);
Entity CreateChest(Registry& reg_, SInt32 x_, SInt32 y_, char type);
Entity CreatePot(Registry& reg_, SInt32 x_, SInt32 y_, char type);
Entity CreateCrate(Registry& reg_, SInt32 x_, SInt32 y_, char type);
Entity CreateSlimeBoss(Registry& reg_, SInt32 x_, SInt32 y_, char type);
