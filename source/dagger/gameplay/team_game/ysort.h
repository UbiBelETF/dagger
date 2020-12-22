#pragma once

#include "core/core.h"
#include "core/system.h"

struct YSort
{
    char _;
};

class YSortingSystem : public dagger::System
{
    inline String SystemName() { return "Y-Sorting System"; }

    void Run() override;
};
