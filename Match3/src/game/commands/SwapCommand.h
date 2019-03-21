#pragma once

#include "glm/glm.hpp"

#include "ICommand.h"

#include "Cell.h"
#include "Piece.h"

class SwapCommand: public ICommand
{
public:
	SwapCommand(Cell* cellA, Cell* cellB);
	~SwapCommand();

	virtual void Prepare() override;
	virtual bool IsReady() override;
	virtual void Execute() override;
	virtual bool IsActive() override;
	virtual void Update(float deltaTime) override;
	virtual bool IsComplete() override;
	virtual void Finish() override;

private:


	Cell* m_CellA;
	Cell* m_CellB;
	Piece* m_PieceA;
	Piece* m_PieceB;
	float m_Timer;
	float m_SwapDuration;
};


