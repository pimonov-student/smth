#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

int main(void)
{
    // Инициализация GLFW
    glfwInit();
    // Настройка GLFW
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

    // Цикл отрисовки окна, в условии функция... ну очевидно, что делает
    while (!glfwWindowShouldClose(window))
    {
        // Проверка на наличие всяких событий
        glfwPollEvents();
        // Замена буфера с цветами для каждого пикселя в window
        glfwSwapBuffers(window);
    }

    // После завершения цикла, закрывавем окно
    glfwTerminate();

    return 0;
}