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

        static Character Get(Entity entity) {
            auto& reg = Engine::Registry();
            auto& sprite = reg.get_or_emplace<Sprite>(entity);
            auto& transform = reg.get_or_emplace<Transform>(entity);
            auto& anim = reg.get_or_emplace<Animator>(entity);
            auto& input = reg.get_or_emplace<InputReceiver>(entity);
            auto& character = reg.get_or_emplace<PlatformerCharacter>(entity);
            auto& col = reg.get_or_emplace<PlatformerCollision>(entity);
            auto& chealth = reg.get_or_emplace<CharacterHealth>(entity);
            return Character{ entity, sprite, transform, anim, input, character,col , chealth };
        }

        static Character Create(
            String input_ = "",
            ColorRGB color_ = { 1, 1, 1 },
            Vector2 position_ = { 0, 0 }) {
            auto& reg = Engine::Registry();
            auto entity = reg.create();
            auto chr = Character::Get(entity);

            chr.sprite.scale = { 1, 1 };
            chr.sprite.position = { position_, 0.0f };
            chr.sprite.color = { color_, 1.0f };

            chr.col.size.x = 30;
            chr.col.size.y = 30;

            chr.transform.position = { position_, 0.0f };

            AssignSpriteTexture(chr.sprite, "souls_like_knight_character:IDLE:idle1");
            AnimatorPlay(chr.animator, "souls_like_knight_character:IDLE");

            if (input_ != "")
                chr.input.contexts.push_back(input_);

            chr.character.speed = 50;
            return chr;
        }
    
};
}