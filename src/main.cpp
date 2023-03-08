#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem>
#include <iostream>
#include "shader/shader.h"
#include "texture/texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "texture/stb_image.h"

// Пути к некоторым файлам
GLchar* v_shader_path;
GLchar* f_shader_path;
GLchar* wall_texture_path;
GLchar* shrek_texture_path;

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
    std::string wall = wd + "\\src\\texture\\img\\wall.jpg";
    std::string shrek = wd + "\\src\\texture\\img\\shrek.png";

    char* v_tmp = v_shader.data();
    char* f_tmp = f_shader.data();
    char* wall_tmp = wall.data();
    char* shrek_tmp = shrek.data();

    v_shader_path = new char[v_shader.length() + 1];
    f_shader_path = new char[f_shader.length() + 1];
    wall_texture_path = new char[wall.length() + 1];
    shrek_texture_path = new char[shrek.length() + 1];

    strcpy(v_shader_path, v_tmp);
    strcpy(f_shader_path, f_tmp);
    strcpy(wall_texture_path, wall_tmp);
    strcpy(shrek_texture_path, shrek_tmp);
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
    int w_width, w_height;
    glfwGetFramebufferSize(window, &w_width, &w_height);
    glViewport(0, 0, w_width, w_height);


    // Создаем шейдеры (шейдерную программу)
    Shader shader(v_shader_path, f_shader_path);


    // Координаты (в том числе текстурные) объекта кубик (локальные координаты относительно его центра)
    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3
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
    // Создаем EBO/IBO (Element/Index Buffer Object)
    GLuint EBO;
    glGenBuffers(1, &EBO);
    // Привязываем к нему GL_ELEMENT_ARRAY_BUFFER (необходимый для EBO тип буфера)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // Передаем в EBO индексы (то есть порядок отрисовки элементов)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // IV
    // Сначала укажем позицию, увеличив шаг
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // То же самое для текстуры
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    // V
    glBindVertexArray(0);


    // Устанавливаем флажок, без которого stb_image будет загружать изображения перевернутыми
    stbi_set_flip_vertically_on_load(true);
    // Создаем текстуры
    Texture wall(wall_texture_path);
    Texture shrek(shrek_texture_path);


    // Эта функция устанавливает режим отрисовки
    // Второй аргумент, например: GL_LINE - только линии, GL_FILL - заливка
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Включаем проверку Z-буфера (чтобы корректно работало перекрытие олних фрагментов другими)
    glEnable(GL_DEPTH_TEST);


    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // Помимо очистки цветов изображения, будем очищать Z-буфер от значений предыдущего кадра
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Сделаем активным текстурный блок 0 и закрепим за ним текстуру стены
        glActiveTexture(GL_TEXTURE0);
        // Привязываем текстуру, чтобы переписанный фрагментный шейдер ее обнаружил (в нем мы указали ее как uniform)
        glBindTexture(GL_TEXTURE_2D, wall.texture);
        // Определяем uniform переменную
        glUniform1i(glGetUniformLocation(shader.program, "texture_one"), 0);
        // Делаем то же самое с шреком
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, shrek.texture);
        glUniform1i(glGetUniformLocation(shader.program, "texture_two"), 1);


        shader.use();


        glBindVertexArray(VAO);


        // Мы хотим чтоб текстура "покоилась на полу" и мы на нее сверху под углом будем "смотреть" при этом как бы "отойдя" назад
        // Начинаем с матрицы модели, которая переведет локальные координаты в мировые, по сути повернет текстуру вдоль оси Х (теперь она лежит на полу)
        glm::mat4 model(1.0f);
        model = glm::rotate(model, (GLfloat)glfwGetTime() * 0.5f, glm::vec3(1.0f, 1.0f, 0.0f));
        // Следующая матрица вида, которая "отодвинет" наблюдателя, на самом деле "пододвинет" все мировые координаты вперед от нас (теперь мы отошли назад)
        glm::mat4 view(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        // Последняя матрица проекции, она преобразует все координаты в соответствии с перспективой (теперь выглядит реалистичнее)
        glm::mat4 projection(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (GLfloat)w_width / (GLfloat)w_height, 0.1f, 100.0f);

        // Определяем наши матрицы как uniform переменные для вершинного шейдера
        GLint model_loc = glGetUniformLocation(shader.program, "model");
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
        GLint view_loc = glGetUniformLocation(shader.program, "view");
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
        GLint projection_loc = glGetUniformLocation(shader.program, "projection");
        glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));


        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Отвязываем текстуры
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);


        glfwSwapBuffers(window);
    }

    // Очищаем выделенную под эти объекты память
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    // После завершения цикла, закрывавем окно
    glfwTerminate();

    return 0;
}