#pragma once

// This represents a single mesh object we are drawing

#include <string>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

class cMesh
{
public:
	std::string meshName;

	glm::vec3 positionXYZ;
	glm::vec3 orientationXYZ;		// "angle"
	float scale;

};
