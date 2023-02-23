#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include "stb_image.h"

class Texture
{
public:
	// ������������� �������
	GLuint texture;
	// ���������� ������� � �������� �������
	int width, height, n_channels;
	// �����������
	Texture(GLchar* texture_path);
	// ����������
	~Texture();
};

#endif