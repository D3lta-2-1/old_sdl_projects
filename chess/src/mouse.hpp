#pragma once
#include <SDL.h>
#include <functional>
#include "gameObjects.hpp"

using action = std::function<void()>;
class mouseHandler
{
	bool _isLeftPressed = false;
	bool _isRightPressed = false;
	action _onRightPressed{ [](){} };
	action _onLeftPressed{ [](){} };
	vec2<int> _postion{};

public:
	void onRightPressed(action w_action);
	void onLeftPressed(action w_action);
	vec2<int> getPostion();
	void update();

};