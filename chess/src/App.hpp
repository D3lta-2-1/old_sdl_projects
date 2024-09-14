#pragma once
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#include <stdexcept>
#include <iostream>
#include <chrono>
#include <optional>
#include <functional>
#include "gameObjects.hpp"
#include "mouse.hpp"

constexpr Uint32 tileSize = 60;
constexpr Uint32 windowX = 8 * tileSize;
constexpr Uint32 windowY = 8 * tileSize;

class App
{
	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;
	SDL_Texture* _texture = nullptr;
	mdarray<pawn,8, 8> _board = newBoard();
	mouseHandler _mouse{};
	mdarray<bool, 8, 8> _isPlayableMove{ false };
	std::optional<vec2<int>> _selection{};
	pawnColor _turn{ pawnColor::white };

	void renderBoard();
	void renderPawns();
	void renderPlayableMove();
	void gameUpdate(std::chrono::duration<double, std::micro>  deltaTime);
	void gameRender();
	void leftClicAction();
	void clearPlayableMove();
	
public:
	
	void initialize();
	void run();
	void cleanup();
};
