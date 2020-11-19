#pragma once

#include "core/system.h"
#include "core/engine.h"

#include "tools/toolmenu.h"


using namespace dagger;

namespace racing_game {
 
    class RacingToolsSystem
        : public System
        , public Publisher<ToolMenuRender>
    {
        bool m_Restart = false;

    public:
        inline String SystemName() { return "Racing Tools System"; }

        void SpinUp() override;
        void WindDown() override;
        void Run() override;

    private:
        void OnEndOfFrame();
        void RenderToolMenu();

    };
}