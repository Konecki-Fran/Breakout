#include "InputHandler.h"

void InputHandler::update()
{
    using enum InputKey;

    SDL_Event e;
    SDL_PollEvent(&e);

    _keyPressed = NONE;

    if (e.type == SDL_QUIT) _keyPressed = QUIT;

    if (e.type == SDL_KEYDOWN) {
        auto keyPressed = e.key.keysym.sym;

        if (keyPressed == SDLK_ESCAPE) _keyPressed = QUIT;
        else if (keyPressed == SDLK_LEFT) _keyPressed = LEFT;
        else if (keyPressed == SDLK_RIGHT) _keyPressed = RIGHT;
        else if (keyPressed == SDLK_n) _keyPressed = NEXT;
    }
}

InputKey InputHandler::getKeyPressed()
{
    return _keyPressed;
}




