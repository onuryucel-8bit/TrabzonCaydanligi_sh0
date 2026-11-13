#pragma once

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

struct MeshMatrix
{
	size_t m_index = 0;
	glm::vec3 m_pos{ 0.0f };
	glm::vec3 m_rot{ 0.0f };
	glm::vec3 m_scale{ 1.0f };

	glm::mat4x4 getWorldMatrix()
	{
		//T * Rz * Ry * Rx * S
		glm::mat4x4 mat = glm::translate(glm::mat4x4(1.0f), m_pos);
		mat = glm::rotate(mat, glm::radians(m_rot.z), glm::vec3(0, 0, 1));
		mat = glm::rotate(mat, glm::radians(m_rot.y), glm::vec3(0, 1, 0));
		mat = glm::rotate(mat, glm::radians(m_rot.x), glm::vec3(1, 0, 0));

		return glm::scale(mat, m_scale);
	}
};