#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCord;
out vec3 ourColor;
out vec2 TexCord;

uniform mat4 RotationMatrix;
// 设置MVP矩阵
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    // 通过moveMat矩阵对顶点的位置进行修改
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCord = aTexCord;
};
