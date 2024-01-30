#pragma once

#include "SDL2/SDL.h"
#undef main
#include "SDL2/SDL_mixer.h"

#include "Helpers.h"

class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	Mix_Chunk* getHitSound();
	Mix_Chunk* getHitInfiniteSound();
	Mix_Chunk* getDestroySound();

	void initialize();
	Mix_Chunk* load(StringView soundPath);
	void play(Mix_Chunk* sound);
	void destroy();

private:
	Mix_Chunk* _hit;
	Mix_Chunk* _hitInfinite;
	Mix_Chunk* _destroy;
};
