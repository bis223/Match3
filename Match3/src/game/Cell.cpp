#include <algorithm>
#include <iostream>

#include "Cell.h"
#include "Piece.h"

Cell::Cell(int row, int column, bool isSpawner)
	: m_Row(row), m_Column(column), m_IsSpawner(isSpawner), m_Piece(0), m_CurrentCommand(0)//, m_Neighbours(0)
{
}

Cell::~Cell()
{
}

void Cell::Update(float deltaTime)
{

}

void Cell::Render() const
{

}

bool Cell::IsNeighour(Cell* neighbour) const
{
	
	//if (std::find(m_Neighbours.begin(), m_Neighbours.end(), neighbour) != m_Neighbours.end())
	if(	   m_Neighbours.left == neighbour
		|| m_Neighbours.right == neighbour
		|| m_Neighbours.up == neighbour
		|| m_Neighbours.down == neighbour)
	{
		return true;
	}
	return false;
}

Cell* Cell::GetNeighbour(const Constants::Direction dir)
{
	switch (dir)
	{
	case Constants::Direction::LEFT:
		return m_Neighbours.left;
	case Constants::Direction::RIGHT:
		return m_Neighbours.right;
	case Constants::Direction::UP:
		return m_Neighbours.up;
	case Constants::Direction::DOWN:
		return m_Neighbours.down;
	default:
		return nullptr;
	}
}

void Cell::SetNeighbours(Cell* left, Cell* right, Cell* up, Cell* down)
{
	m_Neighbours = Neighbours{ left, right, up, down };
}

void Cell::AttachPiece(Piece* piece)
{
	m_Piece = piece;
	piece->SetOwner(this);
}

void Cell::DetachPiece()
{
	if(m_Piece)
		m_Piece->SetOwner(nullptr);
	m_Piece = nullptr;
}

bool Cell::CompareCells(Cell* neighbourCell, std::unordered_set<Cell*>& resolvedMatchCells) const
{
	const bool isInside = resolvedMatchCells.find(neighbourCell) != resolvedMatchCells.end();

	bool is_true = neighbourCell && neighbourCell->HasMatchingPiece(m_Piece) && (resolvedMatchCells.empty() || !isInside && neighbourCell->IsMatchable());

	return is_true;
}

bool Cell::HasMatchingPiece(Piece* p) const
{
	return m_Piece && p && m_Piece->IsMatching(p);
}

void Cell::PrepareForCommand(ICommand *command)
{
	m_PendingCellCommands.push(command);
}

bool Cell::IsReadyForCommand(ICommand * command)
{
	bool ready = m_CurrentCommand == nullptr || m_CurrentCommand->IsComplete();
	if (ready)
	{
		ready &= !m_PendingCellCommands.empty() && m_PendingCellCommands.front() == command;
	}
	return ready;
}

void Cell::BeginCommand(ICommand * command)
{
	ICommand* nextCommand = m_PendingCellCommands.front();

	bool expectedCommand = nextCommand == command;
	ASSERT(expectedCommand);
	m_CurrentCommand = command;
	m_PendingCellCommands.pop();
}

bool Cell::IsBusy()
{
	if (m_CurrentCommand && !m_CurrentCommand->IsComplete())
		return true;

	if (!m_PendingCellCommands.empty())
		return true;
	return false;
}

void Cell::ReleaseCommand(ICommand * command)
{
	bool expectedCommand = m_CurrentCommand == command;
	if(!expectedCommand)
		ASSERT(expectedCommand);
	m_CurrentCommand = nullptr;
}


