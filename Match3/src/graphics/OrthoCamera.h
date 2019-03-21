#pragma once

#include "graphics/Graphics.h"

class OrthoCamera
{
public:
	OrthoCamera(float width, float height, float nearZ, float farZ);
	void SetProjection(Graphics& graphics) const;
	void SetView(Graphics& graphics) const;
	void SetPosition(const glm::vec3& position);
private:
	float m_Width, m_Height, m_NearZ, m_FarZ;
	glm::vec3 m_Position;
};

