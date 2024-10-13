#include "SceneView.h"

#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <iostream>

void SceneView::key_callback(int key, int action)
{
	time_val = (GLfloat)glfwGetTime();

	if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT) {
		if (last_right != 0) {
			camera_dir = rot_y(last_right - time_val) * camera_dir;
		}
		last_right = time_val;
	}

	if (key == GLFW_KEY_UP && action == GLFW_REPEAT) {
		if (last_up != 0) {
			camera_pos += camera_dir * (last_up - time_val);
		}
		last_up = time_val;
	}

	if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT) {
		if (last_down != 0) {
			camera_pos += camera_dir * (last_down - time_val);
		}
		last_down = time_val;
	}

	if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT) {
		if (last_left != 0) {
			camera_dir = rot_y(time_val - last_left) * camera_dir;
		}
		last_left = time_val;
	}

	if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE) {
		last_right = 0;
	}

	if (key == GLFW_KEY_UP && action == GLFW_RELEASE) {
		last_up = 0;
	}

	if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE) {
		last_down = 0;
	}

	if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE) {
		last_left = 0;
	}

	std::cout << glm::to_string(camera_pos) << std::endl;
	std::cout << glm::to_string(camera_dir) << std::endl;
}

SceneView::SceneView(glm::vec3 init_camera_dir, glm::vec3 camera_up, glm::vec3 init_camera_pos, float near_plane, float far_plane, float aspect_ratio)
{
	this->camera_dir = init_camera_dir;
	this->camera_up = camera_up;
	this->camera_pos = init_camera_pos;
	this->near_plane = near_plane;
	this->far_plane = far_plane;
	this->aspect_ratio = aspect_ratio;
	time_val = (GLfloat)0.;

	last_up = 0;
	last_down = 0;
	last_left = 0;
	last_right = 0;
}

SceneView::SceneView(float aspect_ratio)
{
	this->camera_dir = glm::vec3(0.0f, 0.0f, -1.0f);
	this->camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->camera_pos = glm::vec3(0., 0., 0.);
	this->near_plane = 0.1f;
	this->far_plane = 100.0f;
	this->aspect_ratio = aspect_ratio;
	time_val = (GLfloat)0.;

	last_up = 0;
	last_down = 0;
	last_left = 0;
	last_right = 0;
}

glm::mat4 SceneView::get_transform()
{
	
	glm::vec3 camera_target = camera_pos + glm::normalize(camera_dir);

	glm::mat4 proj = glm::perspective(glm::radians(45.f), aspect_ratio, near_plane, far_plane);

	glm::mat4 view = glm::lookAt(camera_pos,
		camera_target,
		camera_up);

	return proj * view;
}

GLfloat SceneView::get_time()
{
	time_val = (GLfloat)glfwGetTime();
	return time_val;
}

glm::mat3 SceneView::rot_y(float angle) {
	float sin = glm::sin(angle);
	float cos = glm::cos(angle);
	return glm::mat3(cos, 0., sin, 0., 1., 0., -sin, 0., cos);
}

glm::mat3 SceneView::rot_z(float angle) {
	float sin = glm::sin(angle);
	float cos = glm::cos(angle);
	return glm::mat3(cos, -sin, 0, sin, cos, 0, 0, 0, 1);
}