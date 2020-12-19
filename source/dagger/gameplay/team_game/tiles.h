#pragma once

#include "gameplay/team_game/tile.h"
#include "core/core.h"
#include "core/system.h"
#include <functional>


using namespace dagger;

 
	class TilemapLoadingSystem
		: public System
		, public Subscriber<TilemapLoadRequest>
	{
        void OnLoadAsset(TilemapLoadRequest request_);

	public:
		String SystemName() override {
			return "Tilemap Loading System";
		}

		void SpinUp() override;
		void WindDown() override;
	};

