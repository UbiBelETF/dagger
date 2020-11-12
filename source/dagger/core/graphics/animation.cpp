#include "animation.h"

#include "imgui.h"
#include "core/engine.h"
#include "core/graphics/animation.h"
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

void dagger::Animator::PropertiesDrawer(Entity entity_)
{
	if (ImGui::CollapsingHeader("Animator"))
	{
		auto& animator = Engine::Registry().get<Animator>(entity_);
		
		ImGui::Checkbox("Playing Animation", &animator.animationPlaying);
	}
}