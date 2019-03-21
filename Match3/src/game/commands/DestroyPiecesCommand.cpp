#include "DestroyPiecesCommand.h"
#include "Constants.h"
#include "Utility.h"
#include "Cell.h"
#include "Piece.h"
#include "Application.h"

#include <iostream>

DestroyPiecesCommand::DestroyPiecesCommand(const Cells& destroyingCells, const Pieces& destroyingPieces, const float duration)
	: m_DestroyingCells(destroyingCells), m_DestroyingPieces(destroyingPieces), m_Duration(duration), m_PieceScale(Constants::CELL_SIZE)
{
	m_Duration = 0.25f;
	Prepare();
}

DestroyPiecesCommand::~DestroyPiecesCommand()
{
}

void DestroyPiecesCommand::Prepare()
{
	for (Cells::const_iterator cellIt = m_DestroyingCells.begin(),
		end = m_DestroyingCells.end();
		cellIt != end;
		++cellIt)
	{
		(*cellIt)->PrepareForCommand(this);
	}
}

bool DestroyPiecesCommand::IsReady()
{
	for (Cells::const_iterator cellIt = m_DestroyingCells.begin(),
		end = m_DestroyingCells.end();
		cellIt != end;
		++cellIt)
	{
		if (!(*cellIt)->IsReadyForCommand(this))
			return false;
	}
	return true;
}

void DestroyPiecesCommand::Execute()
{
	m_IsActive = true;

	for (Cells::const_iterator cellIt = m_DestroyingCells.begin(),
		end = m_DestroyingCells.end();
		cellIt != end;
		++cellIt)
	{
		(*cellIt)->BeginCommand(this);
	}
}

bool DestroyPiecesCommand::IsActive()
{
	return m_IsActive;
}

void DestroyPiecesCommand::Update(float deltaTime)
{
	if (m_IsActive)
	{
		m_Timer += deltaTime;
		//m_PieceScale -= 0.05f;// 
		glm::vec3 scale = Utility::InterPolate(glm::vec3(m_PieceScale), glm::vec3(0), m_Timer / m_Duration);

		if (m_Timer >= m_Duration)// || m_PieceScale <= 0)
		{
			for (Pieces::iterator cellIt = m_DestroyingPieces.begin(),
				end = m_DestroyingPieces.end();
				cellIt != end;
				++cellIt)
			{
				(*cellIt)->SetScale(glm::vec3(0));
		
			}
			Application::GetApplication().GetGame()->DeletePieces(m_DestroyingPieces);
			Finish();
		}
		else
		{
			for (Pieces::iterator cellIt = m_DestroyingPieces.begin(),
				end = m_DestroyingPieces.end();
				cellIt != end;
				++cellIt)
			{
				(*cellIt)->SetScale(scale);// glm::vec3(m_PieceScale));
				//std::cout << "scale " << (*cellIt)->GetPiece()->GetTransform()->scale.x << std::endl;
			}
		}
	}
}

bool DestroyPiecesCommand::IsComplete()
{
	return m_IsComplete;
}

void DestroyPiecesCommand::Finish()
{
	m_IsActive = false;
	m_IsComplete = true;

	for (Cells::const_iterator cellIt = m_DestroyingCells.begin(),
		end = m_DestroyingCells.end();
		cellIt != end;
		++cellIt)
	{
		(*cellIt)->ReleaseCommand(this);
	}
}
