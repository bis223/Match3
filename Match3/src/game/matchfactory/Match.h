#pragma once

#include "Cell.h"
#include <list>

class Match
{
public:
	enum MatchType
	{
		MATCH3 = 0,
		MATCH4 = 1,
		MATCH5 = 2,
		MATCH6 = 3,
		MATCHSQUARE = 4,
		MATCH_INTERSECTION = 5
	};
	Match(Match::MatchType matchType, std::list<Cell*>& matchedCells, Cell* keyCell, int score);
	~Match();
	inline int GetScore() const { return m_MatchScore; }
	inline const std::list<Cell*>& GetMatchedCells() const { return m_MatchedCells; }
private:
	MatchType			m_MatchType;
	std::list<Cell*>    m_MatchedCells;
	Cell*				m_KeyCell;
	int					m_MatchScore;

};
