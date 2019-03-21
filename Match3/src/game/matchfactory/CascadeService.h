#pragma once

#include <unordered_set>
#include <list>
class Cell;

class CascadeService
{
public:
	//CascadeService();
	//~CascadeService();

	typedef std::unordered_set<Cell*> Cells;

	inline const std::unordered_set<Cell*>& GetDirtyCells()	const	{ return m_DirtyCells; }
	inline const std::unordered_set<Cell*>& GetEmptyCells()	const	{ return m_EmptyCells; }
	inline void ClearAllDirtyCells()								{ m_DirtyCells.clear(); }

	void SetDirty(Cell* cell);
	void SetDirty(std::list<Cell*> cells);

	void SetEmpty(Cell* cell);
	void SetEmpty(std::list<Cell*> cells);

	void SetFilled(Cell* cell);
	void SetFilled(std::list<Cell*> cells);


	bool NeedsMatchDetection();
	bool NeedsCascading();

private:
	Cells m_DirtyCells;
	Cells m_EmptyCells;
};
