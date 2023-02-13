#include "shader.h"

Shader::Shader(const GLchar* vertex_path, const GLchar* fragment_path)
{
	// ��������� ��������
	std::string vertex_code, fragment_code;
	// ������ ��� ������ �� ������
	std::ifstream vertex_file, fragment_file;
	// ������ ��� ������ �� ����������� �� ������ �����
	std::stringstream vertex_stream, fragment_stream;
	// ��� �����, �������� ����������� ���� ������� ������ �����������
	vertex_file.exceptions(std::ifstream::badbit);
	fragment_file.exceptions(std::ifstream::badbit);

	try
	{
		// ��������� ����� �� �� ����
		vertex_file.open(vertex_path);
		fragment_file.open(fragment_path);
		// ��������� �� ������ � ������
		vertex_stream << vertex_file.rdbuf();
		fragment_stream << fragment_file.rdbuf();
		// ��������� �����
		vertex_file.close();
		fragment_file.close();
		// ��������������� ������ � ������
		vertex_code = vertex_stream.str();
		fragment_code = fragment_stream.str();
	}
	catch(std::ifstream::failure error)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	// ��������� � ������ � OpenGL
	// �� �� ���������, ��������������� ���
	const GLchar* gl_vertex_code = vertex_code.c_str();
	const GLchar* gl_fragment_code = fragment_code.c_str();
}