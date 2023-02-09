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


    // Загружаем библиотеку GLAD (OpenGL)
    if (!gladLoadGL())
    {
        std::cout << "Can't load GLAD" << std::endl;
        return -1;
    }


    // Передаем OpenGL размер window
    int width, heigth;
    glfwGetFramebufferSize(window, &width, &heigth);
    glViewport(0, 0, width, heigth);


    const GLchar* vertex_shader_src =
        "#version 460 core\n"
        "layout (location = 0) in vec3 position;\n"
        "layout (location = 1) in vec3 color;\n"
        "out vec3 vertex_color;\n"
        "void main()\n"
        "{\n"
        "gl_Position = vec4(position, 1.0f);\n"
        "vertex_color = color;\n"
        "}\0";

    GLuint vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
    glCompileShader(vertex_shader);
    check_shader_compilation(vertex_shader);


    const GLchar* fragment_shader_src =
        "#version 460 core\n"
        "in vec3 vertex_color;\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "color = vec4(vertex_color, 1.0f);\n"
        "}\0";

    GLuint fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
    glCompileShader(fragment_shader);
    check_shader_compilation(fragment_shader);


    // Создаем шейдерную программу (связываем написанные шейдеры воедино)
    GLuint shader_program;
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    check_program_linking(shader_program);


    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);


    GLfloat vertices[] = {
        // Позиция              // Цвет
        -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f,       0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.0f,      0.1f, 0.0f, 1.0f
    };


    // I
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // II
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // III
    // Так как теперь у нас помимо позиции появился цвет, указать нужно и его
    // Сначала укажем позицию, увеличив шаг
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Теперь по аналогии укажем на цвет
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // IV
    glBindVertexArray(0);


    // Эта функция устанавливает режим отрисовки
    // Второй аргумент, например: GL_LINE - только линии, GL_FILL - заливка
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);


        glfwSwapBuffers(window);
    }

    // Очищаем выделенную под эти объекты память
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // После завершения цикла, закрывавем окно
    glfwTerminate();

    return 0;
}