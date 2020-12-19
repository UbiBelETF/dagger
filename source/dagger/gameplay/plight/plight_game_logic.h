#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace plight
{
   
    class PlightGameLogicSystem
        : public System
    {
        bool m_Restart = false;

    public:
        inline String SystemName() { return "Plight Game Logic System System"; }

        void SpinUp() override;
        void WindDown() override;
        void Run() override;

    private:
        void OnEndOfFrame();

    };
}
