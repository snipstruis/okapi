#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/quaternion.hpp>

using glm::vec3;
using glm::mat4;
using glm::quat;

struct transform{
	vec3 position;
	quat orientation;
	vec3 scale;
	vec3 origin;
	mat4 get_transform(){
		return glm::translate(mat4(),position)
			 * glm::mat4_cast(orientation)
			 * glm::scale(mat4(),scale)
			 * glm::translate(mat4(),origin);
	}
};

