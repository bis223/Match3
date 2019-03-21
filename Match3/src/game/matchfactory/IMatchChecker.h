#pragma once

#include <unordered_set>

class Cell;
class Match;


class IMatchChecker
{
public:
	virtual ~IMatchChecker() {}
	virtual Match* Check(Cell* cell, std::unordered_set<Cell*>& resolvedCells) = 0;
};
