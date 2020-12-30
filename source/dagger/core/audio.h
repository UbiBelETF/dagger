#pragma once

#include "core/core.h" 
#include "core/system.h"
#include <string>

struct Sound
{
	String name;
	String path;
};

struct Audio 
{
	void Initialize();
	void Load(AssetLoadRequest<Sound> request_);
	void Play(String name_);
	void PlayLoop(String name_);
	void Stop();
	void Uninitialize();
};

struct AudioSystem : public dagger::System
{
	inline String SystemName() override { return "AudioSystem"; }

	void OnLoadAsset(AssetLoadRequest<Sound> request_);
	void SpinUp() override;
	void WindDown() override;
};