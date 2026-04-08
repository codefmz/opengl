#include "gtest/gtest.h"
#include "base.h"
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class coordinate : public ::testing::Test {
protected:
    coordinate() {
    }
    ~coordinate() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }
};

TEST_F(coordinate, rotation3D)
{
    initGLFW();
    int screenWidth = 800;
    int screenHeight = 600;
    GLFWwindow* window = createWindow();
    std::string vsPath = std::string(RES_DIR) + "//shader_mix.vs";
    std::string fsPath = std::string(RES_DIR) + "//shader_mix.fs";
    Shader shader(vsPath, fsPath);

    float vertices[] = {
        //---- 位置 ----           - 纹理坐标 -
        0.5f,  0.5f, 0.0f,     1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f,     1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,    0.0f, 0.0f,   // 左下
        -0.5f,  0.5f, 0.0f,    0.0f, 1.0f    // 左上
    };

    unsigned int indices[] = {
        0, 1, 2, // first triangle
        2, 3, 0  // second triangle
    };

    //1. 生成顶点数组对象与顶点缓冲对象
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //2. 绑定顶点数组对象与顶点缓冲对象
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // 3. 复制顶点数组到缓冲中供OpenGL使用
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 4. 解析顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); //启用顶点属性
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1); //启用顶点属性

    std::string imgPath = std::string(RES_DIR) + "//container.jpg";
    std::string imgPath2 = std::string(RES_DIR) + "//awesomeface.png";
    unsigned int texture1, texture2;
    genTextureData(texture1, imgPath, GL_RGB, GL_REPEAT);
    //GL_RGBA 指定读取包含透明度的图片
    genTextureData(texture2, imgPath2, GL_RGBA, GL_REPEAT);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    shader.use(); // don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:
    shader.setInt("texture1", 0);
    // or set it via the texture class
    shader.setInt("texture2", 1);

    glm::mat4 model = glm::mat4(1.0f);
    // 围绕x轴旋转55度
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 view = glm::mat4(1.0f);
    // 视图坐标往后移动 -3
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    // 创建一个透视投影矩阵
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
    shader.setMatrix4fv("model", model);
    shader.setMatrix4fv("view", view);
    shader.setMatrix4fv("projection", projection);

    while (!glfwWindowShouldClose(window)) {
        // input
        processInput(window);
        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 清空颜色缓冲区
        glClear(GL_COLOR_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        shader.use();

        // 绑定 VBO
        glBindVertexArray(VAO);
        // draw our first triangle
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

TEST_F(coordinate, rotationMore3D)
{
    initGLFW();
    int screenWidth = 800;
    int screenHeight = 600;
    GLFWwindow* window = createWindow();
    glEnable(GL_DEPTH_TEST);
    std::string vsPath = std::string(RES_DIR) + "//shader_mix.vs";
    std::string fsPath = std::string(RES_DIR) + "//shader_mix.fs";
    Shader shader(vsPath, fsPath);

    float vertices[] = {
        //---- 位置 ----         - 纹理坐标 -
        // 前面
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,   // 前左下
        0.5f, -0.5f, 0.5f,   1.0f, 0.0f,   // 前右下
        0.5f,  0.5f, 0.5f,   1.0f, 1.0f,   // 前右上
        -0.5f,  0.5f, 0.5f,  0.0f, 1.0f,   // 前左上

        // 后面
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f,   // 后左上
        0.5f,  0.5f, -0.5f,  1.0f, 0.0f,   // 后右上
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,   // 后右下
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,   // 后左下

        // 左面
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,   // 后左下
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f,    // 前左下
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,     // 前左上
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,    // 后左上

        // 右面
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,   // 前右下
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  // 后右下
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // 后右上
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f,    // 前右上

        // 上面
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,   // 前左上
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    // 前右上
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // 后右上
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  // 后左上

        // 下面
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,   // 后左下
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,   // 后右下
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f,    // 前右下
        -0.5f, -0.5f, 0.5f, 0.0f, 1.0f   // 前左下
    };

    unsigned int indices[] = {
        0, 1, 2, // 前面
        2, 3, 0,

        4, 5, 6, // 后面
        6, 7, 4,

        8, 9, 10, // 左面
        10, 11, 8,

        12, 13, 14, // 右面
        14, 15, 12,

        16, 17, 18, // 上面
        18, 19, 16,

        20, 21, 22, // 下面
        22, 23, 20
    };

    //1. 生成顶点数组对象与顶点缓冲对象
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //2. 绑定顶点数组对象与顶点缓冲对象
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // 3. 复制顶点数组到缓冲中供OpenGL使用
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 4. 解析顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); //启用顶点属性
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1); //启用顶点属性

    std::string imgPath = std::string(RES_DIR) + "//container.jpg";
    std::string imgPath2 = std::string(RES_DIR) + "//awesomeface.png";
    unsigned int texture1, texture2;
    genTextureData(texture1, imgPath, GL_RGB, GL_REPEAT);
    //GL_RGBA 指定读取包含透明度的图片
    genTextureData(texture2, imgPath2, GL_RGBA, GL_REPEAT);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    shader.use(); // don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:
    shader.setInt("texture1", 0);
    // or set it via the texture class
    shader.setInt("texture2", 1);

    glm::mat4 view = glm::mat4(1.0f);
    // 视图坐标往后移动 -3
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    // 创建一个透视投影矩阵
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
    shader.setMatrix4fv("view", view);
    shader.setMatrix4fv("projection", projection);

    while (!glfwWindowShouldClose(window)) {
        // input
        processInput(window);
        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 清空颜色缓冲区
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // glClear(GL_COLOR_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        shader.use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        shader.setMatrix4fv("model", model);
        // 绑定 VBO
        glBindVertexArray(VAO);
        // draw our first triangle
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
}

TEST_F(coordinate, tenRotation3D)
{
    initGLFW();
    int screenWidth = 800;
    int screenHeight = 600;
    GLFWwindow* window = createWindow();
    glEnable(GL_DEPTH_TEST);
    std::string vsPath = std::string(RES_DIR) + "//shader_mix.vs";
    std::string fsPath = std::string(RES_DIR) + "//shader_mix.fs";
    Shader shader(vsPath, fsPath);

    float vertices[] = {
        //---- 位置 ----         - 纹理坐标 -
        // 前面
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,   // 前左下
        0.5f, -0.5f, 0.5f,   1.0f, 0.0f,   // 前右下
        0.5f,  0.5f, 0.5f,   1.0f, 1.0f,   // 前右上
        -0.5f,  0.5f, 0.5f,  0.0f, 1.0f,   // 前左上

        // 后面
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f,   // 后左上
        0.5f,  0.5f, -0.5f,  1.0f, 0.0f,   // 后右上
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,   // 后右下
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,   // 后左下

        // 左面
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,   // 后左下
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f,    // 前左下
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,     // 前左上
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,    // 后左上

        // 右面
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,   // 前右下
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  // 后右下
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // 后右上
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f,    // 前右上

        // 上面
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,   // 前左上
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    // 前右上
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // 后右上
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  // 后左上

        // 下面
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,   // 后左下
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,   // 后右下
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f,    // 前右下
        -0.5f, -0.5f, 0.5f, 0.0f, 1.0f   // 前左下
    };

    unsigned int indices[] = {
        0, 1, 2, // 前面
        2, 3, 0,

        4, 5, 6, // 后面
        6, 7, 4,

        8, 9, 10, // 左面
        10, 11, 8,

        12, 13, 14, // 右面
        14, 15, 12,

        16, 17, 18, // 上面
        18, 19, 16,

        20, 21, 22, // 下面
        22, 23, 20
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    //1. 生成顶点数组对象与顶点缓冲对象
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //2. 绑定顶点数组对象与顶点缓冲对象
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // 3. 复制顶点数组到缓冲中供OpenGL使用
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 4. 解析顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); //启用顶点属性
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1); //启用顶点属性

    std::string imgPath = std::string(RES_DIR) + "//container.jpg";
    std::string imgPath2 = std::string(RES_DIR) + "//awesomeface.png";
    unsigned int texture1, texture2;
    genTextureData(texture1, imgPath, GL_RGB, GL_REPEAT);
    //GL_RGBA 指定读取包含透明度的图片
    genTextureData(texture2, imgPath2, GL_RGBA, GL_REPEAT);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    shader.use(); // don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:
    shader.setInt("texture1", 0);
    // or set it via the texture class
    shader.setInt("texture2", 1);

    glm::mat4 view = glm::mat4(1.0f);
    // 视图坐标往后移动 -3
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    // 创建一个透视投影矩阵
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
    shader.setMatrix4fv("view", view);
    shader.setMatrix4fv("projection", projection);

    while (!glfwWindowShouldClose(window)) {
        // input
        processInput(window);
        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 清空颜色缓冲区
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // glClear(GL_COLOR_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        shader.use();
        // 绑定 VBO
        glBindVertexArray(VAO);
        for(unsigned int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * (i + 1); 
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMatrix4fv("model", model);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
}