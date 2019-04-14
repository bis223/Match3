#include "CascadeService.h"
#include "Cell.h"

//CascadeService::CascadeService()
//{
//}
//
//CascadeService::~CascadeService()
//{
//}

void CascadeService::SetDirty(Cell * cell)
{
	m_DirtyCells.insert(cell);
}

void CascadeService::SetDirty(std::list<Cell*> cells)
{
	m_DirtyCells.insert(cells.begin(), cells.end());
}

void CascadeService::SetEmpty(Cell * cell)
{
	m_EmptyCells.insert(cell);
}

void CascadeService::SetEmpty(std::list<Cell*> cells)
{
	m_EmptyCells.insert(cells.begin(), cells.end());
}

void CascadeService::SetFilled(Cell * cell)
{
	m_EmptyCells.erase(cell);
}

void CascadeService::SetFilled(std::list<Cell*> cells)
{
	m_EmptyCells.erase(cells.begin(), cells.end());
}

bool CascadeService::NeedsMatchDetection()
{
	if (!m_DirtyCells.empty())
	{
		for (const auto& cellIt : m_DirtyCells)
		{
			if (cellIt->IsBusy())
				return false;
		}
		return true;
	}
	return false;
}

bool CascadeService::NeedsCascading()
{
	if (!m_EmptyCells.empty())
	{
		for (const auto& cellIt : m_EmptyCells) 
		{
			if (cellIt->IsBusy())
				return false;
		}
		return true;
	}
	return false;
}
