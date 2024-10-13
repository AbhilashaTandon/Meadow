#include <GL/glew.h> //must be before other gl stuff
#include <GLFW/glfw3.h>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "Solid.h"
#include "Shaders.h"
#include "Renderer.h"
#include "SceneView.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

#define HEIGHT 1080
#define WIDTH 1920
#define ASPECT_RATIO ((float)HEIGHT / (float)WIDTH)

SceneView dynamic_view = SceneView(ASPECT_RATIO);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	dynamic_view.key_callback(key, action);
} //i need a wrapper because I cant use a class method for a function pointer

SceneView fixed_view = SceneView(ASPECT_RATIO);

int main(void)
{
	GLFWwindow* window;

	unsigned int screen_height = HEIGHT;
	unsigned int screen_width = WIDTH;
	float aspect_ratio = ASPECT_RATIO;

	/* Initialize the library */
	if (!glfwInit()) {
		return -1;
	}

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(screen_width, screen_height, "Hello World", NULL, NULL);


	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	GLenum glew_err = glewInit();

	if (GLEW_OK != glew_err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_err));
		return -1;
	}

	glfwSetKeyCallback(window, key_callback);

	//SHADERS

	const std::string version = "#version 330 core\n\n";

	const std::string vertShader = loadShader("res/shaders/shader.vert");
	const std::string fragShader = loadShader("res/shaders/shader.frag");

	const std::string helperShader = loadShader("res/shaders/header.glsl");
	//contains header functions

	std::stringstream vertStream;
	vertStream << version << helperShader << "\n\n" << vertShader;

	std::stringstream fragStream;
	fragStream << version << helperShader << "\n\n" << fragShader;

	unsigned int shader_id = createShader(vertStream.str(), fragStream.str());

	GLCheckError(glUseProgram(shader_id));

	GLCheckError(glEnable(GL_DEPTH_TEST));
	GLCheckError(glDepthMask(GL_TRUE));
	GLCheckError(glDepthFunc(GL_LESS));

	//OBJECTS
	{
		Solid surf = Solid(glm::vec3(0, 0, -12));
		//x.MakeSphere(1., 40, 20);
		surf.MakeSurface(512, 512, 100, 100);


		std::vector<float> vertices_vec = surf.get_vertices(true);
		std::vector<unsigned int> indices_vec = surf.get_indices();

		unsigned int num_vertices = (unsigned int)vertices_vec.size();
		unsigned int num_indices = (unsigned int)indices_vec.size();

		float* vertices = vertices_vec.data();

		unsigned int* indices = indices_vec.data();

		unsigned int vao;
		GLCheckError(glGenVertexArrays(1, &vao));
		GLCheckError(glBindVertexArray(vao));

		VertexBuffer vb(vertices, num_vertices * sizeof(vertices[0]));

		GLCheckError(glEnableVertexAttribArray(0));
		GLCheckError(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0));
		//3 is dimension

		GLCheckError(glEnableVertexAttribArray(1));
		GLCheckError(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0));
		//3 is dimension

		IndexBuffer ib = IndexBuffer(indices, num_indices);

		vertices_vec.clear();
		indices_vec.clear(); //i assume opengl copies data to internal buffers so i dont need these laying around

		GLCheckError(glClearColor(0.6f, 0.808f, 0.922f, 1.0f));

		float last_frame = -0.01f; //initial value, assumes 100 fps

		float moving_framerate_average = 0.f;

		float framerate_smoothing = .95f;

		glm::mat4 fixed_transform = fixed_view.get_transform();

		//coordinates of camera relative to surface used to render terrain

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{

			surf.MakeRough(1., 100., 2., .5, 4, dynamic_view);

			float camera_height = surf.GetNoise(4, dynamic_view.camera_pos.x, dynamic_view.camera_pos.z);

			dynamic_view.camera_pos.y = -camera_height + 1.;

			glm::mat4 terrain_transform = dynamic_view.get_transform();

			

			//coordinates of camera relative to terrain

			// Clear the screen
			GLCheckError(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

			GLfloat time_val = dynamic_view.get_time();

			float current_framerate = 1.f / (time_val - last_frame);
			moving_framerate_average = moving_framerate_average * framerate_smoothing + current_framerate * (1.f - framerate_smoothing);

			last_frame = time_val;

			std::stringstream framerate;
			framerate << std::fixed << std::setprecision(1) << moving_framerate_average << " FPS";

			glfwSetWindowTitle(window, framerate.str().c_str());

			/*int time_value_loc = GLCheckError(glGetUniformLocation(shader_id, "time"));

			GLCheckError(glUniform1f(time_value_loc, time_val));*/

			int terrain_view_loc = glGetUniformLocation(shader_id, "terrain_view");
			int fixed_view_loc = glGetUniformLocation(shader_id, "fixed_view");

			GLCheckError(glUniformMatrix4fv(terrain_view_loc, 1, GL_FALSE, glm::value_ptr(terrain_transform)));
			GLCheckError(glUniformMatrix4fv(fixed_view_loc, 1, GL_FALSE, glm::value_ptr(fixed_transform)));

			int camera_pos_vec_loc = glGetUniformLocation(shader_id, "camera_pos");

			GLCheckError(glUniform3fv(camera_pos_vec_loc, 1, glm::value_ptr(dynamic_view.camera_pos)));

			GLCheckError(glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, nullptr));

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

		GLCheckError(glDeleteProgram(shader_id));
	}
	glfwTerminate();

	return 0;
}