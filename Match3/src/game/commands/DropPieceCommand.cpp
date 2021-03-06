#include "DropPieceCommand.h"
#include "Constants.h"
#include <GL/glew.h>
#include <iostream>
DropPieceCommand::DropPieceCommand(Piece* droppingPiece, Cell* droppingTo, Cell* droppingFrom, const float delay)
	: m_DroppingPiece(droppingPiece), m_DroppingTo(droppingTo), m_DroppingFrom(droppingFrom), m_Delay(delay), m_Timer(0)
{
	m_Delay = 0.0f;

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


		glm::vec3 distanceToOwner = m_DroppingPiece->GetOwner()->GetTransform()->position - m_DroppingPiece->GetTransform()->position;
		m_dropSpeed += Constants::DROP_GRAVITY * deltaTime;
		float moveDistance = glm::abs(m_dropSpeed * deltaTime);
		float distance = glm::length(distanceToOwner);

		if (moveDistance > distance)    // next frame it will cross the target, so stop here
		{
			m_DroppingPiece->SetTransform(*m_DroppingPiece->GetOwner()->GetTransform());
			m_dropSpeed = 0;
			Finish();
		}
		else
		{
			distanceToOwner = glm::normalize(distanceToOwner);
			glm::vec3 nextPos = moveDistance * distanceToOwner;
			glm::vec3 currentPos = m_DroppingPiece->GetTransform()->position + nextPos;
			m_DroppingPiece->SetPosition(currentPos);
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