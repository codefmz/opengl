#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float mixNum; // 从顶点着色器传来的输入变量（名称相同、类型相同）

void main()
{
    // linearly interpolate between both textures (80% container, 20% awesomeface)
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(1- TexCoord.x, TexCoord.y)), mixNum);
}