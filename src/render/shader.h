#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <glad/glad.h>

class Shader
{
public:
	// Идентификатор программы (?)
	GLuint Program;
	// Конструктор, считывающий и собирающий шейдеры (вершинный и фрагментный)
	Shader(const GLchar* vertex_path, const GLchar* fragment_path);
	// Использование программы
	void use();
};

#endif