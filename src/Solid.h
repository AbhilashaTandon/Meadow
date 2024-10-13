#pragma once

#include <vector>
#include <glm/glm.hpp>

static const float tau = 6.283185307f;

class Solid //all 3d object classes inherit this
{
public:

	Solid(glm::vec3 center);

	std::vector<glm::vec3> get_points() {
		return points;
	}
	std::vector<unsigned int> get_indices() {
		return indices;
	}
	std::vector<glm::vec3> get_normals() {
		return normals;
	}

	std::vector<float> get_vertices(bool use_normals);

	void rotate_x(float angle);
	void rotate_y(float angle);
	void rotate_z(float angle);
	void scale(glm::vec3 size);
	void translate(glm::vec3 delta);

	void MakeSphere(float radius, unsigned int num_lat, unsigned int num_long);

	void MakeCylinder(float radius, float height, unsigned int num_sides);

	void MakeSurface(unsigned int num_tiles_width, unsigned int num_tiles_depth, float width, float depth);

	void MakeCuboid(float height, float width, float depth);

	Solid(Solid a, Solid b);

protected:
	std::vector<glm::vec3> points;
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> normals;

	glm::vec3 center; //should be roughly center of mass, its the point that things rotate and scale based on

	unsigned int num_vertices;
	unsigned int num_triangles;
};

