#include "BoardInput.h"
#include "MouseInput.h"
#include "System.h"
#include "Cell.h"
#include <iostream>

BoardInput::BoardInput(const std::list<Cell*>& cells)
	: m_Cells(cells), m_TouchMovement(0.0f), m_CurrentTouchCell(0), m_LastTouchedCell(0), m_SwapTriggered(false), m_FirstTouch(false), m_MouseDownPos(0), m_MouseHeld(false)
{
	m_MouseInput = System::GetSystemInstance().GetMouseInput();
}

BoardInput::~BoardInput()
{
}

void BoardInput::Update()
{

	//MouseInput* m_MouseInput = Application::GetApplication().GetMouseInput();

	double x, y;
	m_MouseInput->GetMousePosition(x, y);
	glm::vec2 mousePosition = glm::vec2(x, y);

	if (m_MouseInput->GetMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) && !m_MouseHeld) // This on mouse button down
	{
		m_MouseHeld = true;
		m_MouseDownPos = glm::vec2(x, y);
		Cell* touchedCell = GetTouchedCell(x,y);

		if (!touchedCell)
		{
			return;
			//m_FirstTouch = true;                  // first cell clicked
		}

		if (m_CurrentTouchCell && (m_CurrentTouchCell != touchedCell))
		{
			m_FirstTouch = false;
			if (m_CurrentTouchCell->IsNeighour(touchedCell))
			{

				//Cell* m_LastTouchedCell;
				m_LastTouchedCell = m_CurrentTouchCell;     // Second cell clicked
				m_CurrentTouchCell = touchedCell;
				m_SwapTriggered = true;
				//Board.Instance.SelectCell(null);
				//std::cout << "sECOND tUCH! " << touchedCell->GetRow() << " " << touchedCell->GetColumn() << std::endl;
			}
			else
			{
				m_FirstTouch = false;
				m_CurrentTouchCell = touchedCell;
			///	std::cout << "FIRST tUCH! " << touchedCell->GetRow() << " " << touchedCell->GetColumn() << std::endl;
				//Board.Instance.SelectCell(touchedCell);
			}
		}
		else
		{
			m_FirstTouch = true;
			m_CurrentTouchCell = touchedCell;
			//std::cout << "FIRST tUCH! " << touchedCell->GetRow() << " " << touchedCell->GetColumn() << std::endl;
			//Board.Instance.SelectCell(currentTouchCell);
		}
		//if(m_LastTouchedCell)
		//	std::cout << "m_LastTouchedCell! " << m_LastTouchedCell->GetRow() << " " << m_LastTouchedCell->GetColumn() << std::endl;
		//if (m_CurrentTouchCell)
		//	std::cout << "m_CurrentTouchCell! " << m_CurrentTouchCell->GetRow() << m_CurrentTouchCell->GetColumn() << std::endl;
		
	}
	if (m_MouseInput->GetMouseButtonUp(GLFW_MOUSE_BUTTON_LEFT))
	{
		m_MouseHeld = false;
	}
	//std::cout << "m_MouseHeld! " << m_MouseHeld << std::endl;
	if (!m_MouseHeld)
	{
		return;
	}

	if (!m_SwapTriggered && m_FirstTouch && m_CurrentTouchCell)
	{
		m_TouchMovement = glm::abs(glm::length(m_MouseDownPos - mousePosition));
		//std::cout << "m_TouchMovement " << m_TouchMovement << std::endl;
		if (m_TouchMovement >= Constants::SWAP_THRESHOLD)
		{
			m_LastTouchedCell = m_CurrentTouchCell;
			Cell* neighbourCell = nullptr;
			neighbourCell = NeighbourAtDirection(m_MouseDownPos, mousePosition);
			//m_CurrentTouchCell = CellAtDirection(m_MouseDownPos, mousePosition);  //Swipped towards second cell
			//Board.Instance.SelectCell(null);
			if (neighbourCell)
			{
				m_CurrentTouchCell = neighbourCell;
				m_SwapTriggered = true;
				m_FirstTouch = false;
			}
			else
			{
				m_CurrentTouchCell = m_LastTouchedCell;
				m_TouchMovement = 0.0f;
			}
			//std::cout << "sECOND tUCH! " << m_CurrentTouchCell->GetRow() << " " << m_CurrentTouchCell->GetColumn() << std::endl;
			//blockInput = true;
			//std::cout << "m_LastTouchedCell! " << m_LastTouchedCell->GetRow() << m_LastTouchedCell->GetColumn() << std::endl;
			//std::cout << "m_CurrentTouchCell! " << m_CurrentTouchCell->GetRow() << m_CurrentTouchCell->GetColumn() << std::endl;
		}
	}

	if (m_SwapTriggered)
	{
		//std::cout << "m_LastTouchedCell! " << m_LastTouchedCell->GetName() << std::endl;
		//std::cout << "m_CurrentTouchCell! " << m_CurrentTouchCell->GetName() << std::endl;
		if (&m_LastTouchedCell && &m_CurrentTouchCell && !m_LastTouchedCell->IsBusy() && !m_CurrentTouchCell->IsBusy()) //making sure cascading cells are not picked
		{
			System::GetSystemInstance().GetGame()->SwapPieces(m_LastTouchedCell, m_CurrentTouchCell);
			//Board.Instance.DoSwapping(lastTouchedCell, currentTouchCell);
		}
		//Board.Instance.SelectCell(null);
		m_CurrentTouchCell = nullptr;
		m_LastTouchedCell = nullptr;

		m_MouseDownPos = glm::vec2(0);
		m_TouchMovement = 0.0f;
		m_SwapTriggered = false;
		//blockInput = false;
	}

}

Cell* BoardInput::GetTouchedCell(const double x, const double y)
{
	for (std::list<Cell*>::const_iterator cellsIt = m_Cells.begin(),
		end = m_Cells.end();
		cellsIt != end;
		++cellsIt)
	{
		if ((*cellsIt)->IsPointInsideTransform(x, y) && (*cellsIt)->IsMatchable())
		{
			return *cellsIt;
		}
	}
	return nullptr;
}

Cell* BoardInput::NeighbourAtDirection(glm::vec2 swapBeginPos, glm::vec2 swapEndPos)
{
	Constants::Direction dir = Constants::Direction::RIGHT;
	glm::vec2 delta = swapEndPos - swapBeginPos;
	

	if (glm::abs(delta.x) > glm::abs(delta.y))
	{
		if (delta.x > 0)
		{
			dir = Constants::Direction::RIGHT;
		}
		else
		{
			dir = Constants::Direction::LEFT;
		}
	}
	else
	{
		if (delta.y < 0)
		{
			dir = Constants::Direction::UP;
		}
		else
		{
			dir = Constants::Direction::DOWN;
		}
	}

	if (m_LastTouchedCell && !m_LastTouchedCell->IsBusy())
	{
		Cell* neighbourCell = m_LastTouchedCell->GetNeighbour(dir);
		if (!neighbourCell || !neighbourCell->IsMatchable())
		{
			return nullptr;
		}
		return neighbourCell;
	}
	return nullptr;
}
