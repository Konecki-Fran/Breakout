#pragma once

#include "SDL2/SDL.h"
#undef main
#include "SDL2/SDL_image.h"
#include <SDL2/SDL_ttf.h>

#include "GameObject.h"
#include "Texture.h"
#include "Helpers.h"

class Window {
public:
	explicit Window(int width, int height);
	~Window();

	void initialize();
	void renderFrame(const Vector<GameObject>& gameObjects, int score);
	void renderObject(const GameObject& gameObject);
	void destroy();

	SDL_Window* getWindow() const;
	SDL_Renderer* getRenderer() const;
	SDL_Surface* getScreenSurface() const;

	void renderScore(int score);

	int getWidth() const;
	int getHeight() const;

private:
	const int _WIDTH;
	const int _HEIGHT;

	SDL_Window* _window;
	SDL_Renderer* _renderer;
	SDL_Surface* _screenSurface;
	TTF_Font* _font;
};