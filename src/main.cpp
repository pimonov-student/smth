#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <filesystem>
#include <iostream>
#include "render/shader.h"
#include "texture/stb_image.h"

#define STB_IMAGE_IMPLEMENTATION

// Пути к некоторым файлам
GLchar* v_shader_path;
GLchar* f_shader_path;

// Функция "обратного вызова", которая отслеживает нажатие ESC и закрывает окно
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

// Функция подгатавливает расположение файлов необходимых
void work_on_paths(std::string wd)
{
    std::string v_shader = wd + "\\src\\shader\\vertex_shader.vs";
    std::string f_shader = wd + "\\src\\shader\\fragment_shader.fs";

    char* v_tmp = v_shader.data();
    char* f_tmp = f_shader.data();

    v_shader_path = new char[v_shader.length() + 1];
    f_shader_path = new char[f_shader.length() + 1];

    strcpy(v_shader_path, v_tmp);
    strcpy(f_shader_path, f_tmp);
}

int main(void)
{
    // Работаем с путями к файлам путем избиения костылями
    std::filesystem::path cwd = std::filesystem::current_path();
    std::string wd = cwd.string();
    wd.resize(wd.length() - 6);
    work_on_paths(wd);

    // Инициализация и настройка GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
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
    int w_width, w_heigth;
    glfwGetFramebufferSize(window, &w_width, &w_heigth);
    glViewport(0, 0, w_width, w_heigth);


    // Работаем с текстурами
    // Задаем размеры изображения и количество цветовых каналов
    int i_width, i_heigth, i_channels;


    // Инициализируем шейдеры
    Shader shader(v_shader_path, f_shader_path);


    GLfloat vertices[] = {
        // Позиция              // Цвет
        -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f,       0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.0f,      0.0f, 0.0f, 1.0f
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

        shader.use();
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