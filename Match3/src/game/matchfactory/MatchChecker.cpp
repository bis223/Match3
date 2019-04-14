#include "IMatchChecker.h"
#include "MatchChecker.h"
#include <iostream>
MatchChecker::MatchChecker(MatchFactory* matchFactory): m_MatchFactory(matchFactory)
{

}

MatchChecker::~MatchChecker()
{
}

std::vector<Match*> MatchChecker::GetMatches(std::unordered_set<Cell*>& dirtyCells, std::unordered_set<Cell*>& resolvedCells)
{
	std::vector<Match*> newMatches;

	int count = 0;
	IMatchCheckers matchCheckers = m_MatchFactory->GetMatchCheckers();
	for (IMatchCheckers::const_iterator iMatchChecketIt = matchCheckers.begin(),
		end = matchCheckers.end();
		iMatchChecketIt != end;
		++iMatchChecketIt)
	{
		if (!dirtyCells.empty())
		{
			Cells::const_iterator cellIt = dirtyCells.begin();

			while (cellIt != dirtyCells.end())
			{
				if (std::find(resolvedCells.begin(), resolvedCells.end(), *cellIt) != resolvedCells.end())
				{
					cellIt = dirtyCells.erase(cellIt);
				}
				else
				{
					Match* newMatch = (*iMatchChecketIt)->Check(*cellIt, resolvedCells);
					if (newMatch)
					{
						newMatches.push_back(newMatch);
						resolvedCells.insert(newMatch->GetMatchedCells().begin(), newMatch->GetMatchedCells().end());

						std::list<Cell*> matchedCells = newMatch->GetMatchedCells();
					}
					++cellIt;
				}
			}
		}

	}

	return newMatches;
}