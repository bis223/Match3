#pragma once
#include "Graphics.h"

class Utility
{
public:
	static glm::vec3 InterPolate(const glm::vec3& startPos, const glm::vec3& finalPos, float time);
};
