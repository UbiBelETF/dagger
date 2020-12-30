#include "audio.h"

#if !defined(_WIN32)

#include <core/core.h>

using namespace dagger;

void Audio::Initialize() {}

void Audio::Load(AssetLoadRequest<Sound> request_) {}

void Audio::Play(String name_) {}

void Audio::PlayLoop(String name_) {}

void Audio::Stop() {}

void Audio::Uninitialize() {}


void AudioSystem::OnLoadAsset(AssetLoadRequest<Sound> request_)
{
    Logger::critical("Sound is not yet implemented on Unix");
}

void AudioSystem::SpinUp()
{
    Logger::critical("Sound is not yet implemented on Unix");
}

void AudioSystem::WindDown()
{
    Logger::critical("Sound is not yet implemented on Unix");
}

#endif //!defined(_WIN32)