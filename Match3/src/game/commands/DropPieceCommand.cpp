#include "DropPieceCommand.h"
#include "Constants.h"
#include <GL/glew.h>
#include <iostream>
DropPieceCommand::DropPieceCommand(Piece* droppingPiece, Cell* droppingTo, Cell* droppingFrom, const float delay)
	: m_DroppingPiece(droppingPiece), m_DroppingTo(droppingTo), m_DroppingFrom(droppingFrom), m_Delay(delay), m_Timer(0)
{
	m_Delay = 0.0f;
	if (m_DroppingFrom->IsSpawner())
	{
		m_Delay = 0.075f;// Constants::SPAWN_DELAY;
	}
	Prepare();

}

DropPieceCommand::~DropPieceCommand()
{
}

void DropPieceCommand::Prepare()
{
	m_DroppingFrom->PrepareForCommand(this);
	m_DroppingTo->PrepareForCommand(this);
}

bool DropPieceCommand::IsReady()
{
	return m_DroppingTo->IsReadyForCommand(this) && m_DroppingFrom->IsReadyForCommand(this);
}
void DropPieceCommand::Execute()
{
	m_IsActive = true;
	m_DroppingFrom->BeginCommand(this);
	m_DroppingTo->BeginCommand(this);
	if (!m_DroppingFrom->IsSpawner())
		m_DroppingFrom->ReleaseCommand(this);
}

bool DropPieceCommand::IsActive()
{
	return m_IsActive;
}

void DropPieceCommand::Update(float deltaTime)
{
	if (m_IsActive && m_DroppingPiece)
	{
		m_Timer += deltaTime;

		if (m_Timer < m_Delay)
		{
			//return;
		}
		if (m_DroppingFrom->GetCurrentCommand() && m_DroppingFrom->GetCurrentCommand() == this && m_Timer > m_Delay) //TODO make seperate command for spawn
		{
			//if(m_DroppingFrom->IsSpawner())
				m_DroppingFrom->ReleaseCommand(this);
		}

		glm::vec3 distanceToOwner = m_DroppingTo->GetTransform()->position - m_DroppingPiece->GetTransform()->position;
		m_dropSpeed += Constants::DROP_GRAVITY * deltaTime;
		float moveDistance = glm::abs(m_dropSpeed * deltaTime);
		float distance = glm::length(distanceToOwner);

		//if (m_DroppingTo->GetRow() == 7 && m_DroppingTo->GetColumn() == 7)
		//{
		//	std::cout << "moveDistance " << moveDistance << std::endl;
		//	std::cout << "distanceToOwner.length() " << distance << std::endl;
		//	//std::cout << "distanceToOwner " << distanceToOwner << std::endl;
		//	//std::cout << "to y " << m_DroppingTo->GetTransform()->position.y << std::endl;
		//	std::cout << "y " << m_DroppingPiece->GetTransform()->position.y << std::endl;
		//}

		if (moveDistance > distance)    // next frame it will cross the target, so stop here
		{
			//m_DroppingPiece->GetTransform()->position = m_DroppingTo->GetTransform()->position;
			m_DroppingPiece->SetTransform(*m_DroppingTo->GetTransform());
			m_dropSpeed = 0;
			Finish();
			//std::cout << "Finished " << m_DroppingTo->GetRow() << " " << m_DroppingTo->GetColumn() << std::endl;
		}
		else
		{
			distanceToOwner = glm::normalize(distanceToOwner);
			//m_DroppingPiece->GetTransform()->position += moveDistance * distanceToOwner;
			glm::vec3 nextPos = moveDistance * distanceToOwner;
			//if (m_DroppingTo->GetRow() == 7 && m_DroppingTo->GetColumn() == 7)
			//{
			//	//std::cout << "moveDistance " << moveDistance << std::endl;
			//	//std::cout << "distanceToOwner.length() " << distance << std::endl;
			////	std::cout << "nextPos yy " << nextPos.y << std::endl;
			//	//std::cout << "to y " << m_DroppingTo->GetTransform()->position.y << std::endl;
			//	std::cout << "before y " << m_DroppingPiece->GetTransform()->position.y << std::endl;
			//}
			glm::vec3 currentPos = m_DroppingPiece->GetTransform()->position + nextPos;
			m_DroppingPiece->SetPosition(currentPos);
			//m_DroppingPiece->SetTransform(currentPos, m_DroppingPiece->GetTransform()->scale, m_DroppingPiece->GetTransform()->rotationAxis, m_DroppingPiece->GetTransform()->rotationAngle);
			//std::cout << "Finished " << m_DroppingTo->GetRow() << " " << m_DroppingTo->GetColumn() << std::endl;
			//if (m_DroppingTo->GetRow() == 7 && m_DroppingTo->GetColumn() == 7)
			//{
			//	//std::cout << "moveDistance " << moveDistance << std::endl;
			//	//std::cout << "distanceToOwner.length() " << distance << std::endl;
			//	std::cout << "nextPos yy " << nextPos.y << std::endl;
			//	//std::cout << "to y " << m_DroppingTo->GetTransform()->position.y << std::endl;
			//	std::cout << "after y " << m_DroppingPiece->GetTransform()->position.y << std::endl;
			//}
		}


	}
}


bool DropPieceCommand::IsComplete()
{
	return m_IsComplete;
}

void DropPieceCommand::Finish()
{
	m_IsActive = false;
	m_IsComplete = true;

	m_DroppingTo->ReleaseCommand(this);
}