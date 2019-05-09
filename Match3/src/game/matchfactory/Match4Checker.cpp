#include "Match4Checker.h"
#include "Cell.h"
#include "Match.h"
#include <vector>

Match4Checker::Match4Checker(int baseScore, int scorePerPiece) : m_BaseScore(baseScore), m_ScorePerPiece(scorePerPiece)
{
}

Match* Match4Checker::Check(Cell* cell, std::unordered_set<Cell*>& resolvedCells)
{
	std::list<Cell*> matchedCells;
	Cell* pivot = cell;
	matchedCells.push_back(pivot);

	while (pivot->CompareCells(pivot->GetLeft(), resolvedCells) && matchedCells.size() < 4)
	{
		pivot = pivot->GetLeft();
		matchedCells.push_back(pivot);
	}

	pivot = cell;

	while (pivot->CompareCells(pivot->GetRight(), resolvedCells) && matchedCells.size() < 4)
	{
		pivot = pivot->GetRight();
		matchedCells.push_back(pivot);
	}

	if (matchedCells.size() >= 4)
	{
		return new Match(Match::MATCH4, matchedCells, cell, GetScore(matchedCells.size()));
	}

	matchedCells.clear();
	pivot = cell;
	matchedCells.push_back(pivot);

	while (pivot->CompareCells(pivot->GetUp(), resolvedCells) && matchedCells.size() < 4)
	{
		pivot = pivot->GetUp();
		matchedCells.push_back(pivot);
	}

	pivot = cell;

	while (pivot->CompareCells(pivot->GetDown(), resolvedCells) && matchedCells.size() < 4)
	{
		pivot = pivot->GetDown();
		matchedCells.push_back(pivot);
	}

	if (matchedCells.size() >= 4)
	{
		return new Match(Match::MATCH4, matchedCells, cell, GetScore(matchedCells.size()));
	}

	return nullptr;
}

int Match4Checker::GetScore(int totalPiecesDestroyed)
{
	return m_BaseScore + m_ScorePerPiece * totalPiecesDestroyed;
}
