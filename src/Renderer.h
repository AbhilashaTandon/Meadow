#pragma once
#include <GL/glew.h> //must be before other gl stuff
#include <iostream>

#define ASSERT(x) if (!(x)){ __debugbreak();}
#define GLCheckError(x) GLClearError();\
	x;\
	ASSERT(!GLLogError(#x, __FILE__, __LINE__))

void GLClearError();

std::string GLErrorCodeToName(unsigned int error_code);

bool GLLogError(const char* function, const char* file, unsigned int line);