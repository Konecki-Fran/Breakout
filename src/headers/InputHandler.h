#pragma once

#include "SDL2/SDL.h"
#undef main

#include "InputKey.h"

#include <iostream>
#include <vector>

class InputHandler 
{
public:
	InputHandler() = default;
	virtual ~InputHandler() = default;

	static void update();
	static InputKey getKeyPressed();

private:
	static InputKey _keyPressed;
};