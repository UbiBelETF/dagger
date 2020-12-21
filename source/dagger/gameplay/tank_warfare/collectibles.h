#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace tank_warfare
{
    void AddCollectible(bool isPower_);

    enum struct ECollectibleType
    {
        Coin,
        ExtraLife,
        FireRateUp,
        Health,
        Shield
    };

    struct Collectible
    {
        ECollectibleType collType = { ECollectibleType::Coin };
        bool toBeDestroyed{ false };
    };

    class CollectibleSystem
        : public System
    {
    public:
        static int s_NumCoins;
        static int s_NumPowers;
        const int m_MaxCoins = 20;
        const int m_MaxPowers = 3;
        const int m_PowerDuration = 4;

        inline String SystemName() override {
            return "Collectible System";
        };

        void Run() override;
        void SpinUp() override;
        void WindDown() override;

        static void ResetNumCoinsPowers();

    private:
        void OnEndOfFrame();
    };
}