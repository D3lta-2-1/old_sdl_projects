#pragma once 
#include <SDL.h>
#include "mdarray.hpp"
#include <optional>
constexpr auto TEXTURE = "Chess.png";

template<class T>
struct vec2
{
	T x {};
	T y {};
};

enum class pawnType
{
	nothing,
	king,
	queen,
	rook,
	bishop,
	knight,
	pawn
};

enum class pawnColor
{
	white,
	black
};

struct pawn
{
	pawnType type{};
	pawnColor color{};
};

inline SDL_Rect getPawnDrawRect(pawn wPawn)
{
	SDL_Rect rect{};
	rect.x = 0;
	rect.y = 0;
	rect.w = 200;
	rect.h = 200;
	switch (wPawn.type)
	{
	case pawnType::king:
		rect.x = 0 * 200;
		break;
	case pawnType::queen:
		rect.x = 1 * 200;
		break;
	case pawnType::bishop:
		rect.x = 2 * 200;
		break;
	case pawnType::knight:
		rect.x = 3 * 200;
		break;
	case pawnType::rook:
		rect.x = 4 * 200;
		break;
	case pawnType::pawn:
		rect.x = 5 * 200;
		break;
	default:
		break;
	}
	switch (wPawn.color)
	{
	case pawnColor::white:
		rect.y = 0;
		break;
	case pawnColor::black:
		rect.y = 200;
		break;
	default:
		break;
	}
	return rect;
}

inline mdarray<pawn, 8, 8> newBoard()
{
	mdarray<pawn, 8, 8> board{ pawnType::nothing };
	for (auto y = 0; y < 2; y++)
	{
		for (auto x = 0; x < board.size<0>(); x++)
		{
			board(x, y).color = pawnColor::black;
			board(x, y).type = pawnType::pawn;
			board(x, y + 6).color = pawnColor::white;
			board(x, y + 6).type = pawnType::pawn;
		}
	}

	board(0, 0).type = pawnType::rook;
	board(0, 7).type = pawnType::rook;
	board(7, 0).type = pawnType::rook;
	board(7, 7).type = pawnType::rook;

	board(1, 0).type = pawnType::knight;
	board(1, 7).type = pawnType::knight;
	board(6, 0).type = pawnType::knight;
	board(6, 7).type = pawnType::knight;

	board(2, 0).type = pawnType::bishop;
	board(2, 7).type = pawnType::bishop;
	board(5, 0).type = pawnType::bishop;
	board(5, 7).type = pawnType::bishop;

	board(3, 0).type = pawnType::queen;
	board(3, 7).type = pawnType::queen;
	board(4, 0).type = pawnType::king;
	board(4, 7).type = pawnType::king;

	return board;
}

struct RGBcolor
{
	Uint8 R = 0;
	Uint8 G = 0;
	Uint8 B = 0;
};

template<std::size_t s1, std::size_t s2>
inline std::optional<pawn> protectedAccess(mdarray<pawn, s1, s2> const& board, int x, int y)
{
	if (x >= 0 &&
		x < board.size<0>() &&
		y >= 0 &&
		y < board.size<1>()) {
		return  board(x, y);
	}
	return{};
}

void computePlayableMove(mdarray<bool, 8, 8>& playableGrid, mdarray<pawn, 8, 8> const& board, vec2<int> selection);
