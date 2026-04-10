#include "gtest/gtest.h"
#include "base.h"
#include "shader.h"
#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class light : public ::testing::Test {
protected:
    light() {
    }
    ~light() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }
};

static const int SCREEN_WIDTH = 800;
static const int SCREEN_HEIGHT = 600;
static Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
static float lastX = SCREEN_WIDTH / 2.0f;
static float lastY = SCREEN_HEIGHT / 2.0f;
static float firstMouse = true;
static float deltaTime = 0.0f; 
static float lastFrame = 0.0f;


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
static void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}


// glfw: whenever the mouse moves, this callback is called
static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

TEST_F(light, lightScene)
{
    initGLFW();
    GLFWwindow* window = createWindow();
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    // tell GLFW to capture our mouse
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
    std::string vsPath = std::string(RES_DIR) + "//object.vs";
    std::string fsPath = std::string(RES_DIR) + "//object.fs";
    Shader objShader(vsPath, fsPath);

    std::string lightVsPath = std::string(RES_DIR) + "//light.vs";
    std::string lightFsPath = std::string(RES_DIR) + "//light.fs";
    Shader lightShader(lightVsPath, lightFsPath);

    float vertices[] = {
        //---- 位置 ----
        // 前面
        -0.5f, -0.5f, 0.5f, // 前左下
        0.5f, -0.5f, 0.5f,  // 前右下
        0.5f,  0.5f, 0.5f,  // 前右上
        -0.5f,  0.5f, 0.5f, // 前左上

        // 后面
        -0.5f,  0.5f, -0.5f, // 后左上
        0.5f,  0.5f, -0.5f,  // 后右上
        0.5f, -0.5f, -0.5f,  // 后右下
        -0.5f, -0.5f, -0.5f, // 后左下

        // 左面
        -0.5f, -0.5f, -0.5f, // 后左下
        -0.5f, -0.5f, 0.5f,  // 前左下
        -0.5f, 0.5f, 0.5f,   // 前左上
        -0.5f, 0.5f, -0.5f,  // 后左上

        // 右面
        0.5f, -0.5f, 0.5f,   // 前右下
        0.5f, -0.5f, -0.5f,  // 后右下
        0.5f, 0.5f, -0.5f,   // 后右上
        0.5f, 0.5f, 0.5f,    // 前右上

        // 上面
        -0.5f, 0.5f, 0.5f,   // 前左上
        0.5f, 0.5f, 0.5f,    // 前右上
        0.5f, 0.5f, -0.5f,   // 后右上
        -0.5f, 0.5f, -0.5f,  // 后左上

        // 下面
        -0.5f, -0.5f, -0.5f, // 后左下
        0.5f, -0.5f, -0.5f,  // 后右下
        0.5f, -0.5f, 0.5f,   // 前右下
        -0.5f, -0.5f, 0.5f,  // 前左下
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); //启用顶点属性

    objShader.use();
    glm::vec3 objColor(1.0f, 0.5f, 0.31f);
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    objShader.setVector3f("objectColor", objColor);
    objShader.setVector3f("lightColor", lightColor);

    while (!glfwWindowShouldClose(window)) {

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);
        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 清空颜色缓冲区
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // glClear(GL_COLOR_BUFFER_BIT);

        objShader.use();
        glm::mat4 model = glm::mat4(1.0f);
        objShader.setMatrix4fv("model", model);
        glm::mat4 view = camera.GetViewMatrix();
        objShader.setMatrix4fv("view", view);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        objShader.setMatrix4fv("projection", projection);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    
        lightShader.use();
        model = glm::mat4(1.0f);
        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));

        lightShader.setMatrix4fv("model", model);
        lightShader.setMatrix4fv("view", view);
        lightShader.setMatrix4fv("projection", projection);
        glBindVertexArray(VAO);
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