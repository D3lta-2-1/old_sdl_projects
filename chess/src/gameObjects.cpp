#include "gameObjects.hpp"
#include <array>
#include <functional>

void pawnProceed(pawn concurentPawn, mdarray<bool, 8, 8>& playableGrid, mdarray<pawn, 8, 8> const& board, vec2<int> selection)
{
	//white pawn
	if (concurentPawn.color == pawnColor::white)
	{
		//forward move
		auto front = protectedAccess(board, selection.x, selection.y - 1);
		if (front.has_value())
		{
			if (front.value().type == pawnType::nothing)
			{
				playableGrid(selection.x, selection.y - 1) = true;
				//beginning two case move
				if (selection.y == 6)
				{
					auto front = protectedAccess(board, selection.x, selection.y - 2);
					if (front.has_value())
					{
						if (front.value().type == pawnType::nothing)
						{
							playableGrid(selection.x, selection.y - 2) = true;
						}
					}
				}
			}
		}
		//eating
		auto leftEat = protectedAccess(board, selection.x - 1, selection.y - 1);
		if (leftEat.has_value())
		{
			if (leftEat.value().type != pawnType::nothing && leftEat.value().color == pawnColor::black)
			{
				playableGrid(selection.x - 1, selection.y - 1) = true;
			}
		}
		auto rightEat = protectedAccess(board, selection.x + 1, selection.y - 1);
		if (rightEat.has_value())
		{
			if (rightEat.value().type != pawnType::nothing && rightEat.value().color == pawnColor::black)
			{
				playableGrid(selection.x + 1, selection.y - 1) = true;
			}
		}
		
	}
	//black pawn
	if (concurentPawn.color == pawnColor::black)
	{
		//forward move
		auto front = protectedAccess(board, selection.x, selection.y + 1);
		if (front.has_value())
		{
			if (front.value().type == pawnType::nothing)
			{
				playableGrid(selection.x, selection.y + 1) = true;
				//beginning two case move
				if (selection.y == 1)
				{
					auto front = protectedAccess(board, selection.x, selection.y + 2);
					if (front.has_value())
					{
						if (front.value().type == pawnType::nothing)
						{
							playableGrid(selection.x, selection.y + 2) = true;
						}
					}
				}
			}
		}
		//eating
		auto leftEat = protectedAccess(board, selection.x - 1, selection.y + 1);
		if (leftEat.has_value())
		{
			if (leftEat.value().type != pawnType::nothing && leftEat.value().color == pawnColor::white)
			{
				playableGrid(selection.x - 1, selection.y + 1) = true;
			}
		}
		auto rightEat = protectedAccess(board, selection.x + 1, selection.y + 1);
		if (rightEat.has_value())
		{
			if (rightEat.value().type != pawnType::nothing && rightEat.value().color == pawnColor::white)
			{
				playableGrid(selection.x + 1, selection.y + 1) = true;
			}
		}
	}
}

void knightProceed(pawn concurentPawn, mdarray<bool, 8, 8>& playableGrid, mdarray<pawn, 8, 8> const& board, vec2<int> selection)
{
	std::array<vec2<int>, 8> possibleCase{ {
		{ -2, -1 },
		{  2, -1 },
		{  1, -2 },
		{ -1, -2 },
		{ -2,  1 },
		{  2,  1 },
		{  1,  2 },
		{ -1,  2 } } };
	for (auto i: possibleCase)
	{
		auto protCase = protectedAccess(board, selection.x + i.x, selection.y + i.y );
		if(protCase.has_value() && ( protCase.value().color != concurentPawn.color || protCase.value().type == pawnType::nothing)) playableGrid(selection.x + i.x, selection.y + i.y) = true;
	}
}

void kingProceed(pawn concurentPawn, mdarray<bool, 8, 8>& playableGrid, mdarray<pawn, 8, 8> const& board, vec2<int> selection)
{
	std::array<vec2<int>, 8> possibleCase{ {
		{ -1, -1 },
		{  1, -1 },
		{  0, -1 },
		{ -1,  1 },
		{  1,  1 },
		{  0,  1 },
		{  1,  0 },
		{ -1,  0 } } };
	for (auto i : possibleCase)
	{
		auto protCase = protectedAccess(board, selection.x + i.x, selection.y + i.y);
		if (protCase.has_value() && (protCase.value().color != concurentPawn.color || protCase.value().type == pawnType::nothing)) playableGrid(selection.x + i.x, selection.y + i.y) = true;
	}
}

void rookProceed(pawn concurentPawn, mdarray<bool, 8, 8>& playableGrid, mdarray<pawn, 8, 8> const& board, vec2<int> selection)
{
	vec2<int> askPos{ 0, 0 };
	std::optional<pawn> askCase{};
	bool isPlayable;
	std::array<std::function<void(void)>, 4> checkMoves { [&askPos]() { askPos.y--; },
															[&askPos]() { askPos.y++; },
															[&askPos]() { askPos.x--; },
															[&askPos]() { askPos.x++; }  };
	for (auto i : checkMoves)
	{
		askPos = { 0, 0 };
		do
		{
			i();
			askCase = protectedAccess(board, selection.x + askPos.x, selection.y + askPos.y);
			isPlayable = askCase.has_value() && (askCase.value().color != concurentPawn.color || askCase.value().type == pawnType::nothing);
			if (isPlayable)
			{
				playableGrid(selection.x + askPos.x, selection.y + askPos.y) = true;
				if (askCase.value().type != pawnType::nothing) isPlayable = false;
			}
		} while (isPlayable);
	}
}

void bishopProceed(pawn concurentPawn, mdarray<bool, 8, 8>& playableGrid, mdarray<pawn, 8, 8> const& board, vec2<int> selection)
{
	vec2<int> askPos{ 0, 0 };
	std::optional<pawn> askCase{};
	bool isPlayable;
	std::array<std::function<void(void)>, 4> checkMoves{ [&askPos]() { askPos.y--; askPos.x--; },
															[&askPos]() { askPos.y++; askPos.x--; },
															[&askPos]() { askPos.y--; askPos.x++; },
															[&askPos]() {  askPos.y++; askPos.x++; } };
	for (auto i : checkMoves)
	{
		askPos = { 0, 0 };
		do
		{
			i();
			askCase = protectedAccess(board, selection.x + askPos.x, selection.y + askPos.y);
			isPlayable = askCase.has_value() && (askCase.value().color != concurentPawn.color || askCase.value().type == pawnType::nothing);
			if (isPlayable)
			{
				playableGrid(selection.x + askPos.x, selection.y + askPos.y) = true;
				if (askCase.value().type != pawnType::nothing) isPlayable = false;
			}
		} while (isPlayable);
	}
}


void computePlayableMove(mdarray<bool, 8, 8>& playableGrid, mdarray<pawn, 8, 8> const& board, vec2<int> selection)
{
	auto concurentPawn = board(selection.x, selection.y);
	switch (concurentPawn.type)
	{
		case pawnType::pawn:
			pawnProceed(concurentPawn, playableGrid, board, selection);
			break;
		case pawnType::knight:
			knightProceed(concurentPawn, playableGrid, board, selection);
			break;
		case pawnType::king:
			kingProceed(concurentPawn, playableGrid, board, selection);
			break;
		case pawnType::rook:
			rookProceed(concurentPawn, playableGrid, board, selection);
			break;
		case pawnType::bishop:
			bishopProceed(concurentPawn, playableGrid, board, selection);
			break;
		case pawnType::queen:
			rookProceed(concurentPawn, playableGrid, board, selection);
			bishopProceed(concurentPawn, playableGrid, board, selection);
		default:
			break;
	}
	
}
