#include "SpawnPieceCommand.h"

SpawnPieceCommand::SpawnPieceCommand(Piece * droppingPiece, Cell * droppingTo, Cell * droppingFrom, const float delay)
	:m_DroppingPiece(droppingPiece), m_DroppingTo(droppingTo), m_DroppingFrom(droppingFrom), m_Delay(delay), m_Timer(0)
{
	m_Delay = 0.075f;
	Prepare();
}

SpawnPieceCommand::~SpawnPieceCommand()
{
}


void SpawnPieceCommand::Prepare()
{
	m_DroppingFrom->PrepareForCommand(this);
	m_DroppingTo->PrepareForCommand(this);
}

bool SpawnPieceCommand::IsReady()
{
	return m_DroppingTo->IsReadyForCommand(this) && m_DroppingFrom->IsReadyForCommand(this);
}
void SpawnPieceCommand::Execute()
{
	m_IsActive = true;
	m_DroppingFrom->BeginCommand(this);
	m_DroppingTo->BeginCommand(this);
}

bool SpawnPieceCommand::IsActive()
{
	return m_IsActive;
}

void SpawnPieceCommand::Update(float deltaTime)
{
	if (m_IsActive && m_DroppingPiece)
	{
		m_Timer += deltaTime;

		if (m_Timer < m_Delay)
		{
			//return;
		}
		if (m_DroppingFrom->GetCurrentCommand() && m_DroppingFrom->GetCurrentCommand() == this && m_DroppingPiece->GetTransform()->position.y < m_DroppingFrom->GetTransform()->position.y - 12.5) //TODO make seperate command for spawn
		{
			m_DroppingFrom->ReleaseCommand(this);
		}

		glm::vec3 distanceToOwner = m_DroppingTo->GetTransform()->position - m_DroppingPiece->GetTransform()->position;
		m_dropSpeed += Constants::DROP_GRAVITY * deltaTime;
		float moveDistance = glm::abs(m_dropSpeed * deltaTime);
		float distance = glm::length(distanceToOwner);

		if (moveDistance > distance)    // next frame it will cross the target, so stop here
		{
			m_DroppingPiece->SetTransform(*m_DroppingTo->GetTransform());
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


bool SpawnPieceCommand::IsComplete()
{
	return m_IsComplete;
}

void SpawnPieceCommand::Finish()
{
	m_IsActive = false;
	m_IsComplete = true;

	m_DroppingTo->ReleaseCommand(this);
}