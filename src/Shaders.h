#pragma once
#include <GL/glew.h> //must be before other gl stuff
#include <GLFW/glfw3.h>

#include <string>

#define BUFFER_SIZE 4096

#include <iostream>

#include <fstream>

unsigned int compileShader(unsigned int type, const std::string& source);

unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);

std::string loadShader(const std::string file_path);