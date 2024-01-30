#include "SoundManager.h"

#include <Helpers.h>

using namespace std::string_view_literals;

constexpr inline const StringView HIT_SOUND_PATH = "resources\\sounds\\hit.wav"sv;
constexpr inline const StringView HIT_INFINITE_SOUND_PATH = "resources\\textures\\hitInfinite.wav"sv;
constexpr inline const StringView DESTROY_SOUND_PATH = "resources\\textures\\destroy.wav"sv;

SoundManager::SoundManager() { initialize(); }
SoundManager::~SoundManager() { destroy(); }

Mix_Chunk* SoundManager::getHitSound() { return _hit; }
Mix_Chunk* SoundManager::getHitInfiniteSound() { return _hitInfinite; }
Mix_Chunk* SoundManager::getDestroySound() { return _destroy; }

void SoundManager::initialize()
{
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    _hit = load(HIT_SOUND_PATH);
    _hitInfinite = load(HIT_INFINITE_SOUND_PATH);
    _destroy = load(DESTROY_SOUND_PATH);
}

Mix_Chunk* SoundManager::load(StringView soundPath)
{
    return Mix_LoadWAV(soundPath.data());
}

void SoundManager::play(Mix_Chunk* sound)
{
    Mix_PlayChannel(-1, sound, 0);
}

void SoundManager::destroy()
{
    Mix_FreeChunk(_hit);
    Mix_FreeChunk(_hitInfinite);
    Mix_FreeChunk(_destroy);

    _hit = nullptr;
    _hitInfinite = nullptr;
    _destroy = nullptr;
}