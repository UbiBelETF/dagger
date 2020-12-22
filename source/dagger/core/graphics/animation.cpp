#include "animation.h"

#include "core/graphics/animations.h"

void dagger::AnimatorPlay(Animator& animator_, String animationName_, UInt32 startFromFrame_)
{
	if (animationName_ == animator_.currentAnimation) return;

	animator_.currentAnimation = animationName_;
	animator_.currentFrame = startFromFrame_;
	animator_.currentFrameTime = 0;
	animator_.animationPlaying = true;
}

void dagger::AnimatorStop(Animator& animator_)
{
	animator_.animationPlaying = false;
}
