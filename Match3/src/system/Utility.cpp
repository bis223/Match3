#include "Utility.h"

glm::vec3 Utility::InterPolate(const glm::vec3& startPos, const glm::vec3& finalPos, float time)
{
	return startPos + ((finalPos - startPos) * time);
}

