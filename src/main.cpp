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

    // Устанавливаем цвет, которым будем зачищать окно
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // Цикл отрисовки окна, в условии функция... ну очевидно, что делает
    while (!glfwWindowShouldClose(window))
    {
        // Проверка на наличие всяких событий
        glfwPollEvents();

        // Зацищаем окно
        glClear(GL_COLOR_BUFFER_BIT);

        // Замена буфера с цветами для каждого пикселя в window
        glfwSwapBuffers(window);
    }
 
    // После завершения цикла, закрывавем окно
    glfwTerminate();

    return 0;
}