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


    // Задаем вершины треугольника (координаты в трехмерном пространстве)
    GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    };


    // Исходный код вершинного шейдера (строка)
    const GLchar* vertex_shader_src =
        "#version 460 core\n"
        "layout (location = 0) in vec3 position;\n"
        "void main()\n"
        "{\n"
        "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
        "}\0";
    // Создаем объект вершинного шейдера
    GLuint vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    // Привязываем исходник шейдера к объекту шейдера
    glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
    // Компилируем шейдер
    glCompileShader(vertex_shader);
    check_shader_compilation(vertex_shader);


    // Исходный код фрагментного шейдера (строка)
    const GLchar* fragment_shader_src =
        "#version 460 core\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";
    // Сборка и компиляция идентична вершинному шейдеру, отличается только тип (GL_FRAGMENT_SHADER)
    GLuint fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
    glCompileShader(fragment_shader);
    check_shader_compilation(fragment_shader);


    // Создаем шейдерную программу (связываем написанные шейдеры воедино)
    GLuint shader_program;
    shader_program = glCreateProgram();
    // Присоединяем шейдеры к программе
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    // Линкуем это дело
    glLinkProgram(shader_program);
    check_program_linking(shader_program);
    

    // После линковки, шейдеры нужно удалить
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);


    // I
    // Создаем VAO (Vertex Array Object)
    GLuint VAO;
    // Генерируем массив
    glGenVertexArrays(1, &VAO);
    // Привязываем VAO
    glBindVertexArray(VAO);

    // II
    // Создаем VBO (Vertex Buffer Object)
    GLuint VBO;
    // Генерируем буфер
    glGenBuffers(1, &VBO);
    // Привязываем к нему GL_ARRAY_BUFFER (необходимый для VBO тип буфера)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Передаем в VBO буфер наши вершины
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // III
    // Имея на руках вершинный буфер, указываем OpenGL на то, как его интерпретировать
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
    // Включаем атрибут и передаем ему позицию аргумента
    glEnableVertexAttribArray(0);

    // IV
    // Отвязываем VAO
    glBindVertexArray(0);


    // Цикл отрисовки окна, в условии функция... ну очевидно, что делает
    while (!glfwWindowShouldClose(window))
    {
        // Проверка на наличие всяких событий
        glfwPollEvents();


        // Устанавливаем цвет, которым будем зачищать окно
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // Зачищаем окно
        glClear(GL_COLOR_BUFFER_BIT);

        // Вызываем шейдерную программу
        glUseProgram(shader_program);
        // Связываем VAO
        glBindVertexArray(VAO);
        // Отрисовываем то, что хранится в VAO
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // Отвязываем VAO
        glBindVertexArray(0);


        // Замена буфера с цветами для каждого пикселя в window
        glfwSwapBuffers(window);
    }

    // После завершения цикла, закрывавем окно
    glfwTerminate();

    return 0;
}