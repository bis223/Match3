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
	Match* match = nullptr;

	bool leftMatch = cell->CompareCells(cell->GetLeft(), resolvedCells);
	bool rightMatch = cell->CompareCells(cell->GetRight(), resolvedCells);

	//std::vector<Cell*> matchedCells;
	//matchedCells.reserve(3);


	if (leftMatch)
	{
		if (rightMatch)
		{
			/*matchedCells.push_back(cell->GetLeft());
			matchedCells.push_back(cell);
			matchedCells.push_back(cell->GetRight());*/
			//return new Match(Match.MatchType.MATCH3, new List<Cell>() { cell.Left, cell, cell.Right }, cell);
			std::list<Cell*> matchedCells = std::list<Cell*>{ cell->GetLeft(), cell, cell->GetRight()};
			for (std::list<Cell*>::const_iterator celIt = matchedCells.begin(),
				end = matchedCells.end();
				celIt != end;
				++celIt)
			{
				std::cout <<" lr new match3 cell name" << (*celIt)->GetName() << std::endl;

			}

			return new Match(Match::MATCH3, matchedCells, cell, GetScore(matchedCells.size()));
		}
		else if (cell->CompareCells(cell->GetLeft()->GetLeft(), resolvedCells))
		{
			/*matchedCells.push_back(cell->GetLeft()->GetLeft());
			matchedCells.push_back(cell->GetLeft);
			matchedCells.push_back(cell);*/
			//return new Match(Match.MatchType.MATCH3, new List<Cell>() { cell.Left.Left, cell.Left, cell }, cell);
			std::list<Cell*> matchedCells = std::list<Cell*>{ cell->GetLeft()->GetLeft(), cell->GetLeft(), cell };
			for (std::list<Cell*>::const_iterator celIt = matchedCells.begin(),
				end = matchedCells.end();
				celIt != end;
				++celIt)
			{
				std::cout << " ll new match3 cell name" << (*celIt)->GetName() << std::endl;

			}
			return new Match(Match::MATCH3, matchedCells, cell, GetScore(matchedCells.size()));
		}
	}

	if (rightMatch)
	{
		if (cell->CompareCells(cell->GetRight()->GetRight(), resolvedCells))
		{
			/*matchedCells.push_back(cell);
			matchedCells.push_back(cell->GetRight());
			matchedCells.push_back(cell->GetRight()->GetRight());*/
			//return new Match(Match.MatchType.MATCH3, new List<Cell>() { cell, cell.Right, cell.Right.Right }, cell);
			std::list<Cell*> matchedCells = std::list<Cell*>{ cell, cell->GetRight(), cell->GetRight()->GetRight() };
			for (std::list<Cell*>::const_iterator celIt = matchedCells.begin(),
				end = matchedCells.end();
				celIt != end;
				++celIt)
			{
				std::cout << " rr new match3 cell name" << (*celIt)->GetName() << std::endl;

			}
			return new Match(Match::MATCH3, matchedCells, cell, GetScore(matchedCells.size()));
		}
	}

	bool upMatch = cell->CompareCells(cell->GetUp(), resolvedCells);
	bool downMatch = cell->CompareCells(cell->GetDown(), resolvedCells);

	if (upMatch)
	{
		if (downMatch)
		{/*
			matchedCells.push_back(cell->GetUp());
			matchedCells.push_back(cell);
			matchedCells.push_back(cell->GetDown());*/
			//return new Match(Match.MatchType.MATCH3, new List<Cell>() { cell.Up, cell, cell.Down }, cell);
			std::list<Cell*> matchedCells = std::list<Cell*>{ cell->GetUp(), cell, cell->GetDown() };
			for (std::list<Cell*>::const_iterator celIt = matchedCells.begin(),
				end = matchedCells.end();
				celIt != end;
				++celIt)
			{
				std::cout << " ud new match3 cell name" << (*celIt)->GetName() << std::endl;

			}
			return new Match(Match::MATCH3, matchedCells, cell, GetScore(matchedCells.size()));
		}
		else if (cell->CompareCells(cell->GetUp()->GetUp(), resolvedCells))
		{
			/*
			matchedCells.push_back(cell->GetUp()->GetUp());
			matchedCells.push_back(cell->GetUp());
			matchedCells.push_back(cell);*/
			//return new Match(Match.MatchType.MATCH3, new List<Cell>() { cell.Up.Up, cell.Up, cell }, cell);
			std::list<Cell*> matchedCells = std::list<Cell*>{ cell->GetUp()->GetUp(), cell->GetUp(), cell};
			for (std::list<Cell*>::const_iterator celIt = matchedCells.begin(),
				end = matchedCells.end();
				celIt != end;
				++celIt)
			{
				std::cout << " uu new match3 cell name" << (*celIt)->GetName() << std::endl;

			}
			return new Match(Match::MATCH3, matchedCells, cell, GetScore(matchedCells.size()));
		}
	}

	if (downMatch)
	{
		if (cell->CompareCells(cell->GetDown()->GetDown(), resolvedCells))
		{
			/*matchedCells.push_back(cell);
			matchedCells.push_back(cell->GetDown());
			matchedCells.push_back(cell->GetDown()->GetDown());*/
			std::list<Cell*> matchedCells = std::list<Cell*>{ cell , cell->GetDown(), cell->GetDown()->GetDown() };
			for (std::list<Cell*>::const_iterator celIt = matchedCells.begin(),
				end = matchedCells.end();
				celIt != end;
				++celIt)
			{
				std::cout << " dd new match3 cell name" << (*celIt)->GetName() << std::endl;

			}
			return new Match(Match::MATCH3, matchedCells, cell, GetScore(matchedCells.size()));
			//return new Match(Match.MatchType.MATCH3, new List<Cell>() { cell, cell.Down, cell.Down.Down }, cell);
		}
	}


	/*if (matchedCells.size() == 3)
	{
		match = new Match(Match::MATCH3, matchedCells, cell, GetScore(matchedCells.size()));
	}
	matchedCells.clear();
*/
	return match;
}

int Match3Checker::GetScore(int totalPiecesDestroyed)
{
	return m_BaseScore + m_ScorePerPiece * totalPiecesDestroyed;
}
