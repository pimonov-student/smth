#include "shader.h"

Shader::Shader(const GLchar* vertex_path, const GLchar* fragment_path)
{
	// Исходники шейдеров
	std::string vertex_code, fragment_code;
	// Потоки для чтения из файлов
	std::ifstream vertex_file, fragment_file;
	// Потоки для чтения из прочитанных из файлов строк
	std::stringstream vertex_stream, fragment_stream;
	// Как понял, проверка возможности этих потоков кидать исключенимя
	vertex_file.exceptions(std::ifstream::badbit);
	fragment_file.exceptions(std::ifstream::badbit);

	try
	{
		// Открываем файлы по их пути
		vertex_file.open(vertex_path);
		fragment_file.open(fragment_path);
		// Считываем из файлов в потоки
		vertex_stream << vertex_file.rdbuf();
		fragment_stream << fragment_file.rdbuf();
		// Закрываем файлы
		vertex_file.close();
		fragment_file.close();
		// Преобразовываем потоки в строки
		vertex_code = vertex_stream.str();
		fragment_code = fragment_stream.str();
	}
	catch(std::ifstream::failure error)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	// Переходим к работе с OpenGL
	// Те же исходники, преобразовываем тип
	const GLchar* gl_vertex_code = vertex_code.c_str();
	const GLchar* gl_fragment_code = fragment_code.c_str();
}