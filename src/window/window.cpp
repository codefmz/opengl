#include "gtest/gtest.h"
#include "base.h"
#include "shader.h"

class windowTest : public ::testing::Test {
protected:
    windowTest() {
    }
    ~windowTest() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }
};

TEST_F(windowTest, createWindow)
{
    initGLFW();
    GLFWwindow* window = createWindow();

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

TEST_F(windowTest, createTrangle)
{
    initGLFW();
    GLFWwindow* window = createWindow();
    Shader shader(R"(E:\Code\opengl\OpenGlTest1\src\window\shader.vs)",
        R"(E:\Code\opengl\OpenGlTest1\src\window\shader.fs)");

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    //1. 生成顶点数组对象与顶点缓冲对象
    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    //2. 绑定顶点数组对象与顶点缓冲对象
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // 3. 复制顶点数组到缓冲中供OpenGL使用
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 4. 解析顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); //启用顶点属性

    while (!glfwWindowShouldClose(window)) {
        // input
        processInput(window);
        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 清空颜色缓冲区
        glClear(GL_COLOR_BUFFER_BIT);
        // draw our first triangle
        shader.use();
        // 绑定 VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
}

TEST_F(windowTest, createTrangle2)
{
    initGLFW();
    GLFWwindow* window = createWindow();
    Shader shader(R"(E:\Code\opengl\OpenGlTest1\src\window\shader.vs)",
        R"(E:\Code\opengl\OpenGlTest1\src\window\shader.fs)");

    float vertices[] = {
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f, // 左下角
        -0.5f, 0.5f, 0.0f   // 左上角
    };

    unsigned int indices[] = {
        // 注意索引从0开始!
        // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
        // 这样可以由下标代表顶点组合成矩形

        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };

    //1. 生成顶点数组对象、顶点缓冲对象与顶点索引对象
    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    //2. 绑定顶点数组对象与顶点缓冲对象
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // 3. 复制顶点数组到缓冲中供OpenGL使用
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 4. 解析顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); //启用顶点属性


    // note that this is allowed, the call to glVertexAttribPointer registered VBO
    // as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active
    // as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //在 VAO 激活时不要解绑 EBO，否则会导致绘制时无法正确使用索引数据

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO,
    // but this rarely happens. Modifying other VAOs requires a call to glBindVertexArray anyways
    // so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        // 用于设置清空颜色缓冲区时使用的颜色, 为颜色缓冲区设置一个默认的背景颜色。
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 清空颜色缓冲区
        glClear(GL_COLOR_BUFFER_BIT);
        // draw our first triangle
        shader.use();
        // seeing as we only have a single VAO there's no need to bind it every time,
        // but we'll do so to keep things a bit more organized
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
}

TEST_F(windowTest, createTwoTrangleWithDifColor)
{
    initGLFW();
    GLFWwindow* window = createWindow();
    Shader shader(R"(E:\Code\opengl\OpenGlTest1\src\window\shader.vs)",
        R"(E:\Code\opengl\OpenGlTest1\src\window\shader.fs)");
    Shader shader2(R"(E:\Code\opengl\OpenGlTest1\src\window\shader.vs)",
        R"(E:\Code\opengl\OpenGlTest1\src\window\shader2.fs)");

    float vertices[] = {
        -0.8f, 0.0f, 0.0f,
        -0.4f, 0.4f, 0.0f,
        0.0f, 0.0f, 0.0f,
    };

    float vertices2[] = {
        0.0f, 0.0f, 0.0f,
        0.4f, 0.4f, 0.0f,
        0.8f, 0.0f, 0.0f,
    };

    unsigned int VBO[2], VAO[2];
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        // input
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        shader.use();
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        shader2.use();
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
}