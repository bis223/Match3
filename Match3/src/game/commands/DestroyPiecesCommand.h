#pragma once
#include "glm/glm.hpp"
#include "ICommand.h"
#include <list>

class Piece;
class Cell;

class DestroyPiecesCommand : public ICommand
{
public:
	typedef std::list<Cell*> Cells;
	typedef std::list<Piece*> Pieces;
	DestroyPiecesCommand(const Cells& destroyingCells, const Pieces& destroyingPieces, const float duration);
	~DestroyPiecesCommand();

	virtual void Prepare() override;
	virtual bool IsReady() override;
	virtual void Execute() override;
	virtual bool IsActive() override;
	virtual void Update(float deltaTime) override;
	virtual bool IsComplete() override;
	virtual void Finish() override;

private:
	Pieces m_DestroyingPieces;
	Cells m_DestroyingCells;
	float m_Duration;
	float m_Timer;
	float m_PieceScale;
	int m_highestRow;
};
