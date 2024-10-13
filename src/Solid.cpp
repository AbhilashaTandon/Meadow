#include "Solid.h"
#include "SimplexNoise.h"

Solid::Solid(glm::vec3 center)
{
	this->center = center;

	this->points = std::vector<glm::vec3>();
	this->indices = std::vector<unsigned int>();
	this->normals = std::vector<glm::vec3>();

	num_vertices = 0;
	num_triangles = 0;

}

std::vector<float> Solid::get_vertices(bool use_normals) {

	std::vector<float> vertices;

	for (unsigned int i = 0; i < points.size(); i++) {
		vertices.push_back(points[i].x);
		vertices.push_back(points[i].y);
		vertices.push_back(points[i].z);

		if (use_normals) {
			vertices.push_back(normals[i].x);
			vertices.push_back(normals[i].y);
			vertices.push_back(normals[i].z);
		}

	}

	return vertices;
}

void Solid::rotate_x(float angle)
{
	//TODO:
}

void Solid::rotate_y(float angle)
{
	//TODO:
}

void Solid::rotate_z(float angle)
{
	//TODO:
}

void Solid::scale(glm::vec3 size)
{
	for (int i = 0; i < points.size(); i ++) {
		points[i] = (points[i] - center) * size + center;
	}
}

void Solid::translate(glm::vec3 delta)
{
	for (int i = 0; i < points.size(); i++) {
		points[i] += delta;
	}
	center += delta;
}

void Solid::MakeSphere(float radius, unsigned int num_lat, unsigned int num_long)
{

	num_vertices = 2 + (num_lat - 1) * num_long;
	num_triangles = num_long * num_lat;
	//SET VERTICES

	//north pole
	glm::vec3 current_vec = glm::vec3(0, 0, radius);

	points.push_back(center - current_vec);
	normals.push_back(current_vec / radius);
	//current vec is distance to center, we normalize it to get normal vector

	for (float j = 1; j < num_lat; j += 1) { //excludes north and south poles
		for (float i = 0; i < num_long; i += 1) {
			//does rings at each latitude line down the sphere

			//math convention for spherical coordinates
			float phi = i * tau / (num_long);
			float theta = j * tau / (2 * num_lat);

			//temp variables to add to vec3
			float x_coord = radius * std::sin(theta) * std::cos(phi);
			float y_coord = radius * std::sin(theta) * std::sin(phi);
			float z_coord = radius * std::cos(theta);

			current_vec = glm::vec3(x_coord, y_coord, z_coord);

			points.push_back(center - current_vec);

			normals.push_back(current_vec / radius); //current vec is distance to center, we normalize it to get normal vector
		}
	}


	//south pole
	current_vec = glm::vec3(0, 0, -radius);

	points.push_back(center - current_vec);
	normals.push_back(current_vec / radius);

	//SET INDICES

	for (unsigned int i = 0; i < num_long; i++) {

		indices.push_back(1 + i);
		indices.push_back(0); //north pole
		indices.push_back(1 + ((i + 1) % num_long));
		//adds triangle from north pole to first set of points
	}

	for (unsigned int j = 0; j < num_lat - 2; j++) {
		//add 2 triangles for each tile
		for (unsigned int i = 0; i < num_long; i += 1) {

			//next point on latitude circle

			unsigned int next_point = (i + 1) % num_long; //use modulus to wraparound

			indices.push_back(1 + j * num_long + i);
			indices.push_back(1 + (j + 1) * num_long + i); //down 1
			indices.push_back(1 + j * num_long + next_point); //right 1

			indices.push_back(1 + (j + 1) * num_long + next_point);
			indices.push_back(1 + (j + 1) * num_long + i); //down 1
			indices.push_back(1 + j * num_long + next_point); //right 1
		}
	}

	for (unsigned int i = 0; i < num_long; i++) {
		indices.push_back(1 + (num_lat - 2) * num_long + i); //last circle
		indices.push_back(1 + (num_lat - 2) * num_long + (i + 1) % num_long); //last circle next point
		indices.push_back((num_lat - 1) * (num_long)+1);
		//adds triangle from north pole to first set of points
	}

	assert(indices.size() == num_triangles * 3);
	assert(points.size() == num_vertices);
}

void Solid::MakeCylinder(float radius, float height, unsigned int num_sides)
{
	num_vertices = 2 * (num_sides + 1);
	num_triangles = num_sides * 4;
	//SET VERTICES
	
	//north pole
	glm::vec3 north_pole = glm::vec3(0, height / 2, 0);
	glm::vec3 current_vec = north_pole;

	points.push_back(center - current_vec);
	normals.push_back(current_vec / (height / 2));
	//current vec is distance to center, we normalize it to get normal vector

	float dist_to_center = sqrt(radius * radius + height * height / 4); //pythagorean theorem
	//distance from top and bottom edge to center

	for (unsigned int i = 0; i < num_sides; i++) { //adds perimeter of top plane
		float angle = i * tau / num_sides;
		current_vec = north_pole + glm::vec3(glm::cos(angle), 0, glm::sin(angle)) * radius;
		points.push_back(center - current_vec);
		normals.push_back(current_vec / dist_to_center);
	}

	glm::vec3 south_pole = glm::vec3(0, -height / 2, 0);

	points.push_back(center - south_pole);
	normals.push_back(south_pole / (height / 2));

	for (unsigned int i = 0; i < num_sides; i++) { //adds perimeter of top plane
		float angle = i * tau / num_sides;
		current_vec = south_pole + glm::vec3(glm::cos(angle), 0, glm::sin(angle)) * radius;
		points.push_back(center - current_vec);
		normals.push_back(current_vec / dist_to_center);
	}

	//SET INDICES

	for (unsigned int i = 0; i < num_sides; i++) {
		indices.push_back(1 + i);
		indices.push_back(0); //north pole
		indices.push_back(1 + ((i + 1) % num_sides));
		//adds top face
	}

	for (unsigned int i = 0; i < num_sides; i++) {
		indices.push_back(2 + i + num_sides);
		indices.push_back(1 + num_sides); //south pole
		indices.push_back(2 + ((i + 1) % num_sides) + num_sides);
		//adds bottom face

		//these indices of these points are 1 + num_sides more than the top face
	}

	for (unsigned int i = 0; i < num_sides; i++) {
		indices.push_back(1 + i);
		indices.push_back(1 + ((i + 1) % num_sides));
		indices.push_back(2 + i + num_sides);

		//top triangle

		indices.push_back(1 + ((i + 1) % num_sides));
		indices.push_back(2 + i + num_sides);
		indices.push_back(2 + ((i + 1) % num_sides) + num_sides);
	}

	assert(indices.size() == num_triangles * 3);
	assert(points.size() == num_vertices);
}

void Solid::MakeSurface(unsigned int num_tiles_width, unsigned int num_tiles_depth, float width, float depth)
{
	num_vertices = num_tiles_depth * num_tiles_width;
	num_triangles = 2 * (num_tiles_depth - 1) * (num_tiles_width - 1);

	for (int i = 0; i < num_tiles_width; i++) {
		for (int j = 0; j < num_tiles_depth; j++) {
			float x_coord = center.x - width / 2 + width / num_tiles_width * i;
			float z_coord = center.z - depth / 2 + depth / num_tiles_depth * j;

			points.push_back(glm::vec3(x_coord, 0, z_coord));

			normals.push_back(glm::vec3(0., 1., 0.));
		}
	}

	for (int i = 0; i < num_tiles_width - 1; i++) {
		for (int j = 0; j < num_tiles_depth - 1; j++) {
			//triangle 1
			indices.push_back(i * num_tiles_width + j);
			indices.push_back((i + 1) * num_tiles_width + j);
			indices.push_back(i * num_tiles_width + j + 1);

			//triangle 2
			indices.push_back((i + 1) * num_tiles_width + j + 1);
			indices.push_back((i + 1) * num_tiles_width + j);
			indices.push_back(i * num_tiles_width + j + 1);

		}
	}
	
	assert(indices.size() == num_triangles * 3);
	assert(points.size() == num_vertices);
}


void Solid::MakeCuboid(float height, float width, float depth)
{
	num_vertices = 8;
	num_triangles = 12;

	for (int i = -1; i < 2; i+=2) {
		for (int j = -1; j < 2; j+=2) {
			for (int k = -1; k < 2; k+=2) {

				glm::vec3 pos = glm::vec3(i * width, j * height, k * depth);

				points.push_back(pos); //vertices, +-width +-height +-depth

				normals.push_back(glm::normalize(pos));
			}
		}
	}

	unsigned int indices_arr[36] = {
	0, 1, 2,
	1, 2, 3,

	0, 1, 4,
	1, 4, 5,

	0, 2, 4,
	0, 4, 6,

	4, 5, 6,
	5, 6, 7,

	2, 3, 6,
	3, 6, 7,

	1, 3, 5,
	3, 5, 7,
	};

	for (unsigned int i = 0; i < 36; i++) {
		indices.push_back(indices_arr[i]);
	}

	assert(indices.size() == num_triangles * 3);
	assert(points.size() == num_vertices);
}

 Solid::Solid(Solid a, Solid b){
	 this->center = (a.center + b.center) * .5f;

	 this->points = std::vector<glm::vec3>();
	 this->indices = std::vector<unsigned int>();
	 this->normals = std::vector<glm::vec3>();

	 this->points.insert(this->points.end(), a.points.cbegin(), a.points.cend());
	 this->points.insert(this->points.end(), b.points.cbegin(), b.points.cend());

	 this->indices.insert(this->indices.end(), a.indices.cbegin(), a.indices.cend());
	 this->indices.insert(this->indices.end(), b.indices.cbegin(), b.indices.cend());

	 this->normals.insert(this->normals.end(), a.normals.cbegin(), a.normals.cend());
	 this->normals.insert(this->normals.end(), b.normals.cbegin(), b.normals.cend());

	 this->num_vertices = a.num_vertices + b.num_vertices;
	 this->num_triangles = a.num_triangles + b.num_triangles;

}



