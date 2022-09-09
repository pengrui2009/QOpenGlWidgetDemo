#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCord;
// 纹理单元【注：这里不需要在CPU端进行uniform的编写工作】
uniform sampler2D textureCity;

void main(){
    FragColor = texture(textureCity, TexCord);
};
