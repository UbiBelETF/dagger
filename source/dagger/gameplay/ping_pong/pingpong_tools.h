#pragma once

#if defined(DAGGER_DEBUG)

#include "core/system.h"
#include "core/core.h"

#include "tools/toolmenu.h"

using namespace dagger;

namespace ping_pong
{
    class PingPongTools
        : public System
        , public Publisher<ToolMenuRender>
    {
        bool m_RestartGame = false;
        bool m_GenerateBall = false;

    public:
        inline String SystemName() { return "Ping Pong Tools System"; }

        void SpinUp() override;
        void Run() override;
        void WindDown() override;

    protected:
        void RenderToolMenu();
        void OnEndOfFrame();
    };
}

#endif //defined(DAGGER_DEBUG)
