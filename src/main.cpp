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

// ��������� �� ������ ���������� �������
void check_shader_compilation(GLuint shader)
{
    // �����, ������� ���������� ���������� ������ (���� ������)
    GLint success;
    // ��������� ��� �������� ��������� �� ������
    GLchar info_log[512];
    // ���������� ������� ��� ��������, ����� ����� ������
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << info_log << std::endl;
    }
}

// �������� �� ������ �������� ���������
void check_program_linking(GLuint program)
{
    // ���������� �������� �� ���������� �������
    GLint success;
    GLchar info_log[512];
    // ������� ��������, ����� ����� ������
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, info_log);
        std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
    }
}

int main(void)
{
    // ������������� � ��������� GLFW
    glfwInit();
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


    // ������ ������� ������������ (���������� � ���������� ������������)
    GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    };


    // �������� ��� ���������� ������� (������)
    const GLchar* vertex_shader_src =
        "#version 460 core\n"
        "layout (location = 0) in vec3 position;\n"
        "void main()\n"
        "{\n"
        "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
        "}\0";
    // ������� ������ ���������� �������
    GLuint vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    // ����������� �������� ������� � ������� �������
    glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
    // ����������� ������
    glCompileShader(vertex_shader);
    check_shader_compilation(vertex_shader);


    // �������� ��� ������������ ������� (������)
    const GLchar* fragment_shader_src =
        "#version 460 core\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";
    // ������ � ���������� ��������� ���������� �������, ���������� ������ ��� (GL_FRAGMENT_SHADER)
    GLuint fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
    glCompileShader(fragment_shader);
    check_shader_compilation(fragment_shader);


    // ������� ��������� ��������� (��������� ���������� ������� �������)
    GLuint shader_program;
    shader_program = glCreateProgram();
    // ������������ ������� � ���������
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    // ������� ��� ����
    glLinkProgram(shader_program);
    check_program_linking(shader_program);
    

    // ����� ��������, ������� ����� �������
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);


    // I
    // ������� VAO (Vertex Array Object)
    GLuint VAO;
    // ���������� ������
    glGenVertexArrays(1, &VAO);
    // ����������� VAO
    glBindVertexArray(VAO);

    // II
    // ������� VBO (Vertex Buffer Object)
    GLuint VBO;
    // ���������� �����
    glGenBuffers(1, &VBO);
    // ����������� � ���� GL_ARRAY_BUFFER (����������� ��� VBO ��� ������)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // �������� � VBO ����� ���� �������
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // III
    // ���� �� ����� ��������� �����, ��������� OpenGL �� ��, ��� ��� ����������������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
    // �������� ������� � �������� ��� ������� ���������
    glEnableVertexAttribArray(0);

    // IV
    // ���������� VAO
    glBindVertexArray(0);


    // ���� ��������� ����, � ������� �������... �� ��������, ��� ������
    while (!glfwWindowShouldClose(window))
    {
        // �������� �� ������� ������ �������
        glfwPollEvents();


        // ������������� ����, ������� ����� �������� ����
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // �������� ����
        glClear(GL_COLOR_BUFFER_BIT);

        // �������� ��������� ���������
        glUseProgram(shader_program);
        // ��������� VAO
        glBindVertexArray(VAO);
        // ������������ ��, ��� �������� � VAO
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // ���������� VAO
        glBindVertexArray(0);


        // ������ ������ � ������� ��� ������� ������� � window
        glfwSwapBuffers(window);
    }

    // ����� ���������� �����, ���������� ����
    glfwTerminate();

    return 0;
}