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
	//std::unordered_set<Cell*> resolvedCells = matchedCells;

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
					//std::cout << "erase cell name" << *cellIt << std::endl;
					cellIt = dirtyCells.erase(cellIt);
				}
				else
				{
					Match* newMatch = (*iMatchChecketIt)->Check(*cellIt, resolvedCells);
					if (newMatch)
					{
						//std::cout << count << " Match type " << count << std::endl;
						newMatches.push_back(newMatch);
						resolvedCells.insert(newMatch->GetMatchedCells().begin(), newMatch->GetMatchedCells().end());
						for (std::list<Cell*>::const_iterator celIt = resolvedCells.begin(),
							end = resolvedCells.end();
							celIt != end;
							++celIt)
						{
							//std::cout << count << " resolved cell name " << *celIt << std::endl;

						}

						std::list<Cell*> matchedCells = newMatch->GetMatchedCells();
						for (std::list<Cell*>::const_iterator celIt = matchedCells.begin(),
							end = matchedCells.end();
							celIt != end;
							++celIt)
						{
							//std::cout << count << " cell name " << *celIt << std::endl;

						}
//						count++;
					}
					++cellIt;
				}
			}
		}

	}

	/*for (Cells::const_iterator cellsIt = dirtyCells.begin(),
		end = dirtyCells.end();
		cellsIt != end;
		++cellsIt)
	{
		if (std::find(resolvedCells.begin(), resolvedCells.end(), *cellsIt) != resolvedCells.end())
		{
			continue;
		}
		else
		{
			int count = 0;
			IMatchCheckers matchCheckers = m_MatchFactory->GetMatchCheckers();
			for (IMatchCheckers::const_iterator iMatchChecketIt = matchCheckers.begin(),
				end = matchCheckers.end();
				iMatchChecketIt != end;
				++iMatchChecketIt)
			{
				std::cout << count << " before " << resolvedCells.size() << std::endl;
				Match* newMatch = (*iMatchChecketIt)->Check(*cellsIt, resolvedCells);
				if (newMatch)
				{
					newMatches.push_back(newMatch);
					resolvedCells.insert(newMatch->GetMatchedCells().begin(), newMatch->GetMatchedCells().end());

				}
				std::cout << count << " after " << resolvedCells.size() << std::endl;
				count++;
			}
		}

	}*/

	return newMatches;
}