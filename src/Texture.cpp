#include "Texture.h"

Texture::Texture(SDL_Renderer* renderer, const StringView& imageName) 
    : Component(ComponentType::TEXTURE), _renderer(renderer) {
    SDL_Surface* surface = IMG_Load(imageName.data());
    if (surface) 
    {
        _texture = SDL_CreateTextureFromSurface(_renderer, surface);

        SDL_SetTextureAlphaMod(_texture, 255);
        SDL_FreeSurface(surface);
    }
}

SDL_Renderer* Texture::getRenderer() { return _renderer; };
SDL_Texture* Texture::getTexture() { return _texture; };

void Texture::update() {};
