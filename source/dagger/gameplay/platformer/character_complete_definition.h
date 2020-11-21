#include "gameplay/platformer/platformer_main.h"

#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/game/transforms.h"
#include "core/input/inputs.h"


#include "gameplay/platformer/platformer_collision.h"
#include "gameplay/platformer/platformer_controller.h"
#include "gameplay/platformer/platformer_combat.h"


using namespace std;


namespace platformer {

struct Character {
    
        Entity entity;
        Sprite& sprite;
        Transform& transform;
        Animator& animator;
        InputReceiver& input;
        PlatformerCharacter& character;
        PlatformerCollision& col;
        CharacterHealth& chealth;

        static Character Get(Entity entity);

        static Character Create(
            String input_ = "",
            ColorRGB color_ = { 1, 1, 1 },
            Vector2 position_ = { 0, 0 });
    
 };
}