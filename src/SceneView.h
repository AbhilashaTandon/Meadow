#pragma once
#include <GL/glew.h> //must be before other gl stuff
#include <GLFW/glfw3.h>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

class SceneView
{
public:
	void key_callback(int key, int action);

	SceneView(glm::vec3 init_camera_dir, glm::vec3 camera_up, glm::vec3 init_camera_pos, float near_plane, float far_plane, float aspect_ratio);

	SceneView(float aspect_ratio);

	glm::mat4 get_transform();

	GLfloat get_time();

	glm::vec3 camera_pos;

	inline glm::vec3 get_camera_pos() {
		return camera_pos;
	}

	inline glm::vec3 get_camera_dir() {
		return camera_dir;
	}


private:

	glm::mat3 rot_y(float angle);

	glm::mat3 rot_z(float angle);

	glm::vec3 camera_dir;

	glm::vec3 camera_up;


	float near_plane;
	float far_plane;

	float aspect_ratio;

	GLfloat time_val;

	GLfloat last_up;
	GLfloat last_down;
	GLfloat last_left;
	GLfloat last_right;
};






