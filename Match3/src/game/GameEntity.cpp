#include <iostream>
#include "GameEntity.h"

GameEntity::GameEntity()
{
	m_Transform = new Transform();
}

GameEntity::~GameEntity()
{
	delete m_Transform;
	m_Transform = nullptr;

}

void GameEntity::SetTransform(const glm::vec3 & position, const glm::vec3 & scale, const glm::vec3 & rotationAxis, const float rotAngle)
{
	m_Transform->position		= position;
	m_Transform->scale			= scale;
	m_Transform->rotationAxis	= rotationAxis;
	m_Transform->rotationAngle	= rotAngle;
}

void GameEntity::SetTransform(Transform& transform)
{
	*m_Transform = transform;
}

bool GameEntity::CheckCollision(const Transform & transform1, const Transform & transform2)
{
	/*glm::vec3 difference = transform1.position - transform2.position;
	float distance = glm::length(difference);

	if (distance < (transform1.scale.x + transform2.scale.x))
	{
		return true;
	}

	return false;*/

	// Collision x-axis?
	bool collisionX = transform1.position.x + transform1.scale.x >= transform2.position.x &&
		transform2.position.x + transform2.scale.x >= transform1.position.x;
	// Collision y-axis?
	bool collisionY = transform1.position.y + transform1.scale.y >= transform2.position.y &&
		transform2.position.y + transform2.scale.y >= transform1.position.y;
	// Collision only if on both axes
	return collisionX && collisionY;
}


bool GameEntity::IsPointInsideTransform(float pointX, float pointY)
{
	pointY = 600 - pointY;
	float halfWidth = ((*m_Transform).scale.x * 10) / 2;
	float halfHeight = ((*m_Transform).scale.y * 10) / 2;
	// Collision x-axis?
	bool collisionX = (*m_Transform).position.x + halfWidth >= pointX &&
		(*m_Transform).position.x - halfWidth <= pointX;
	// Collision y-axis?
	bool collisionY = (*m_Transform).position.y + halfHeight >= pointY &&
		(*m_Transform).position.y - halfHeight <= pointY;
	// Collision only if on both axes
	return collisionX && collisionY;

}

