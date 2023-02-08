#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

// ������� "��������� ������", ������� ����������� ������� ESC � ��������� ����
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main(void)
{
    // ������������� GLFW
    glfwInit();
    // ��������� GLFW
    // ����������� ����������� ������ OpenGL (�������� � ��������)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    // "��������� ��������, ��� �������� ��������� ��������"
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // ������ �� ������������ ��������� ����
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // ������� ������ ����
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // ������ window ������� �����
    glfwMakeContextCurrent(window);
    // �������� ���� ���������� callback-������� � GLFW ������� �������� ����� callback �������
    glfwSetKeyCallback(window, key_callback);

    // ��������� ���������� GLAD (OpenGL)
    if (!gladLoadGL())
    {
        std::cout << "Can't load GLAD" << std::endl;
        return -1;
    }

    // �������� OpenGL ������ window
    int width, heigth;
    glfwGetFramebufferSize(window, &width, &heigth);

    glViewport(0, 0, width, heigth);

    // ������������� ����, ������� ����� �������� ����
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // ���� ��������� ����, � ������� �������... �� ��������, ��� ������
    while (!glfwWindowShouldClose(window))
    {
        // �������� �� ������� ������ �������
        glfwPollEvents();

        // �������� ����
        glClear(GL_COLOR_BUFFER_BIT);

        // ������ ������ � ������� ��� ������� ������� � window
        glfwSwapBuffers(window);
    }
 
    // ����� ���������� �����, ���������� ����
    glfwTerminate();

    return 0;
}