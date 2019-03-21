#include "SwapCommand.h"
#include "Utility.h"


SwapCommand::SwapCommand(Cell* cellA, Cell* cellB): m_CellA(cellA), m_CellB(cellB), m_PieceA(0), m_PieceB(0), m_Timer(0.0f), m_SwapDuration(0.35f)
{
	m_PieceA = m_CellA->GetPiece();
	m_PieceB = m_CellB->GetPiece();
	Prepare();
}

SwapCommand::~SwapCommand()
{
}

void SwapCommand::Prepare()
{
	m_CellA->PrepareForCommand(this);
	m_CellB->PrepareForCommand(this);
}

bool SwapCommand::IsReady()
{
	return m_CellA->IsReadyForCommand(this) && m_CellB->IsReadyForCommand(this);
}

void SwapCommand::Execute()
{
	m_IsActive = true;
	m_CellA->BeginCommand(this);
	m_CellB->BeginCommand(this);
}

bool SwapCommand::IsActive()
{
	return m_IsActive;
}



void SwapCommand::Update(float deltaTime)
{
	if (m_IsActive && m_PieceA && m_PieceB)
	{
		m_Timer += deltaTime;
		if (m_Timer >= m_SwapDuration) // || pieceA == null || pieceB == null)
		{
			Finish();
		}
		else
		{
			m_PieceA->SetPosition(Utility::InterPolate(m_CellA->GetPosition(), m_CellB->GetPosition(), m_Timer / m_SwapDuration));
			m_PieceB->SetPosition(Utility::InterPolate(m_CellB->GetPosition(), m_CellA->GetPosition(), m_Timer / m_SwapDuration));
		}


	}
}


bool SwapCommand::IsComplete()
{
	return m_IsComplete;
}

void SwapCommand::Finish()
{
	m_PieceA->SetPosition(m_CellB->GetPosition());
	m_PieceB->SetPosition(m_CellA->GetPosition());

	m_IsActive = false;
	m_IsComplete = true;

	m_CellA->ReleaseCommand(this);
	m_CellB->ReleaseCommand(this);
}