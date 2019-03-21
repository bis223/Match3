#include "Match.h"

Match::Match(Match::MatchType matchType, std::list<Cell*>& matchedCells, Cell * keyCell, int score)
	: m_MatchType(matchType), m_MatchedCells(matchedCells), m_KeyCell(keyCell), m_MatchScore(score)
{

}

Match::~Match()
{
}
