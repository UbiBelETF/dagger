#include "audio.h"

#if defined(_WIN32)

#include <core/core.h>
#include <core/engine.h>

#include <windows.h>
#include <mmsystem.h>

using namespace dagger;

// on Windows, this does nothing
void Audio::Initialize() {}

void Audio::Load(AssetLoadRequest<Sound> request_) 
{
    FilePath path{ request_.path };
    String name = path.stem().string();
    auto& sounds = Engine::Res<Sound>();

    if (!sounds.contains(name))
        sounds[name] = new Sound();

    FilePath root{ request_.path };
    root.remove_filename();

    String soundName = "";
    {
        String pathName = root.append(path.stem().string()).string();
        if (pathName.find("sounds") == 0)
            pathName = pathName.substr(7, pathName.length() - 15);

        std::replace(pathName.begin(), pathName.end(), '/', ':');
        std::replace(pathName.begin(), pathName.end(), '\\', ':');
        soundName = pathName;
    }

    auto* sound = sounds[soundName];
    sound->name = soundName;
    sound->path = request_.path;
}

void Audio::Play(String name_) 
{
    auto& sounds = Engine::Res<Sound>();
    assert(sounds.contains(name_));
    PlaySound(sounds[name_]->path.c_str(), NULL, SND_FILENAME | SND_ASYNC);
}

void Audio::PlayLoop(String name_) 
{
    auto& sounds = Engine::Res<Sound>();
    assert(sounds.contains(name_));
    PlaySound(sounds[name_]->path.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

void Audio::Stop() 
{
    PlaySound(NULL, NULL, NULL);
}

void Audio::Uninitialize() {
    for (auto sound : Engine::Res<Sound>())
    {
        delete sound.second;
    }

    Engine::Res<Sound>().clear();
}


void AudioSystem::OnLoadAsset(AssetLoadRequest<Sound> request_)
{
    Engine::GetDefaultResource<Audio>()->Load(request_);
}

void AudioSystem::SpinUp()
{
    Engine::PutDefaultResource<Audio>(new Audio());
    auto* audio = Engine::GetDefaultResource<Audio>();
    audio->Initialize();

    Engine::Dispatcher().sink<AssetLoadRequest<Sound>>().connect<&AudioSystem::OnLoadAsset>(this);

    if (Files::exists("sounds") && Files::is_directory("sounds"))
    {
        for (auto& entry : Files::recursive_directory_iterator("sounds"))
        {
            if (entry.path().extension() == ".wav")
                Engine::Dispatcher().trigger<AssetLoadRequest<Sound>>(AssetLoadRequest<Sound>{ entry.path().string() });
        }
    }
}

void AudioSystem::WindDown()
{
    auto* audio = Engine::GetDefaultResource<Audio>();
    delete Engine::GetDefaultResource<Audio>();
}

#endif //defined(_WIN32)