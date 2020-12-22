#include "remote_animation.h"

#include "core/engine.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animations.h"

using namespace dagger;
using namespace team_game;

void RemoteAnimationSystem::Run()
{
	Engine::Registry().view<AnimationExecutor, Animator, Sprite>().each([](AnimationExecutor& exec_, Animator& animator_, Sprite& sprite_)
		{
			if (exec_.source != nullptr)
			{
				if (*(exec_.source))
				{
					*(exec_.source) = false;

					if (exec_.startingSpriteName != "" || exec_.animationName != "")
					{
						AssignSprite(sprite_, exec_.startingSpriteName);
						AnimatorPlay(animator_, exec_.animationName);
					}
				}
			}
		});
}
