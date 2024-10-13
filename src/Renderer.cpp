#include "Renderer.h"

void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

std::string GLErrorCodeToName(unsigned int error_code) {
	switch (error_code) {
	case 0:
		return "GL_NO_ERROR";
	case 0x0500:
		return "GL_INVALID_ENUM";
	case 0x0501:
		return "GL_INVALID_VALUE";
	case 0x0502:
		return "GL_INVALID_OPERATION";
	case 0x0503:
		return "GL_STACK_OVERFLOW";
	case 0x0504:
		return "GL_STACK_UNDERFLOW";
	case 0x0505:
		return "GL_OUT_OF_MEMORY";
	default:
		return "UNKNOWN_ERROR";
	}
}

bool GLLogError(const char* function, const char* file, unsigned int line) {
	bool error_happened = false;
	while (GLenum error = glGetError() != GL_NO_ERROR) {
		std::cout << "[OpenGL Error]: "
			<< error << " (" << std::hex << error << " " << GLErrorCodeToName(error) << ") "
			<< function << " " << file << ": Line " << line;
		std::cout << std::endl;
		error_happened = true;
	}
	return error_happened;
}