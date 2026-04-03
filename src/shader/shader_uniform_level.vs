#version 330 core
layout (location = 0) in vec3 aPos;   // 位置变量的属性位置值为 0
layout (location = 1) in vec3 aColor; // 颜色变量的属性位置值为 1

out vec3 ourColor; // 向片段着色器输出一个颜色
uniform float time;
uniform float xPos;

void main()
{
    float yOffset = sin(time) / 2.0;
    gl_Position = vec4(aPos.x + xPos, aPos.y + yOffset, aPos.z, 1.0);
    ourColor = aColor;
};