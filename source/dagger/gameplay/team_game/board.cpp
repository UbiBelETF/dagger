#include "board.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/graphics/sprite.h"

void Peep::SetState(PeepState state_)
{
	dagger::Engine::Registry().get<dagger::Sprite>(this->entity).grayscale = state_ == PeepState::Active ? 0.0f : 1.0f;
}