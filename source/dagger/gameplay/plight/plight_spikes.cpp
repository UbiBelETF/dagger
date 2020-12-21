#define _USE_MATH_DEFINES

#include "plight_spikes.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/game/transforms.h"


#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"



using namespace plight;


void PlightSpikesSystem::Run()
{
    Engine::Registry().view<PlightSpikesFSM::StateComponent>()
        .each([&](PlightSpikesFSM::StateComponent& state_)
            {
                spikesFSM.Run(state_);
            });

   
}

