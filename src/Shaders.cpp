#include "Shaders.h"

 unsigned int compileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str(); //gl wants c style string
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	//ERROR HANDLING
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		GLint length = BUFFER_SIZE;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* shader_error = (char*)_malloca(BUFFER_SIZE * sizeof(char));
		glGetShaderInfoLog(id, length, &length, shader_error);

		switch (type) {
		case GL_VERTEX_SHADER:
			std::cerr << "Error in vertex shader" << std::endl;
			break;
		case GL_FRAGMENT_SHADER:
			std::cerr << "Error in fragment shader" << std::endl;
			break;
		default:
			std::cerr << "Error in other shader" << std::endl;
			break;
		}
		std::cerr << shader_error << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

 unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader) {
	//these strings are the source code of the shaders

	unsigned int program = glCreateProgram();
	unsigned int vertId = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fragId = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vertId);
	glAttachShader(program, fragId);
	glLinkProgram(program);
	glValidateProgram(program);


	//ERROR HANDLING
	GLint length = BUFFER_SIZE;
	char* program_error = (char*)_malloca(BUFFER_SIZE * sizeof(char));
	GLsizei error_length;
	glGetProgramInfoLog(program, length, &error_length, program_error);
	if (error_length != 0) {
		std::cerr << "Error with program" << std::endl;
		std::cerr << program_error << std::endl;
	}

	glDeleteShader(vertId);
	glDeleteShader(fragId);

	return program;
}

std::string loadShader(const std::string file_path) {
	std::ifstream shaderFile;
	shaderFile.open(file_path);
	if (shaderFile.is_open()) {
		std::string out((std::istreambuf_iterator<char>(shaderFile)),
			(std::istreambuf_iterator<char>()));
		shaderFile.close();
		return out;
	}
	else {
		std::cerr << "Unable to open shader file " << file_path << std::endl;
		return "";
	}
}
