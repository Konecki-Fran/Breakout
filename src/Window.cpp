#include "Window.h"

Window::Window(int width, int height) : _WIDTH(width), _HEIGHT(height) { initialize(); }
Window::~Window() { destroy(); }

SDL_Window* Window::getWindow() const { return _window; };
SDL_Renderer* Window::getRenderer() const { return _renderer; };
SDL_Surface* Window::getScreenSurface()const { return _screenSurface; };
int Window::getWidth() const { return _WIDTH; }
int Window::getHeight() const { return _HEIGHT; }

void Window::initialize() 
{
    _window = SDL_CreateWindow("Breakout Game",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        _WIDTH, _HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL); 

    if (!_window) {
        std::cout << "Error creating window:" << SDL_GetError() << std::endl;
        return;
    }

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    if (!_renderer) {
        std::cout << "Error creating renderer:" << SDL_GetError() << std::endl;
        return;
    }

    TTF_Init();
    _font = TTF_OpenFont("FreeSans.ttf", 24);
    if (_font == nullptr) {
        fprintf(stderr, "error: font not found\n");
        exit(EXIT_FAILURE);
    }

    _screenSurface = SDL_GetWindowSurface(_window);
    SDL_FillRect(_screenSurface, nullptr, SDL_MapRGB(_screenSurface->format, 0xff, 0xff, 0xff));
    SDL_UpdateWindowSurface(_window);
}

void Window::renderFrame(const Vector<GameObject>& objects, int score) 
{
    SDL_SetRenderTarget(_renderer, nullptr);
    SDL_SetRenderDrawColor(_renderer, 80, 80, 80, 255);
    SDL_RenderClear(_renderer);

    for (const GameObject& object : objects) {
        renderObject(object);
    }

    renderScore(score);

    SDL_RenderPresent(_renderer);
}

void Window::renderScore(int score)
{
    std::string score_text = std::format("score: {}", std::to_string(score));
    SDL_Color textColor = { 255, 255, 255, 0 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(_font, score_text.c_str(), textColor);
    SDL_Texture* text = SDL_CreateTextureFromSurface(_renderer, textSurface);
    int text_width = textSurface->w;
    int text_height = textSurface->h;
    SDL_FreeSurface(textSurface);
    SDL_Rect renderQuad = { 20, 600 - 30, text_width, text_height };
    SDL_RenderCopy(_renderer, text, nullptr, &renderQuad);
    SDL_DestroyTexture(text);
}

void Window::renderObject(const GameObject& gameObject) {
    auto textureOpt = gameObject.getComponentPtr(ComponentType::TEXTURE);
    auto transformOpt = gameObject.getComponentPtr(ComponentType::TRANSFORM);

    if (textureOpt.has_value() && transformOpt.has_value()) {
        auto textureComponent = dynamic_cast<Texture*>(textureOpt.value());
        auto* texture = textureComponent->getTexture();

        auto const* transform = dynamic_cast<Transform*>(transformOpt.value());
        int x = transform->getX();
        int y = transform->getY();
        int w = transform->getW();
        int h = transform->getH();

        SDL_Rect fromTexture = { 0, 0, 100, 100 };
        SDL_Rect onScreen = { x, y, w, h };
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(_renderer, texture, &fromTexture, &onScreen);
    }
}

void Window::destroy() 
{
	SDL_DestroyWindow(_window);
}



