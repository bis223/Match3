#include "OrthoCamera.h"

OrthoCamera::OrthoCamera(float width, float height, float nearZ, float farZ) 
	: m_Width(width), m_Height(height),
	  m_NearZ(nearZ), m_FarZ(farZ), m_Position(glm::vec3(0, 0, 0))
{
}

void OrthoCamera::SetProjection(Graphics& graphics) const
{
	glm::mat4 projection = glm::ortho(0.0f, m_Width, 0.0f, m_Height, m_NearZ, m_FarZ);
	graphics.SetProjectionMatrix(projection);
}

void OrthoCamera::SetView(Graphics& graphics) const
{
	graphics.SetViewMatrix(glm::translate(glm::mat4(1.0f), m_Position));
}

void OrthoCamera::SetPosition(const glm::vec3& position)
{
	m_Position = position;
}
