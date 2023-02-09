#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

// Функция "обратного вызова", которая отслеживает нажатие ESC и закрывает окно
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

// Проверяем на ошибки компиляции шейдера
void check_shader_compilation(GLuint shader)
{
    // Число, которое определяет успешность сборки (флаг короче)
    GLint success;
    // Контейнер для хранения сообщения об ошибке
    GLchar info_log[512];
    // Собственно функция для проверки, потом ловим ошибки
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << info_log << std::endl;
    }
}

// Проверем на ошибки линковки программу
void check_program_linking(GLuint program)
{
    // Аналогично проверке на компиляцию шейдера
    GLint success;
    GLchar info_log[512];
    // Функция проверки, потом ловим ошибки
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, info_log);
        std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
    }
}

int main(void)
{
    // Инициализация и настройка GLFW
    glfwInit();
    // Минимальная необходимая версия OpenGL (мажорная и минорная)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    // "Установка профайла, для которого создается контекст"
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Запрет на динамическое изменение окна
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


    // Создаем объект окна
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Задаем window рабочим окном
    glfwMakeContextCurrent(window);
    // Передаем нашу рукописную callback-функцию в GLFW функцию проверки таких callback функций
    glfwSetKeyCallback(window, key_callback);


    if (!gladLoadGL())
    {
        std::cout << "Can't load GLAD" << std::endl;
        return -1;
    }


    int width, heigth;
    glfwGetFramebufferSize(window, &width, &heigth);
    glViewport(0, 0, width, heigth);


    const GLchar* vertex_shader_src =
        "#version 460 core\n"
        "layout (location = 0) in vec3 position;\n"
        "void main()\n"
        "{\n"
        "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
        "}\0";

    GLuint vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
    glCompileShader(vertex_shader);
    check_shader_compilation(vertex_shader);


    const GLchar* fragment_shader_one_src =
        "#version 460 core\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";
    const GLchar* fragment_shader_two_src =
        "#version 460 core\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "color = vec4(1.0f, 0.1f, 0.6f, 1.0f);\n"
        "}\0";

    GLuint fragment_shader_one, fragment_shader_two;

    fragment_shader_one = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_one, 1, &fragment_shader_one_src, NULL);
    glCompileShader(fragment_shader_one);
    check_shader_compilation(fragment_shader_one);

    fragment_shader_two = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_two, 1, &fragment_shader_two_src, NULL);
    glCompileShader(fragment_shader_two);
    check_shader_compilation(fragment_shader_two);

    GLuint shader_program_one, shader_program_two;

    shader_program_one = glCreateProgram();
    glAttachShader(shader_program_one, vertex_shader);
    glAttachShader(shader_program_one, fragment_shader_one);
    glLinkProgram(shader_program_one);
    check_program_linking(shader_program_one);

    shader_program_two = glCreateProgram();
    glAttachShader(shader_program_two, vertex_shader);
    glAttachShader(shader_program_two, fragment_shader_two);
    glLinkProgram(shader_program_two);
    check_program_linking(shader_program_two);
    

    // После линковки, шейдеры нужно удалить
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader_one);
    glDeleteShader(fragment_shader_two);


    GLfloat vertices_one[] = {
        -0.7f, 0.7f, 0.0f,
        -0.3f, 0.7f, 0.0f,
        -0.5f, 0.1f, 0.0f
    };
    GLfloat vertices_two[] = {
        -0.1f, -0.7f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.7f, -0.7f, 0.0f
    };

    GLuint indices[] = {
        0, 1, 2,
        3, 4, 5
    };


    GLuint VAO[2], VBO[2];

    // Треугольник первый
    glGenVertexArrays(1, &VAO[0]);
    glBindVertexArray(VAO[0]);

    glGenBuffers(1, &VBO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_one), vertices_one, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // Треугольник второй
    glGenVertexArrays(1, &VAO[1]);
    glBindVertexArray(VAO[1]);

    glGenBuffers(1, &VBO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_two), vertices_two, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);


    // Эта функция устанавливает режим отрисовки
    // Второй аргумент, например: GL_LINE - только линии, GL_FILL - заливка
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program_one);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glUseProgram(shader_program_two);
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);


        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}