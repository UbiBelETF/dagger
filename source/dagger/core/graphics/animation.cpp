#include "animation.h"

#include "core/graphics/animations.h"

void dagger::AnimatorPlay(Animator& animator_, String animationName_)
{
	if (animationName_ == animator_.currentAnimation) return;

	animator_.currentAnimation = animationName_;
	animator_.currentFrame = 0;
	animator_.currentFrameTime = 0;
	animator_.animationPlaying = true;
}

void dagger::AnimatorStop(Animator& animator_)
{
	animator_.animationPlaying = false;
}

UInt32 dagger::AnimatorNumberOfFrames(Animator& animator_)
{
	const auto currentAnimation = AnimationSystem::Get(animator_.currentAnimation);
	return currentAnimation->frames.size() - 1;
}
