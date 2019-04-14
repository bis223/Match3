#pragma once
#include "ICommand.h"

#include "Cell.h"
#include "Piece.h"

class SpawnPieceCommand : public ICommand
{
public:
	SpawnPieceCommand(Piece* droppingPiece, Cell* droppingTo, Cell* droppingFrom, const float delay = 0.0f);
	~SpawnPieceCommand();

	virtual void Prepare() override;
	virtual bool IsReady() override;
	virtual void Execute() override;
	virtual bool IsActive() override;
	virtual void Update(float deltaTime) override;
	virtual bool IsComplete() override;
	virtual void Finish() override;

private:
	Piece* m_DroppingPiece;
	Cell* m_DroppingTo;
	Cell* m_DroppingFrom;
	float m_Delay;

	float m_dropSpeed;
	float m_Timer;
};


