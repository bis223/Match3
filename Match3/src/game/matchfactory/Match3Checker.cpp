#include "Match3Checker.h"
#include "Cell.h"
#include "Match.h"
#include <vector>
#include <iostream>

Match3Checker::Match3Checker(int baseScore, int scorePerPiece): m_BaseScore(baseScore), m_ScorePerPiece(scorePerPiece)
{
}

Match* Match3Checker::Check(Cell* cell, std::unordered_set<Cell*>& resolvedCells)
{

	bool leftMatch = cell->CompareCells(cell->GetLeft(), resolvedCells);
	bool rightMatch = cell->CompareCells(cell->GetRight(), resolvedCells);


	if (leftMatch)
	{
		if (rightMatch)
		{
			std::list<Cell*> matchedCells = std::list<Cell*>{ cell->GetLeft(), cell, cell->GetRight()};

			return new Match(Match::MATCH3, matchedCells, cell, GetScore(matchedCells.size()));
		}
		else if (cell->CompareCells(cell->GetLeft()->GetLeft(), resolvedCells))
		{
			std::list<Cell*> matchedCells = std::list<Cell*>{ cell->GetLeft()->GetLeft(), cell->GetLeft(), cell };

			return new Match(Match::MATCH3, matchedCells, cell, GetScore(matchedCells.size()));
		}
	}

	if (rightMatch)
	{
		if (cell->CompareCells(cell->GetRight()->GetRight(), resolvedCells))
		{

			std::list<Cell*> matchedCells = std::list<Cell*>{ cell, cell->GetRight(), cell->GetRight()->GetRight() };

			return new Match(Match::MATCH3, matchedCells, cell, GetScore(matchedCells.size()));
		}
	}

	bool upMatch = cell->CompareCells(cell->GetUp(), resolvedCells);
	bool downMatch = cell->CompareCells(cell->GetDown(), resolvedCells);

	if (upMatch)
	{
		if (downMatch)
		{
			std::list<Cell*> matchedCells = std::list<Cell*>{ cell->GetUp(), cell, cell->GetDown() };

			return new Match(Match::MATCH3, matchedCells, cell, GetScore(matchedCells.size()));
		}
		else if (cell->CompareCells(cell->GetUp()->GetUp(), resolvedCells))
		{
			std::list<Cell*> matchedCells = std::list<Cell*>{ cell->GetUp()->GetUp(), cell->GetUp(), cell};

			return new Match(Match::MATCH3, matchedCells, cell, GetScore(matchedCells.size()));
		}
	}

	if (downMatch)
	{
		if (cell->CompareCells(cell->GetDown()->GetDown(), resolvedCells))
		{
			std::list<Cell*> matchedCells = std::list<Cell*>{ cell , cell->GetDown(), cell->GetDown()->GetDown() };

			return new Match(Match::MATCH3, matchedCells, cell, GetScore(matchedCells.size()));
		}
	}


	return nullptr;
}

int Match3Checker::GetScore(int totalPiecesDestroyed)
{
	return m_BaseScore + m_ScorePerPiece * totalPiecesDestroyed;
}
