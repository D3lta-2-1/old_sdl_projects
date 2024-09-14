#include "App.hpp"

void App::renderBoard()
{
	bool isBlack = false;
	constexpr RGBcolor blackCase{ 181, 136, 99 };
	constexpr RGBcolor whiteCase{ 240, 217, 181 };
	for (auto y = 0; y < _board.size<1>(); y++)
	{
		for (auto x = 0; x < _board.size<0>(); x++)
		{
			RGBcolor color = isBlack ? blackCase : whiteCase;
			SDL_SetRenderDrawColor(_renderer, color.R, color.G, color.B, 255);
			SDL_Rect rect;
			rect.x = x * tileSize;
			rect.y = y * tileSize;
			rect.w = tileSize;
			rect.h = tileSize;
			isBlack = !isBlack;

			SDL_RenderFillRect(_renderer, &rect);
		}
		isBlack = !isBlack;
	}
}

void App::renderPawns()
{
	for (auto y = 0; y < _board.size<1>(); y++)
	{
		for (auto x = 0; x < _board.size<0>(); x++)
		{
			if (_board(x, y).type != pawnType::nothing)
			{
				auto sRect = getPawnDrawRect(_board(x, y));
				SDL_Rect dRect{};
				dRect.x = x * tileSize;
				dRect.y = y * tileSize;
				dRect.w = tileSize;
				dRect.h = tileSize;
				SDL_RenderCopy(_renderer, _texture, &sRect, &dRect);
			}
		}
	}
}

void App::renderPlayableMove()
{
	if (_selection.has_value())
	{
		SDL_Rect dRect{};
		dRect.x = _selection.value().x * tileSize; // 0x1E81B0
		dRect.y = _selection.value().y * tileSize;
		dRect.w = tileSize;
		dRect.h = tileSize;
		SDL_SetRenderDrawColor(_renderer, 0x1E, 0xE81, 0xB0, 255);
		SDL_RenderDrawRect(_renderer, &dRect);
	}
	for (auto y = 0; y < _isPlayableMove.size<1>(); y++)
	{
		for (auto x = 0; x < _isPlayableMove.size<0>(); x++)
		{
			if (_isPlayableMove(x, y))
			{
				SDL_Rect dRect{};
				dRect.x = x * tileSize;
				dRect.y = y * tileSize;
				dRect.w = tileSize;
				dRect.h = tileSize;
				SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 255);
				SDL_RenderDrawRect(_renderer, &dRect);

				filledCircleColor(_renderer, x * tileSize, y * tileSize, tileSize / 5, 0x1E81B0);
			}
		}
	}
}

void App::gameRender()
{
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
	SDL_RenderClear(_renderer);
	renderBoard();
	renderPawns();
	renderPlayableMove();
	SDL_RenderPresent(_renderer);
}

void App::leftClicAction()
{
	vec2<int> pos{};
	pos.x = (_mouse.getPostion().x / tileSize);
	pos.y = (_mouse.getPostion().y / tileSize);
	auto& boardCase = _board(pos.x, pos.y);
	auto& isPlayable = _isPlayableMove(pos.x, pos.y);

	//undo the selection
	if (_selection.has_value() && !isPlayable)
	{
		_selection.reset();
		clearPlayableMove();
	}
	//generate a selction if a pawn is selectied
	if (!_selection.has_value() && boardCase.type != pawnType::nothing && boardCase.color == _turn)
	{
		_selection.emplace(pos);
		//compute possible moves
		computePlayableMove(_isPlayableMove, _board, _selection.value());
	}
	//move to a selection if possible
	if (isPlayable)
	{
		boardCase = _board(_selection.value().x, _selection.value().y); //move and replace the pawn 
		_board(_selection.value().x, _selection.value().y).type = pawnType::nothing; // clear the old pawn's case
		
		_turn = (_turn == pawnColor::white) ? pawnColor::black : pawnColor::white;
		_selection.reset();
		clearPlayableMove();
	}
}

void App::clearPlayableMove()
{
	for (auto y = 0u; y < _isPlayableMove.size<1>(); y++)
	{
		for (auto x = 0u; x < _isPlayableMove.size<0>(); x++)
		{
			_isPlayableMove(x, y) = false;
		}
	}
}

void App::initialize()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	//initialize graphics components
	_window = SDL_CreateWindow("chess game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowX, windowY, 0);
	if (!_window) { throw std::runtime_error(SDL_GetError()); }

	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!_renderer) { throw std::runtime_error(SDL_GetError()); }

	_texture = IMG_LoadTexture(_renderer, TEXTURE);
	if (!_renderer) { throw std::runtime_error(SDL_GetError()); }

	SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	//setup left Clic-Action
	//_mouse.onLeftPressed( std::bind(&App::leftClicAction, this));
	_mouse.onLeftPressed([this]() { leftClicAction();  });
}

void App::gameUpdate(std::chrono::duration<double, std::micro> deltaTime)
{
	_mouse.update();
}

void App::run()
{
	std::chrono::duration<double, std::micro>  deltaTime{};
	bool exit = false;
	SDL_Event event;
	while (!exit)
	{
		auto begining = std::chrono::high_resolution_clock::now();
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_WINDOWEVENT)
				switch (event.window.event)
				{
				case SDL_WINDOWEVENT_CLOSE:
					exit = true;
					break;
				default:
					break;
				}
		}
		gameUpdate(deltaTime);
		gameRender();
		auto end = std::chrono::high_resolution_clock::now();
		deltaTime = end - begining;
	}
}

void App::cleanup()
{
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_DestroyTexture(_texture);
}
