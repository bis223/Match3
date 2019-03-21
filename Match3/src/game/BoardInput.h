#pragma once
#include "glm/glm.hpp"

#include <list>

class Cell;
class MouseInput;

class BoardInput
{
public:
	BoardInput(const std::list<Cell*>& cells);
	~BoardInput();

	void Update();


private:
	Cell* GetTouchedCell(const double x, const double y);
	Cell* NeighbourAtDirection(glm::vec2 swapBeginPos, glm::vec2 swapEndPos);

	std::list<Cell*>	m_Cells;
	float				m_TouchMovement;
	//glm::vec2			m_CurrentTouchPos;
	Cell*				m_CurrentTouchCell;
	Cell*				m_LastTouchedCell;
	bool				m_SwapTriggered;
	bool				m_FirstTouch;
	bool				m_MouseHeld;
	glm::vec2			m_MouseDownPos;
	MouseInput* m_MouseInput;
	//float				m_SwapDist;
};
