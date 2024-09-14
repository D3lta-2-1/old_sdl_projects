#include "mouse.hpp"

void mouseHandler::onRightPressed(action w_action)
{
	_onRightPressed = w_action;
}

void mouseHandler::onLeftPressed(action w_action)
{
	_onLeftPressed = w_action;
}

vec2<int> mouseHandler::getPostion()
{
	return _postion;
}

void mouseHandler::update()
{
	SDL_PumpEvents();
	auto buttons = SDL_GetMouseState(&_postion.x, &_postion.y);

	if (buttons & SDL_BUTTON_LMASK)
	{
		if (!_isLeftPressed)
		{
			_isLeftPressed = true;
			_onLeftPressed();
		}
	}
	else _isLeftPressed = false;

	if (buttons & SDL_BUTTON_RMASK)
	{
		if (!_isRightPressed)
		{
			_isRightPressed = true;
			_onRightPressed();
		}
	}
	else _isRightPressed = false;
}
