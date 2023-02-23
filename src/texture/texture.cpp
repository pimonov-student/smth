#include "texture.h"

Texture::Texture(GLchar* texture_path)
{
	// ��������� �����������
	unsigned char* img = stbi_load(texture_path, &this->width, &this->height, &this->n_channels, 0);

    // ���������� ��������
    glGenTextures(1, &this->texture);
    // ��������� �� ��� (�����������)
    glBindTexture(GL_TEXTURE_2D, this->texture);
    // ���������� ��������, �� ���� �������� ���� ����������� ����������� � ���������
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    // ���������� ��� �� mipmap � ���� ��������
    glGenerateMipmap(GL_TEXTURE_2D);

    // ������� ������ ��� ����������� (��� ��� ������ �� �����), � ����� ������� ��������
    stbi_image_free(img);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() = default;