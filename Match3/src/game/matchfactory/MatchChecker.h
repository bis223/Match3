#pragma once
#include "MatchFactory.h"

class MatchChecker
{
public:
	MatchChecker(MatchFactory* matchFactory);
	~MatchChecker();
	std::vector<Match*> GetMatches(std::unordered_set<Cell*>& dirtyCells, std::unordered_set<Cell*>& matchedCells);
	typedef std::unordered_set<Cell*> Cells;
	typedef std::vector<IMatchChecker*> IMatchCheckers;

private:
	MatchFactory* m_MatchFactory;
};
