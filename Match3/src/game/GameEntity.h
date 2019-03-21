#pragma once

#include "Graphics.h"
#include "Game.h"

#include <string>

struct Transform
{
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotationAxis;
	float rotationAngle;
};

class GameEntity
{
public:
	GameEntity();
	virtual ~GameEntity();

	Transform* GetTransform()							
	{ 

		return m_Transform;
	}
	glm::vec3 GetPosition()	const						{ return m_Transform->position; }
	inline void SetPosition(const glm::vec3& position)		{ m_Transform->position = position; }
	inline void SetRotation(const glm::vec3& rotationAxis, const float rotAngle) { m_Transform->rotationAxis = rotationAxis; m_Transform->rotationAngle = rotAngle; }
	inline void SetScale(const glm::vec3& scale)			{ m_Transform->scale = scale; }

	void SetTransform(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotationAxis, const float rotAngle);
	void SetTransform(Transform& transform);
	bool CheckCollision(const Transform& transform1, const Transform& transform2);
	bool IsPointInsideTransform(float pointX, float pointY);

	inline void SetName(const std::string& name) { m_Name = name; }
	inline std::string& GetName() { return m_Name; }
private:
	Transform* m_Transform;
	std::string m_Name;
};


