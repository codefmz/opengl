#version 330 core
layout(location = 0) in vec3 aPos;
out vec4 vertexColor; // 为片段着色器指定一个颜色输出
void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vertexColor = vec4(0.0f, 0.7f, 0.8f, 1.0f); // 把输出变量设置为青蓝
};