#pragma once

#include "SDL2/SDL.h"
#undef main

#include <SDL2/SDL_image.h>

#include "Component.h"

#include "Helpers.h"

class Texture : public Component
{
public:
    explicit Texture(SDL_Renderer* renderer, const StringView& imageName);
    ~Texture() final = default;
     
    SDL_Renderer* getRenderer();
    SDL_Texture* getTexture();

    void update() override;

private:
    SDL_Renderer* _renderer = nullptr;
    SDL_Texture* _texture = nullptr;
};