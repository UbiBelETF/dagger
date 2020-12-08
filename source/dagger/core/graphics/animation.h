#pragma once

#include "core/core.h"
#include "core/graphics/texture.h"
#include "core/graphics/sprite.h"

namespace dagger
{
	struct Frame
	{
		String textureName;
		UInt32 relativeLength{ 0 };
		Float64 absoluteLength{ 0 };
		Vector2 pivot;
		SpriteFrame spritesheet;
	};

	struct Animation
	{
		String name;
		UInt32 length{ 0 };
		Float64 absoluteLength{ 0.0 };
		UInt32 frameLengthRelativeSum{ 0 };
		std::vector<Frame> frames{};
	};

	struct Animator
	{
		String currentAnimation;
		Bool animationPlaying{ false };
		UInt32 currentFrame{ 0 };
		Float64 currentFrameTime{ 0 };
	};

	void AnimatorPlay(Animator& animator_, String animationName_);
	void AnimatorStop(Animator& animator_);
	UInt32 AnimatorNumberOfFrames(Animator& animator_);
}